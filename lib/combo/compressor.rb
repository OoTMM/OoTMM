require 'yaz0'
require 'combo/common'
require 'combo/dma_data'

module Combo::Compressor
  def self.run(game, data)
    meta = Combo::METADATA[game]
    dma_old = Combo::DmaData.new(File.binread(File.join(Combo::PATH_BUILD, "roms", "#{game}_dma.bin")))
    dma_new = Combo::DmaData.new(data[meta[:dma_addr], meta[:dma_count] * 0x10])
    new_data = "\x00" * (32 * 1024 * 1024)

    puts "Compressing #{game}..."
    paddr = 0
    dma_old.each do |i, e_old|
      next if e_old.dummy?
      e_new = dma_new.entry(i)
      src_paddr = e_new.pstart
      src_plen = e_new.plen
      d = data[src_paddr, src_plen]
      if e_old.compressed?
        d = Yaz0.compress(d).force_encoding('ASCII-8BIT')
      end
      size = (d.size + 15) & 0xfffffff0
      if e_old.compressed?
        dma_new.set_compressed(i, paddr, size)
      else
        dma_new.write(i, e_old.vstart, e_old.vend, paddr, 0)
      end
      new_data[paddr, d.length] = d
      paddr = paddr + size
    end

    # Write the new DMA table
    new_data[meta[:dma_addr], meta[:dma_count] * 0x10] = dma_new.data

    new_data
  end
end
