require 'combo/common'

class Combo::Options
  attr_reader :out

  def initialize(args)
    while args.length > 0
      x = args.shift
      case x
        when "-o"
          v = args.shift
          if v.nil? || v.empty?
            raise "Missing output filename"
          end
          @out = v
        else
          raise "Unknown option: #{x}"
      end
    end
    @out ||= Combo::ROM_OOTMM
  end
end
