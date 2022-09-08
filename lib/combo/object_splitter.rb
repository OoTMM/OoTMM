module Combo
  # Splits large objects into smaller ones.
  # Used for MM boss remains
  class ObjectSplitter
    class Object
      attr_accessor :data, :offsets

      def initialize(data, offsets)
        @data = data
        @offsets = offsets
      end
    end

    def initialize(object, offsets)
      @object = object
      @offsets = offsets
      @buffer = "".force_encoding("ASCII-8BIT")
      @offsets_new = []
      @shifts = {}
    end

    def run()
      @offsets.each do |offset|
        dlist(offset)
      end
      File.binwrite("test.zobj", @buffer)
      Object.new(@buffer, @offsets_new)
    end

    def dlist(offset)
      size = 0
      loop do
        data = @object[offset + size, 8].unpack("L>2")
        size += 8
        break if data[0] == 0xdf000000
      end
      list = @object[offset, size]
      patch_list(list)
      @offsets_new << (@buffer.size | 0x06000000)
      @buffer << list
      while @buffer.size % 0x10 != 0
        @buffer << "\x00"
      end
    end

    def patch_list(list)
      count = list.size / 8
      count.times do |i|
        data = list[i * 8, 8].unpack("L>2")
        op = data[0] >> 24
        if op == 0x01
          # G_VTX
          count = (data[0] >> 12) & 0xff
          list[i * 8 + 4,4] = [copy_vertices(data[1], count)].pack("L>")
        elsif op == 0xfd
          data2 = list[(i + 1) * 8, 8].unpack("L>2")
          op2 = data2[0] >> 24
          if op2 == 0xf5
            # Texture
            fmt = (data[0] >> 16) & 0xff
            bpp = 0
            case fmt
            when 0x50
              bpp = 4
            when 0x10
              bpp = 16
            end
            data3 = list[(i + 6) * 8, 8].unpack("L>2")
            w = (((data3[1] >> 12) & 0xfff) / 4) + 1
            h = (((data3[1] >>  0) & 0xfff) / 4) + 1
            list[i * 8 + 4,4] = [copy_data(data[1], (h * w * bpp) / 8)].pack("L>")
          elsif op2 == 0xe8
            # LuT
            list[i * 8 + 4,4] = [copy_data(data[1], 32)].pack("L>")
          end
        end
      end
    end

    def copy_vertices(addr, count)
      copy_data(addr, count * 0x10)
    end

    def copy_data(addr, size)
      if @shifts[addr]
        @shifts[addr]
      else
        new_addr = @buffer.size | 0x06000000
        @buffer << @object[(addr & 0xffffff), size]
        @shifts[addr] = new_addr
        new_addr
      end
    end

    def self.run(*args)
      self.new(*args).run
    end
  end
end
