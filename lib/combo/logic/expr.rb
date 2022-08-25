module Combo::Logic
  class Expr
  end

  class ExprBinary < Expr
    def initialize(left, right)
      @left = left
      @right = right
    end
  end

  class ExprOr < ExprBinary
    def eval(*args)
      [left, right].any? { |expr| expr.eval(*args) }
    end
  end

  class ExprAnd < Expr
    def eval(*args)
      [left, right].all? { |expr| expr.eval(*args) }
    end
  end

  class ExprHas < Expr
    def initialize(item)
      @item = item
    end

    def eval(items)
      !items[@item].nil?
    end
  end
end
