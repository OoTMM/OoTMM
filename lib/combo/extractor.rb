require 'json'
require 'fileutils'

require 'combo/common'
require 'combo/yaz0'

class Combo::Extractor
  def initialize(rom)
    @rom = rom
    @filenames = JSON.parse(File.read(File.join(Combo::ROOT, 'data', 'OoT.json'))).to_a.map{|x| [x[0].to_i(16), x[1]]}.to_h
    @files = []
  end

  def run!(dst)
    FileUtils.mkpath(dst)

    1510.times do |idx|
      base = 0x7430 + idx * 0x10
      vstart = @rom.read32(base + 0x00)
      vend = @rom.read32(base + 0x04)
      pstart = @rom.read32(base + 0x08)
      pend = @rom.read32(base + 0x0c)

      if @filenames[vstart].nil?
        puts "Error at VROM 0x#{vstart.to_s(16)}"
      end

      len = 0
      vlen = 0
      compressed = false
      if pend.zero?
        len = vend - vstart
        vlen = len
      else
        vlen = vend - vstart
        len = pend - pstart
        compressed = true
      end

      extract(pstart, len, vlen, compressed, File.join(dst, @filenames[vstart]))
      @files << { path: @filenames[vstart], offset: vstart, len: vlen, compressed: compressed }
    end

    File.write(File.join(dst, "map.json"), @files.to_json)
  end

  private
  def extract(addr, len, vlen, compressed, filename)
    FileUtils.mkpath(File.dirname(filename))
    data = @rom.dump(addr, len)
    if compressed
      File.binwrite(filename + ".yaz0", data)
      data = Combo::Yaz0.decompress(data)
    end
    if data.size != vlen
      raise StandardError, "Bad file length for #{filename}, got 0x#{data.size.to_s(16)}, expected 0x#{vlen.to_s(16)}"
    end
    File.binwrite(filename, data)
  end
end
