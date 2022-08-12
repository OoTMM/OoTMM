require 'combo/packer'
require 'combo/decompressor'
require 'combo/options'

module Combo
  def self.run(args)
    @opts = Options.new(args)

    decompress()
    build()
    pack()
  end

  def self.decompress()
    Combo::GAMES.each{|g| Combo::Decompressor.run(g)}
  end

  def self.build()
    unless system("make", "-C", "src")
      raise "Failed to build ASM"
    end
  end

  def self.pack()
    Combo::Packer.run(@opts)
  end
end
