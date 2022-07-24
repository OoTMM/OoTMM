require 'json'
require 'fileutils'
require 'yaz0'
require 'combo/common'

module Combo::Extractor
  def self.run()
    Combo::GAMES.each do |game|
      src = File.join(Combo::PATH_ROMS, "#{game}.z64")
      dst = File.join(Combo::PATH_EXTRACT, game.downcase)

      data = File.binread(src)
      file_data = Combo::DATA_FILES[game]

      file_data.each do |f|
        path = f["path"]
        vstart = f["vstart"]
        vend = f["vend"]
        pstart = f["pstart"]
        pend = f["pend"]

        vlen = vend - vstart
        if pend.zero?
          plen = vlen
        else
          plen = pend - pstart
        end

        d = data[pstart, plen]
        p = File.join(dst, path)
        FileUtils.mkdir_p(File.dirname(p))
        if f["compressed"]
          puts "Extracting #{p}.yaz0"
          File.binwrite("#{p}.yaz0", d)
          d = Yaz0.decompress(d)
        end
        puts "Extracting #{p}"
        File.binwrite("#{p}", d)
        if f["compressed"]
          FileUtils.touch("#{p}.yaz0", nocreate: true)
        end
      end
    end
  end
end
