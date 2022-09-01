require 'json'
require 'combo/logic/expr'
require 'combo/logic/util'

module Combo::Logic
  TOKENS = {
    '(' => :lparen,
    ')' => :rparen,
    ',' => :comma,
    'or' => :or,
    'and' => :and,
  }

  class ExprBuilder
    class Macro
      attr_reader :name, :args, :value

      def initialize(name, args, value)
        @name = name
        @args = args
        @value = value
      end
    end

    class Context
      attr_accessor :str, :cursor, :next, :scope

      def initialize(str, scope = {})
        @str = str
        @cursor = 0
        @next = nil
        @scope = scope
      end
    end

    def initialize(game)
      @game = game
      @macros = {}
      @contexts = []
    end

    def load_macros(path)
      File.open(path) do |f|
        JSON.parse(f.read).each do |k, v|
          # Horrible hack
          name, *args = *k.gsub(/\(|\)|,/, ' ').split(' ')
          @macros[name] = Macro.new(name, args, v)
        end
      end
    end

    def parse(str)
      @contexts << Context.new(str)

      expr = parse_expr()
      expect(:eof)

      expr
    end

    private
    def parse_expr
      if accept(:lparen)
        expr = parse_expr()
        expect(:rparen)
        expr
      else
        parse_expr_or()
      end
    end

    def parse_expr_or
      exprs = [parse_expr_and()]
      while accept(:or)
        exprs << parse_expr()
      end
      if exprs.size == 1
        exprs.first
      else
        ExprOr.new(exprs)
      end
    end

    def parse_expr_and
      exprs = [parse_expr_single()]
      while accept(:and)
        exprs << parse_expr()
      end
      if exprs.size == 1
        exprs.first
      else
        ExprAnd.new(exprs)
      end
    end

    def parse_expr_single
      e = parse_expr_has() || parse_expr_reach() || parse_expr_macro()
      if e.nil?
        raise "Unexpected token #{@contexts.last.next}"
      end
      e
    end

    def parse_expr_has
      return nil unless accept("has")
      expect(:lparen)
      item = Util.game_id(@game, expect(:id))
      expect(:rparen)
      ExprHas.new(item)
    end

    def parse_expr_reach
      return nil unless accept("reach")
      expect(:lparen)
      name = expect(:id)
      expect(:rparen)
      ExprReach.new(name)
    end

    def parse_expr_macro
      name = accept(:id)
      return nil if name.nil?
      macro = @macros[name]
      args = []
      if macro.nil?
        # Not a macro, put the token back
        @contexts.last.next = name
        return nil
      end
      if accept(:lparen)
        unless accept(:rparen)
          loop do
            arg = expect(:id)
            args << arg
            break if accept(:rparen)
            expect(:comma)
          end
        end
      end
      # We have all the args, now evaluate the macro
      if args.size != macro.args.size
        raise "Wrong number of arguments for macro #{name}: provided #{args.size}, expected #{macro.args.size}"
      end
      scope = macro.args.zip(args).to_h
      @contexts << Context.new(macro.value, scope)
      expr = parse_expr()
      expect(:eof)
      @contexts.pop
      expr
    end

    def accept(type)
      if @contexts.last.next.nil?
        @contexts.last.next = next_token
      end
      t = @contexts.last.next
      if type == :id && t.is_a?(String)
        @contexts.last.next = nil
        return t
      elsif type == t
        @contexts.last.next = nil
        return t
      end
      nil
    end

    def expect(type)
      t = accept(type)
      if t.nil?
        raise "Expected #{type} but found #{@contexts.last.next}"
      end
      t
    end

    def next_token
      ctx = @contexts.last

      skip_ws(ctx)
      if ctx.cursor >= ctx.str.length
        return :eof
      end

      buf = ctx.str[ctx.cursor..-1]
      TOKENS.each do |k, v|
        if buf[0,k.length] == k
          ctx.cursor += k.length
          return v
        end
      end

      id = buf[/^[a-zA-Z0-9_]+/]
      if id.nil?
        raise "Invalid token: #{buf[0,1]}"
      end
      ctx.cursor += id.length

      # Check for macro args
      substitution = ctx.scope[id]
      if substitution
        id = substitution
      end
      id
    end

    def skip_ws(ctx)
      ws = ctx.str[ctx.cursor..-1].index(/[^\s]/)
      if ws
        ctx.cursor += ws
      end
    end
  end
end
