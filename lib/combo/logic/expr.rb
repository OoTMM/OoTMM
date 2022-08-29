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
      eval_impl(*args).result
    end

    def missing(*args)
      eval_impl(*args).missing
    end

    private
    def eval_impl(*args)
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

  class ExprNary < Expr
    def initialize(exprs)
      @exprs = exprs
    end
  end

  class ExprOr < ExprNary
    def eval_expr(*args)
      missing = Set.new
      @exprs.each do |expr|
        value = expr.eval_expr(*args)
        return SOLVED_VALUE if value.result
        missing.merge(value.missing)
      end
      Value.new(false, missing: missing)
    end
  end

  class ExprAnd < ExprNary
    def eval_expr(*args)
      missing = Set.new
      result = true
      @exprs.each do |expr|
        value = expr.eval_expr(*args)
        result = false unless value.result
        missing.merge(value.missing)
      end
      if result
        SOLVED_VALUE
      else
        Value.new(false, missing: missing)
      end
    end
  end

  class ExprHas < Expr
    def initialize(item)
      @item = item.to_sym
    end

    def eval_expr(age, state)
      if state.items[@item]
        SOLVED_VALUE
      else
        Value.new(false, missing: [@item])
      end
    end
  end

  class ExprReach < Expr
    def initialize(name)
      @name = name.to_sym
    end

    def eval_expr(age, state)
      if state.reachable_locations[age].include?(@name)
        SOLVED_VALUE
      else
        Value.new(false)
      end
    end
  end
end
