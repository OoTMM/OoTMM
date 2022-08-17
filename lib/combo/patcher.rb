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
          patch_load_store(f)
        when 0x3
          patch_rel_hilo(f)
        when 0x5
          patch_write32(f)
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

  def patch_load_store(f)
    bits, unsigned = *f.read(4).unpack('S>2')
    count = f.read(4).unpack('L>').first / 4

    op_load = nil
    case bits
    when 8
      op_load = 0x20
    when 16
      op_load = 0x21
    when 32
      op_load = 0x23
    else
      raise "Unknown load/store size #{bits}"
    end
    op_store = op_load | 8
    if !unsigned.zero?
      op_load |= 4
    end

    count.times do
      addr = f.read(4).unpack('L>').first
      offset = offset_for_addr(addr)
      raw = @data[offset, 4].unpack('L>').first
      opcode = (raw >> 26) & 0x3f
      case opcode
      when 0x20, 0x21, 0x23, 0x24, 0x25, 0x27
        opcode = op_load
      when 0x28, 0x29, 0x2b, 0x2c, 0x2d, 0x2f
        opcode = op_store
      else
        raise "Unknown opcode #{opcode}"
      end
      raw = (raw & 0x3ffffff) | (opcode << 26)
      @data[offset, 4] = [raw].pack('L>')
    end
  end

  def patch_rel_hilo(f)
    target, count = *f.read(8).unpack('L>2')
    count = count / 4
    target_lo = target & 0xffff
    target_hi = (target >> 16) & 0xffff
    if (target_lo & 0x8000) != 0
      target_hi = target_hi + 1
    end
    count.times do
      addr = f.read(4).unpack('L>').first
      offset = offset_for_addr(addr)
      raw = @data[offset, 4].unpack('L>').first
      opcode = (raw >> 26) & 0x3f
      value = target_lo
      if opcode == 0x0f
        value = target_hi
      end
      raw = (raw & 0xffff0000) | value
      @data[offset, 4] = [raw].pack('L>')
    end
  end

  def patch_write32(f)
    value = f.read(4)
    count = f.read(4).unpack('L>').first
    count = count / 4
    count.times do
      addr = f.read(4).unpack('L>').first
      offset = offset_for_addr(addr)
      @data[offset, 4] = value
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
