require 'combo/file_buffer'

REGS = [
  :zero, :at,
  :v0, :v1,
  :a0, :a1, :a2, :a3,
  :t0, :t1, :t2, :t3, :t4, :t5, :t6, :t7,
  :s0, :s1, :s2, :s3, :s4, :s5, :s6, :s7,
  :t8, :t9,
  :k0, :k1,
  :gp, :sp, :fp, :ra
].map.with_index{|x, i| [x, i]}.to_h.freeze

class Combo::Patcher
  def self.run()
    patcher = self.new
    patcher.run
  end

  def run
    puts "Loading the base ROM"
    @rom.load(0, File.binread(Combo::ROM_BASE))

    puts "Adding OoT payload"
    @rom.load(0x03fc0000, File.binread(File.join(Combo::PATH_BUILD, 'oot_payload.bin')))

    puts "Adding MM payload"
    @rom.load(0x03fe0000, File.binread(File.join(Combo::PATH_BUILD, 'mm_payload.bin')))

    Combo::GAMES.each do |game|
      apply_patches(game)
    end

    puts "Fixing metadata"
    @rom.load(0x20, "OOT+MM COMBO       ")

    puts "Fixing the checksum"
    @rom.finalize

    @rom.close
  end

  private
  def initialize
    @rom = Combo::FileBuffer.new(Combo::ROM_OOTMM, true)
    @rom.init(128 * 1024 * 1024)

    @offsets = Combo::GAMES.map{|g| [g, JSON.parse(File.read(File.join(Combo::PATH_BUILD, "#{g}_offsets.json")))]}.to_h
  end

  def apply_patches(game)
    puts "Applying patches for #{game}"
    File.open(File.join(Combo::PATH_BUILD, "#{game.downcase}_patch.bin"), "rb") do |f|
      while !f.eof? do
        addr, len = *f.read(8).unpack('L>L>')
        data = f.read(len)
        puts "Patching #{game} at 0x#{addr.to_s(16)}"
        offset = file_offset_from_addr(game, addr)
        @rom.load(offset, data)
      end
    end
  end

  VRAM = {
    "OoT" => {
      0x80000460..0x80006830 => "boot",
      0x800110A0..0x80114DD0 => "code",
    },
    "MM" => {
      0x80080060..0x8009b110 => "boot",
      0x800a76a0..0x801e3d8f => "code",
    }
  }

  def file_offset_from_addr(game, addr)
    table = VRAM[game]
    table.each do |range, file|
      if range.include?(addr)
        offset = addr - range.first
        base = @offsets[game][file]
        if game == "MM"
          base += 0x04000000
        end
        return base + offset
      end
    end
    raise "Unknown address 0x#{addr.to_s(16)} for #{game}"
  end
end
