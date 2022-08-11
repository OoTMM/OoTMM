require 'digest'
require 'yaz0'
require 'thread/pool'
require 'combo/common'
require 'combo/dma_data'

module Combo::Compressor
  def self.run(game, data)
    meta = Combo::METADATA[game]
    dma_old = Combo::DmaData.new(File.binread(File.join(Combo::PATH_BUILD, "roms", "#{game}_dma.bin")))
    dma_new = Combo::DmaData.new(data[meta[:dma_addr], meta[:dma_count] * 0x10])
    new_data = "\x00" * (32 * 1024 * 1024)

    puts "Compressing #{game}..."
    files = compress_files(data, dma_old, dma_new)
    paddr = 0
    dma_old.each do |i, e|
      next if e.dummy?
      d = files[i]
      size = (d.size + 15) & 0xfffffff0
      if e.compressed?
        dma_new.set_compressed(i, paddr, size)
      else
        dma_new.write(i, e.vstart, e.vend, paddr, 0)
      end
      new_data[paddr, d.length] = d
      paddr = paddr + size
    end

    # Write the new DMA table
    new_data[meta[:dma_addr], meta[:dma_count] * 0x10] = dma_new.data

    new_data
  end

  def self.compress_files(data, dma_old, dma_new)
    pool = Thread.pool(32)
    files = Array.new(dma_old.count)
    dma_new.each do |i, e_new|
      next if e_new.dummy?
      pool.process do
        e_old = dma_old.entry(i)
        d = data[e_new.pstart, e_new.plen]
        if e_old.compressed?
          hash = Digest::SHA1.hexdigest(d)
          cache_dir = File.join(Combo::PATH_BUILD, "cache", "yaz0", hash[0,2])
          path = File.join(cache_dir, hash[2..])
          if File.exists?(path)
            d = File.binread(path)
          else
            d = Yaz0.compress(d).force_encoding('ASCII-8BIT')
            FileUtils.mkdir_p(cache_dir)
            File.binwrite(path, d)
          end
        end
        files[i] = d
      end
    end
    pool.shutdown
    files
  end
end
