module Combo::Util
  def self.parse_syms(path)
    syms = {}
    File.open(path, 'rb') do |f|
      while line = f.gets
        line.chomp!
        columns = line.split(/\s+/)
        if columns.size >= 2
          addr = columns.first[8..16].to_i(16)
          name = columns.last
          syms[name] = addr
        end
      end
    end
    syms
  end
end
