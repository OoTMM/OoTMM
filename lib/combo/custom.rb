require 'warning'
Warning.ignore(//, /.*ruby-elf.*/)
require 'elf'
require 'combo/common'
require 'combo/object_splitter'

module Combo
  class Custom
    def self.generate()
      self.new.run()
    end

    def initialize()
      @buffer = StringIO.new
      @dma = []
      @objects = []
      @defines = []
    end

    def run()
      add_split_object("GI_REMAINS_ODOLWA",   :mm, "objects/object_bsmask", [0x0690])
      add_split_object("GI_REMAINS_GOHT",     :mm, "objects/object_bsmask", [0x3ad0])
      add_split_object("GI_REMAINS_GYORG",    :mm, "objects/object_bsmask", [0x1d80])
      add_split_object("GI_REMAINS_TWINMOLD", :mm, "objects/object_bsmask", [0x5020])

      custom_object_files = Dir.glob(File.join(Combo::PATH_BUILD, 'obj/objects/**/*.o'))
      custom_object_files.each do |file|
        add_custom_object(file)
      end
      write_custom()
    end

    def add_split_object(name, game, filename, offsets)
      dma_index = Combo::DATA_FILES[game].index(filename)
      object = nil
      File.open(File.join(Combo::PATH_BUILD, 'roms', "#{game}_decompressed.z64"), "rb") do |f|
        f.seek(Combo::METADATA[game][:dma_addr])
        dma_data = Combo::DmaData.new(f.read(Combo::METADATA[game][:dma_count] * 10))
        entry = dma_data.entry(dma_index)
        f.seek(entry.vstart)
        object = f.read(entry.vlen)
      end
      object_new = ObjectSplitter.run(object, offsets)
      exports = object_new.offsets.map.with_index{|addr, i| [i, addr]}.to_h
      add_object(name, object_new.data, exports)
    end

    def add_custom_object(file)
      File.open(file, 'rb') do |f|
        Elf::File.open(file) do |elf|
          cobject_data = elf.load_section(".cobject.data")
          cobject_name = elf.load_section(".cobject.name")

          f.seek(cobject_data.offset)
          data = f.read(cobject_data.size)

          f.seek(cobject_name.offset)
          name = f.read(cobject_name.size).unpack('Z*').first

          cobject_exports_addr = elf.load_section(".cobject.exports_addr")
          f.seek(cobject_exports_addr.offset)
          exports_addr = f.read(cobject_exports_addr.size).unpack('L>*')

          cobject_exports_name = elf.load_section(".cobject.exports_name")
          f.seek(cobject_exports_name.offset)
          exports_name = f.read(cobject_exports_name.size).split(0.chr)

          exports = {}
          exports_addr.size.times do |i|
            addr = exports_addr[i]
            n = exports_name[i]
            exports[n] = addr
          end

          add_object(name, data, exports)
        end
      end
    end

    def add_object(name, data, exports)
      exports.each do |k, v|
        defname = ["CUSTOM_OBJECT", name, k].join('_')
        add_define(defname, v)
      end
      id = append_vrom_object(data)
      add_define(["CUSTOM_OBJECT_ID", name].join('_'), id)
    end

    def append_vrom_object(data)
      id = @objects.size | 0x2000
      @objects.push(append_vrom_dma(data))
      id
    end

    def append_vrom_dma(data)
      paddr_start = Combo::METADATA[:custom_addr] + @buffer.size
      paddr_end = paddr_start + data.size
      vaddr_start = append_vrom(data)
      vaddr_end = vaddr_start + data.size
      @dma.push([vaddr_start, vaddr_end, paddr_start, 0])
      [vaddr_start, vaddr_end]
    end

    def append_vrom(data)
      off = @buffer.size
      @buffer.write(data)
      return off | 0x08000000
    end

    def write_custom()
      custom_addr_dma = Combo::METADATA[:custom_addr] + @buffer.size
      dma_data = @dma.map { |dma| dma.pack('L>4') }.join('')
      @buffer.write(dma_data)

      custom_addr_objects = Combo::METADATA[:custom_addr] + @buffer.size
      objects_data = @objects.map { |object| object.pack('L>2') }.join('')
      @buffer.write(objects_data)

      add_define("CUSTOM_DMA_ADDR", custom_addr_dma)
      add_define("CUSTOM_DMA_SIZE", @dma.size)
      add_define("CUSTOM_OBJECTS_ADDR", custom_addr_objects)
      add_define("CUSTOM_OBJECTS_SIZE", @objects.size)

      File.open(File.join(Combo::PATH_BUILD, 'include', 'combo', 'custom.h'), "w") do |f|
        f.puts("#ifndef COMBO_CUSTOM_H")
        f.puts("#define COMBO_CUSTOM_H")
        f.puts("")
        f.puts(@defines.join("\n"))
        f.puts("")
        f.puts("#endif")
      end

      File.binwrite(File.join(Combo::PATH_BUILD, 'custom.bin'), @buffer.string)
    end

    def add_define(name, value)
      @defines.push("#define %s 0x%08x" % [name, value])
    end
  end
end
