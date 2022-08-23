require 'yaz0'
require 'fileutils'
require 'digest'
require 'combo/common'
require 'combo/dma_data'

module Combo::Decompressor
  def self.run(game)
    meta = Combo::METADATA[game]
    dst_dir = File.join(Combo::PATH_BUILD, "roms")
    stamp = File.join(dst_dir, "#{game}.stamp")
    FileUtils.mkdir_p(dst_dir)

    return if File.exists?(stamp)

    puts "Decompressing #{game}..."

    File.open(File.join(Combo::PATH_ROMS, "#{game}.z64"), "rb") do |src|
      # Check the hash of the ROMs
      hash = Digest::SHA256.hexdigest(src.read)
      valid_hashes = meta[:sha256]

      unless valid_hashes.include?(hash)
        raise "Invalid ROM hash for #{game}: #{hash}"
      end

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
      end
    end
    FileUtils.touch(stamp)
  end
end
