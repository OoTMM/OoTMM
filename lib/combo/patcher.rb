require 'combo/file'
require 'combo/assembler'

class Combo::Patcher
  def self.patch(path, &block)
    patcher = self.new(path)
    patcher.instance_eval &block
  end

  private
  def initialize(path)
    @file = Combo::File.new(path)
  end

  def asm(addr, &block)
    assembler = Combo::Assembler.new(@file, addr)
    assembler.instance_eval &block
  end

  def seek(offset)
    @file.seek(offset)
  end

  def tell
    @file.tell
  end

  def write32(value)
    @file.write32_inplace(value)
  end
end
