module Combo::Logic
  class Expr
    def eval(*args)
      eval_missing(*args).empty?
    end

    def missing(*args)
      eval_missing(*args).uniq
    end
  end

  class ExprBinary < Expr
    def initialize(left, right)
      @left = left
      @right = right
    end
  end

  class ExprOr < ExprBinary
    def eval_missing(*args)
      left_missing = @left.missing(*args)
      if left_missing.empty?
        []
      else
        right_missing = @right.missing(*args)
        if left_missing.size > right_missing.size
          right_missing
        else
          left_missing
        end
      end
    end
  end

  class ExprAnd < ExprBinary
    def eval_missing(*args)
      [@left, @right].map{|x| x.missing(*args)}.flatten
    end
  end

  class ExprHas < Expr
    def initialize(item)
      @item = item.to_sym
    end

    def eval_missing(items)
      if items[@item].nil?
        [@item]
      else
        []
      end
    end
  end
end
