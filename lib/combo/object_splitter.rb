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
    end

    def run()
      @offsets.each do |offset|
        dlist(offset)
      end
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
    end

    def patch_list(list)
      count = list.size / 8
      count.times do |i|
        data = list[i * 8, 8].unpack("L>2")
        op = data[0] >> 24
        if op == 0x01
          # G_VTX
          count = (data[0] >> 12) & 0xff
          data[1] = copy_vertices(data[1], count)
        elsif op == 0xfd
          # Texture
          data[1] = copy_texture(data[1])
        end
      end
    end

    def copy_texture(addr)
      0x06000000
    end

    def copy_vertices(addr, count)
      copy_data(addr, count * 0x10)
    end

    def copy_data(addr, size)
      new_addr = @buffer.size | 0x06000000
      @buffer << @object[(addr & 0xffffff), size]
      new_addr
    end

    def self.run(*args)
      self.new(*args).run
    end
  end
end
