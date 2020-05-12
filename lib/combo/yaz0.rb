module Combo::Yaz0
  def self.decompress(data)
    size = data[0x04,4].unpack('L>')[0]
    idx_group = 0x10
    idx_data = 0x11
    idx_out = 0x00
    out = ("\x00" * size).force_encoding('binary')

    loop do
      group_data = data[idx_group]
      break if group_data.nil?
      group = group_data.unpack('C')[0]
      8.times do
        break if size == idx_out
        backref = (group & 0x80).zero?
        group = group << 1
        if backref
          rrr = 0
          refsize = 0
          if (data[idx_data].unpack('C')[0] & 0xf0).zero?
            # 3 bytes
            refsize = data[idx_data + 2].unpack('C')[0] + 0x12
            rrr = data[idx_data,2].unpack('S>')[0] & 0xfff;
            idx_data += 3
          else
            # 2 bytes
            refsize = (data[idx_data].unpack('C')[0] >> 4) + 2
            rrr = data[idx_data,2].unpack('S>')[0] & 0xfff;
            idx_data += 2
          end
          idx_src = idx_out - (rrr + 1)
          refsize.times do
            out[idx_out] = out[idx_src]
            idx_out += 1
            idx_src += 1
          end
        else
          out[idx_out] = data[idx_data]
          idx_out += 1
          idx_data += 1
        end
      end
      idx_group = idx_data
      idx_data = idx_group + 1
    end
    out
  end

  def self.compress(data)
    out = []
    out << "Yaz0".force_encoding('binary')
    out << [data.size].pack('L>')
    out << "\x00\x00\x00\x00\x00\x00\x00\x00".force_encoding('binary')

    cursor = 0

    loop do
      break if cursor == data.size
      cursor = compress_group(out, data, cursor)
    end

    out.join ""
  end

  private
  def self.compress_group(out, data, cursor)
    runs = []
    group_byte = 0x00

    8.times do |i|
      run = compress_chunk(data, cursor)
      if run
        runs << run
        cursor += run_length(run)
        if run[0] == :data
          group_byte |= (1 << (7 - i))
        end
      end
    end

    out << ([[group_byte].pack('C')] + runs.map{|x| run_encode(x)}).join("")
    cursor
  end

  def self.compress_chunk(data, cursor)
    # To save a byte, at least 3 bytes must be compressed
    # but there may be a repeating candidate
    if data.size == cursor
      return nil
    end

    run_a = run_data(data[cursor])
    run_b = search_largest_pattern(data, cursor)
    run_best(run_a, run_b)
  end

  def self.search_largest_pattern(data, cursor)
    min_size = 1
    max_size = data.size - cursor
    if max_size > 0x111
      max_size = 0x111
    end

    run = nil
    max_size.times do |i|
      len = i + 1
      pattern = data[cursor, len]
      new_run = search_pattern(data, cursor, pattern)
      if new_run
        run = new_run
      else
        break
      end
    end
    run
  end

  def self.search_pattern(data, cursor, pattern)
    len = pattern.size
    max = cursor - len
    min = cursor - 0x1000
    if min < 0
      min = 0
    end
    cursor_pattern = max
    loop do
      break if cursor_pattern < min
      if data[cursor_pattern, len] == pattern
        return run_ref(cursor_pattern - cursor, len)
      end
      cursor_pattern -= 1
    end
    nil
  end

  def self.run_data(byte)
    [:data, byte]
  end

  def self.run_ref(offset, length)
    [:ref, offset, length]
  end

  def self.run_best(a, b)
    return a if b.nil?
    return b if a.nil?
    [a, b].sort_by {|x| run_cost(x)}[0]
  end

  def self.run_cost(run)
    if run[0] == :data
      1
    else
      if run[2] >= 0x12
        3 + run[2]
      else
        2 + run[2]
      end
    end
  end

  def self.run_length(run)
    if run[0] == :data
      1
    else
      run[2]
    end
  end

  def self.run_encode(run)
    if run[0] == :data
      run[1]
    else
      offset = 1 - run[1]
      len = run[2]
      if len < 0x12
        len = len - 2
        payload = (len << 12) | offset
        [payload].pack('L>')
      else
        len = len - 0x12
        [offset, len].pack('L>C')
      end
    end
  end
end
