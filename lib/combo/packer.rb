require 'combo/common'
require 'combo/file_buffer'

DMA = {
  'OoT' => [0x00007430, 0x0000d390, 0x00007430, 0x00000000],
  'MM' => [0x0001C110, 0x000222F0, 0x0001C110, 0x00000000],
}

module Combo::Packer
  def self.run()
    rom = Combo::FileBuffer.new(Combo::ROM_BASE, true)
    rom.init(128 * 1024 * 1024)

    Combo::GAMES.each_with_index do |name, index|
      base = index * 0x04000000
      dmadata = DMA[name]

      file_data = JSON.parse(File.read(File.join(Combo::PATH_DATA, "#{name}.json"))).freeze
      offsets = {}
      src = File.join(Combo::PATH_EXTRACT, name.downcase)

      paddr_base = 0

      file_data.each do |f|
        index = f["index"]
        compressed = false

        path = File.join(src, f["path"])
        puts "Packing #{path}"

        data = File.binread(path)
        rom.load(base + paddr_base, data)
        size = (data.size + 15) & 0xfffffff0

        rom.write32(base + dmadata[0] + 0x10 * index + 0x00, f["vstart"])
        rom.write32(base + dmadata[0] + 0x10 * index + 0x04, f["vend"])
        rom.write32(base + dmadata[0] + 0x10 * index + 0x08, paddr_base)

        if compressed
          rom.write32(base + dmadata[0] + 0x10 * index + 0x0c, paddr_base + size)
        else
          rom.write32(base + dmadata[0] + 0x10 * index + 0x0c, 0)
        end

        offsets[f["path"]] = paddr_base

        paddr_base += size
        if f["index"] == 1
          # Skip DMA data
          paddr_base = dmadata[1]
        end
      end

      File.write(File.join(Combo::PATH_BUILD, "#{name}_offsets.json"), offsets.to_json)

      rom.write32(base + dmadata[0] + 0x10 * 2 + 0x00, dmadata[0])
      rom.write32(base + dmadata[0] + 0x10 * 2 + 0x04, dmadata[1])
      rom.write32(base + dmadata[0] + 0x10 * 2 + 0x08, dmadata[2])
      rom.write32(base + dmadata[0] + 0x10 * 2 + 0x0c, dmadata[3])
    end

    # Force English, in case of JP rom
    rom.write8(0x3e, 'E'.ord)

    rom.close
  end
end
