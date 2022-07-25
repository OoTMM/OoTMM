require 'combo/common'

class Combo::FileBuffer
  def initialize(path, write=false)
    @file = File.open(path, write ? 'wb+' : 'rb+')
  end

  def init(size)
    return if size.zero?
    @file.seek(size - 1, :SET)
    @file.write([0].pack('C'))
  end

  def read32(addr)
    @file.seek(addr, :SET)
    @file.read(4).unpack('L>')[0]
  end

  def write32(addr, value)
    @file.seek(addr, :SET)
    @file.write([value].pack('L>'))
  end

  def write8(addr, value)
    @file.seek(addr, :SET)
    @file.write([value].pack('C'))
  end

  def write32_inplace(value)
    pos = @file.tell
    raw = @file.read(4)
    data = raw.nil? ? nil : raw.unpack('L>')[0]
    if data != value
      @file.seek(pos, :SET)
      @file.write([value].pack('L>'))
    end
  end

  def dump(addr, len)
    @file.seek(addr, :SET)
    @file.read(len)
  end

  def load(addr, data)
    @file.seek(addr, :SET)
    @file.write(data)
  end

  def finalize
    sums = checksum()
    write32(0x10, sums[0])
    write32(0x14, sums[1])
  end

  def close
    @file.close
  end

  def seek(addr)
    @file.seek(addr, :SET)
  end

  def tell
    @file.tell
  end

  private
  def rol(v, b)
    ((v << b) | (v >> (32 - b))) & 0xffffffff
  end

  def checksum
    seed = 0xDF26F436
    t1 = t2 = t3 = t4 = t5 = t6 = seed

    (0x100000 / 4).times do |i|
      offset = 0x1000 + i * 4
      d = read32(offset)
      if ((t6 + d) & 0xffffffff) < t6
        t4 = (t4 + 1) & 0xffffffff
      end
      t6 = (t6 + d) & 0xffffffff
      t3 = (t3 ^ d) & 0xffffffff
      r = rol(d, (d & 0x1f))
      t5 = (t5 + r) & 0xffffffff
      if t2 > d
        t2 = t2 ^ r
      else
        t2 = t2 ^ (t6 ^ d)
      end
      t1 = (t1 + ((read32(0x750 + ((i * 4) & 0xff))) ^ d)) & 0xffffffff
    end

    [t6 ^ t4 ^ t3, t5 ^ t2 ^ t1]
  end
end
