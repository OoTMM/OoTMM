require 'combo/extractor'
require 'combo/packer'
require 'combo/patcher'
require 'combo/decompressor'

module Combo
  def self.run(args)
    Combo::GAMES.each{|g| Combo::Decompressor.run(g)}
    #extract_roms()
    #pack_rom()
    #build_asm()
    #patch_rom()
  end

  private
  def self.extract_roms
    # Extract the ROMs if they haven't been extracted yet.
    if !File.exist?(Combo::STAMP_EXTRACT)
      puts "Extracting ROMs..."
      Combo::Extractor.run()
      FileUtils.touch(Combo::STAMP_EXTRACT)
    end
  end

  def self.pack_rom
    if !File.exist?(Combo::ROM_BASE)
      puts "Preparing the base ROM..."
      Combo::Packer.run()
    end
  end

  def self.build_asm()
    unless system("make", "-C", "src")
      raise "Failed to build ASM"
    end
  end

  def self.patch_rom()
    puts "Patching ROM..."
    Combo::Patcher.run()
  end
end
