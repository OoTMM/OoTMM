require 'combo/common'

class Combo::Options
  def initialize(args)
    while args.length > 0
      x = args.shift
      raise "Unknown option: #{x}"
    end
  end
end
