require 'combo/packer'
require 'combo/decompressor'

module Combo
  def self.run(args)
    decompress()
    build()
    pack()
    #extract_roms()
    #pack_rom()
    #build_asm()
    #patch_rom()
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
    Combo::Packer.run()
  end
end
