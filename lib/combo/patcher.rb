require 'combo/common'
require 'combo/dma_data'

class Combo::Patcher
  # Hardcoded entries
  VRAM_DATA = {
    oot: {
      0x80000460..0x80006830 => 0x1060,     # boot
      0x800110a0..0x80114dd0 => 0xa87000,   # code
      0x80800000..0x808009c0 => 0xb9da40,   # ovl_title
      0x80803880..0x8081379f => 0xba12c0,   # ovl_file_choose
      0x808301c0..0x80856720 => 0xbcdb70,   # ovl_player_actor
      0x808137c0..0x8083014f => 0xbb11e0,   # kaleido_scope
    },
    mm: {
      0x80080060..0x8009b110 => 0x1060,   # boot
      0x800a5ac0..0x801e3fa0 => 0xb3c000, # code
      0x80800000..0x80800910 => 0xc7a4e0, # ovl_title
      0x80803df0..0x80804010 => 0xc7e2d0, # ovl_opening
      0x80804010..0x80814e80 => 0xc7e4f0, # ovl_file_choose
      0x8082da90..0x80862af0 => 0xca7f00, # ovl_player_actor
    }
  }

  def self.run(*args)
    x = self.new(*args)
    x.run
  end

  def initialize(game)
    @game = game
    @data = File.binread(File.join(Combo::PATH_BUILD, 'roms', "#{@game}_decompressed.z64"))
    make_vram_table()
  end

  def run()
    puts "Patching #{@game}..."
    STDOUT.flush
    File.open(File.join(Combo::PATH_BUILD, "#{@game}_patch.bin"), "rb") do |f|
      while !f.eof? do
        # Align on 4-byte boundary
        f.seek((4 - (f.pos % 4)) % 4, IO::SEEK_CUR)
        type = f.read(4).unpack('L>').first
        case type
        when 0x1
          patch_vram(f)
        when 0x2
          patch_load_store(f)
        when 0x3
          patch_rel_hilo(f)
        when 0x4
          patch_rel_jump(f)
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

  def patch_rel_jump(f)
    target, count = *f.read(8).unpack('L>2')
    count = count / 4
    target = ((target & 0x0fffffff) >> 2)
    count.times do
      addr = f.read(4).unpack('L>').first
      offset = offset_for_addr(addr)
      raw = @data[offset, 4].unpack('L>').first
      raw = ((raw & 0xfc000000) | target)
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

  def offset_for_addr(addr)
    @vram.each do |range, base|
      if range.include?(addr)
        offset = addr - range.first
        return base + offset
      end
    end
    raise "Unknown address 0x#{addr.to_s(16)} for #{@game}"
  end

  def make_vram_table()
    @vram = VRAM_DATA[@game].dup
    base = Combo::METADATA[@game][:actors_ovl_addr]
    count = Combo::METADATA[@game][:actors_ovl_count]
    count.times do |i|
      off = base + i * 0x20
      romStart, romEnd, ramStart, ramEnd = *@data[off, 0x10].unpack('L>4')
      if ramStart.zero?
        next
      end
      @vram[ramStart..ramEnd] = romStart
    end
  end
end
