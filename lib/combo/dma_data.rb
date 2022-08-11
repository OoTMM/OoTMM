class Combo::DmaData
  class Entry
    def initialize(buffer)
      @buffer = buffer.dup
    end

    def raw_vstart
      @buffer[0x0,4].unpack('L>').first
    end

    def raw_vend
      @buffer[0x4,4].unpack('L>').first
    end

    def raw_pstart
      @buffer[0x8,4].unpack('L>').first
    end

    def raw_pend
      @buffer[0xc,4].unpack('L>').first
    end

    def compressed?
      raw_pend != 0
    end

    def dummy?
      pstart == 0xffffffff || pend == 0xffffffff
    end

    def vstart
      raw_vstart
    end

    def vend
      raw_vend
    end

    def pstart
      raw_pstart
    end

    def pend
      if compressed?
        raw_pend
      else
        raw_pstart + raw_vend - raw_vstart
      end
    end

    def vlen
      vend - vstart
    end

    def plen
      pend - pstart
    end
  end

  def initialize(buffer)
    @buffer = buffer.dup
  end

  def entry(i)
    Entry.new(@buffer[i * 0x10, 0x10])
  end

  def data
    @buffer
  end

  def count
    @buffer.size / 0x10
  end

  def each(&block)
    count.times do |i|
      block.call(i, entry(i))
    end
  end

  def set_decompressed(i)
    e = entry(i)
    write(i, e.vstart, e.vend, e.vstart, 0)
  end

  def set_compressed(i, pstart, size)
    pend = pstart + size
    e = entry(i)
    write(i, e.vstart, e.vend, pstart, pend)
  end

  def write(i, vstart, vend, pstart, pend)
    data = [vstart, vend, pstart, pend].pack('L>4')
    @buffer[i*0x10,0x10] = data
  end
end
