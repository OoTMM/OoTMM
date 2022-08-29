require 'combo/packer'
require 'combo/decompressor'
require 'combo/options'
require 'combo/custom'
require 'combo/logic'

module Combo
  def self.run(args)
    Logic.run
    exit 1
    case args.first
    when '--generate-custom'
      run_generate_custom()
    else
      run_build(args)
    end
  end

  def self.run_build(args)
    @opts = Options.new(args)

    decompress()
    build()
    pack()
  end

  def self.run_generate_custom()
    Custom.generate()
  end

  def self.decompress()
    Combo::GAMES.each{|g| Combo::Decompressor.run(g)}
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
