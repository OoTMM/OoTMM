require 'set'

module Combo::Logic
  class Expr
    class Value
      attr_accessor :result, :missing

      def initialize(result, opts = {})
        @result = !!result
        @missing = Set.new(opts[:missing] || [])
      end
    end

    SOLVED_VALUE = Value.new(true)

    def initialize
      @solved = false
    end

    def eval(*args)
      # We want to memoize true exprs
      if @solved
        SOLVED_VALUE
      else
        value = eval_expr(*args)
        if value.result
          @solved = true
          SOLVED_VALUE
        else
          value
        end
      end
    end
  end

  class ExprBinary < Expr
    def initialize(left, right)
      @left = left
      @right = right
    end
  end

  class ExprOr < ExprBinary
    def eval_expr(*args)
      left = @left.eval(*args)
      if left.result
        return SOLVED_VALUE
      end
      right = @right.eval(*args)
      if right.result
        return SOLVED_VALUE
      end
      Value.new(false, missing: left.missing | right.missing)
    end
  end

  class ExprAnd < ExprBinary
    def eval_expr(*args)
      left = @left.eval(*args)
      right = @right.eval(*args)
      if left.result && right.result
        SOLVED_VALUE
      else
        Value.new(false, missing: left.missing | right.missing)
      end
    end
  end

  class ExprHas < Expr
    def initialize(item)
      @item = item.to_sym
    end

    def eval_expr(graph, items)
      if items[@item]
        SOLVED_VALUE
      else
        Value.new(false, missing: [@item])
      end
    end
  end

  class ExprReach < Expr
    def initialize(name)
      @name = name
    end

    def eval_expr(graph, items)
      if graph.get_room(@name).reachable
        SOLVED_VALUE
      else
        Value.new(false)
      end
    end
  end
end
