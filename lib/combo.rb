require 'combo/packer'
require 'combo/decompressor'
require 'combo/options'
require 'combo/custom'
require 'combo/logic'

module Combo
  def self.run(args)
    @opts = Options.new(args)

    decompress()
    custom()
    build()
    pack()
  end

  def self.decompress()
    Combo::GAMES.each{|g| Combo::Decompressor.run(g)}
  end

  def self.custom()
    Custom.run()
  end

  def self.build()
    unless system("make", "-j", "32")
      raise "Failed to build ASM"
    end
  end

  def self.pack()
    Combo::Packer.run(@opts)
  end
end
