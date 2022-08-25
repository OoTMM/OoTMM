require 'combo/logic/expr'

module Combo::Logic
  TOKENS = {
    '(' => :lparen,
    ')' => :rparen,
    'or' => :or,
    'and' => :and,
  }

  class ExprBuilder
    def parse(str)
      @str = str
      @cursor = 0
      @next = nil

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
      left = parse_expr_and()
      if accept(:or)
        right = parse_expr()
        ExprOr.new(left, right)
      else
        left
      end
    end

    def parse_expr_and
      left = parse_expr_single()
      if accept(:and)
        right = parse_expr()
        ExprAnd.new(left, right)
      else
        left
      end
    end

    def parse_expr_single
      parse_expr_has()
    end

    def parse_expr_has
      expect("has")
      expect(:lparen)
      item = expect(:id)
      expect(:rparen)
      ExprHas.new(item)
    end

    def accept(type)
      if @next.nil?
        @next = next_token
      end
      t = @next
      if type == :id && t.is_a?(String)
        @next = nil
        return t
      elsif type == t
        @next = nil
        return t
      end
      nil
    end

    def expect(type)
      t = accept(type)
      if t.nil?
        raise "Expected #{type} but found #{@next}"
      end
      t
    end

    def next_token
      skip_ws()

      if @cursor >= @str.length
        return :eof
      end

      buf = @str[@cursor..-1]
      TOKENS.each do |k, v|
        if buf[0,k.length] == k
          @cursor += k.length
          return v
        end
      end

      id = buf[/^[a-zA-Z0-9_]+/]
      if id.nil?
        raise "Invalid token: #{buf[0,1]}"
      end
      @cursor += id.length
      id
    end

    def skip_ws()
      ws = @str[@cursor..-1].index(/[^\s]/)
      if ws
        @cursor += ws
      end
    end
  end
end
