require 'combo/common'
require 'combo/dma_data'

class Combo::Patcher
  VRAM = {
    oot: {
      0x80000460..0x80006830 => "boot",
      0x800110a0..0x80114dd0 => "code",
      0x80800000..0x808009c0 => "ovl_title",
      0x80803880..0x8081379f => "ovl_file_choose",
      0x808301c0..0x80856720 => "ovl_player_actor",
      0x80aecdf0..0x80aefd00 => "actors/ovl_En_Mag",
    },
    mm: {
      0x80080060..0x8009b110 => "boot",
      0x800a5ac0..0x801e3fa0 => "code",
      0x80800000..0x80800910 => "ovl_title",
      0x80803df0..0x80804010 => "ovl_opening",
      0x80804010..0x80814e80 => "ovl_file_choose",
      0x8096b310..0x8096ec40 => "actors/ovl_En_Mag",
    }
  }

  def self.run(*args)
    x = self.new(*args)
    x.run
  end

  def initialize(game)
    @game = game
    @data = File.binread(File.join(Combo::PATH_BUILD, 'roms', "#{@game}_decompressed.z64"))
    make_lookup_table()
  end

  def run()
    File.open(File.join(Combo::PATH_BUILD, "#{@game}_patch.bin"), "rb") do |f|
      while !f.eof? do
        type = f.read(4).unpack('L>').first
        case type
        when 0x1
          patch_vram(f)
        when 0x2
          patch_expand16(f)
        else
          raise "Unknown patch type #{type}"
        end
      end
    end

    @data
  end

  def patch_vram(f)
    addr, len = *f.read(8).unpack('L>L>')
    data = f.read(len)
    puts "Patching #{@game} at 0x#{addr.to_s(16)}"
    offset = offset_for_addr(addr)
    @data[offset, data.size] = data
  end

  def patch_expand16(f)
    count = f.read(4).unpack('L>').first / 4
    count.times do
      addr = f.read(4).unpack('L>').first
      puts "Expanding (16 bits) #{@game} at 0x#{addr.to_s(16)}"
      offset = offset_for_addr(addr)
      raw = @data[offset, 4].unpack('L>').first
      opcode = (raw >> 26) & 0x3f
      case opcode
      # Store
      when 0x28
        opcode = 0x29
      when 0x20
        opcode = 0x21
      else
        raise "Unknown opcode #{opcode}"
      end
      raw = (raw & 0x3ffffff) | (opcode << 26)
      @data[offset, 4] = [raw].pack('L>')
    end
  end

  def make_lookup_table()
    meta = Combo::METADATA[@game]
    files = Combo::DATA_FILES[@game]
    dma = Combo::DmaData.new(@data[meta[:dma_addr], meta[:dma_count] * 0x10])
    @offsets = {}
    dma.each do |i, e|
      next if e.dummy?
      @offsets[files[i]] = e.pstart
    end
  end

  def offset_for_addr(addr)
    table = VRAM[@game]
    table.each do |range, file|
      if range.include?(addr)
        offset = addr - range.first
        base = @offsets[file]
        return base + offset
      end
    end
    raise "Unknown address 0x#{addr.to_s(16)} for #{@game}"
  end
end
