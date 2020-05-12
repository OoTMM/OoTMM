require 'combo/yaz0'

class Combo::Packer
  def initialize(rom, root)
    @rom = rom
    @root = root
    @files = JSON.parse(File.read(File.join(@root, 'map.json')))
  end

  def run!()
    @files.each.with_index {|x, idx| pack_file(idx, x)}
    @rom.finalize
  end

  private
  def pack_file(idx, file)
    if file["path"] == "dmadata"
      fix_dma(idx, file)
    else
      compressed = file["compressed"]
      data = File.binread(File.join(@root, file["path"]))
      if compressed
        data = Combo::Yaz0.compress(data)
        p file["path"]
      end
      @rom.load(file["offset"], data)
      fix_dma(idx, file)
    end
  end

  def fix_dma(idx, file)
    base = 0x7430 + idx * 0x10
    @rom.write32(base + 0x00, file["offset"])
    @rom.write32(base + 0x04, file["offset"] + file["len"])
    @rom.write32(base + 0x08, file["offset"])
    @rom.write32(base + 0x0c, 0)
  end
end
