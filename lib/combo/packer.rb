require 'fileutils'
require 'combo/patcher'
require 'combo/compressor'
require 'combo/randomizer'

module Combo::Packer
  def self.run(opts)
    oot = Combo::Patcher.run(:oot)
    mm = Combo::Patcher.run(:mm)

    oot = Combo::Compressor.run(:oot, oot)
    mm = Combo::Compressor.run(:mm, mm)

    FileUtils.mkpath(Combo::PATH_DIST)
    File.open(File.join(Combo::PATH_DIST, 'OoTMM.z64'), "w+b") do |f|
      puts "Packing the rom..."
      STDOUT.flush
      f.truncate(64 * 1024 * 1024)
      f.write(oot)
      f.write(mm)

      Combo::GAMES.each do |game|
        puts "Packing payload for #{game}..."
        STDOUT.flush
        data = File.binread(File.join(Combo::PATH_BUILD, "#{game}_payload.bin"))
        addr = Combo::METADATA[game][:payload_addr]
        if (data.size > 0x20000)
          raise "Payload for #{game} is too big"
        end
        f.seek(addr)
        f.write(data)
      end

      puts "Packing custom data..."
      STDOUT.flush
      custom_data = File.binread(File.join(Combo::PATH_BUILD, "custom.bin"))
      addr = Combo::METADATA[:custom_addr]
      if (custom_data.size > 0x20000)
        raise "Custom data is too big"
      end
      f.seek(addr)
      f.write(custom_data)

      puts "Fixing MM DMA..."
      STDOUT.flush
      fix_mm_dma(f)

      puts "Patching the rom header..."
      STDOUT.flush
      f.seek(0x20)
      f.write("OOT+MM COMBO       ")

      f.seek(0x3c)
      f.write('ZZE')

      puts "Fixing the checksum..."
      STDOUT.flush
      fix_checksum(f)

      puts "Randomizing..."
      STDOUT.flush
      randomize(f)
    end
  end

  private
  def self.fix_mm_dma(f)
    meta = Combo::METADATA[:mm]
    f.seek(meta[:dma_addr] | 0x02000000)
    dma = Combo::DmaData.new(f.read(meta[:dma_count] * 0x10))
    dma.each do |i, e|
      next if e.dummy?
      pstart = e.pstart | 0x02000000
      pend = 0
      if e.compressed?
        pend = e.pend | 0x02000000
      end
      dma.write(i, e.vstart, e.vend, pstart, pend)
    end
    f.seek(meta[:dma_addr] | 0x02000000)
    f.write(dma.data)
  end

  def self.fix_checksum(f)
    f.seek(0)
    data = f.read(0x1000000)
    sum = checksum(data).pack('L>2')
    f.seek(0x10)
    f.write(sum)
  end

  def self.rol(v, b)
    ((v << b) | (v >> (32 - b))) & 0xffffffff
  end

  def self.checksum(data)
    seed = 0xDF26F436
    t1 = t2 = t3 = t4 = t5 = t6 = seed

    (0x100000 / 4).times do |i|
      offset = 0x1000 + i * 4
      d = data[offset,4].unpack('L>').first
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
      offset = 0x750 + ((i * 4) & 0xff)
      x = data[offset,4].unpack('L>').first
      t1 = (t1 + ((x ^ d))) & 0xffffffff
    end

    [t6 ^ t4 ^ t3, t5 ^ t2 ^ t1]
  end

  def self.randomize(f)
    payload = Combo::Randomizer.run()
    f.seek(0x03fe0000)
    f.write(payload)
  end
end
