require 'yaz0'
require 'fileutils'
require 'combo/common'
require 'combo/dma_data'

module Combo::Decompressor
  METADATA = {
    oot: {dma_addr: 0x7430, dma_count: 1510},
    mm: {dma_addr: 0x1a500, dma_count: 1552},
  }

  def self.run(game)
    meta = METADATA[game]
    dst_dir = File.join(Combo::PATH_BUILD, "roms")
    FileUtils.mkdir_p(dst_dir)

    File.open(File.join(Combo::PATH_ROMS, "#{game}.z64"), "rb") do |src|
      # Read and copy the DMA Data
      src.seek(meta[:dma_addr])
      dma = Combo::DmaData.new(src.read(meta[:dma_count] * 0x10))
      File.open(File.join(dst_dir, "#{game}_dma.bin"), "wb") do |f|
        f.write(dma.data)
      end

      File.open(File.join(dst_dir, "#{game}_decompressed.z64"), "wb") do |dst|
        # Prepare the 64 MiB rom
        dst.truncate(64 * 1024 * 1024)

        # Copy data
        dma.each do |i, e|
          next if e.dummy?
          src.seek(e.pstart)
          dst.seek(e.vstart)
          if e.compressed?
            Yaz0.decompress(src, dst)
          else
            data = src.read(e.plen)
            dst.write(data)
          end
          dma.set_decompressed(i)
        end

        # Write the new DMA data
        dst.seek(meta[:dma_addr])
        dst.write(dma.data)

        # Force english OoT
        if game == :oot
          dst.seek(0x3e)
          dst.write('E')
        end
      end
    end
  end
end
