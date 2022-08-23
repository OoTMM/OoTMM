require 'warning'
Warning.ignore(//, /.*ruby-elf.*/)
require 'elf'
require 'combo/common'

class Combo::Custom
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
    custom_object_files = Dir.glob(File.join(Combo::PATH_BUILD, 'obj/objects/**/*.o'))
    custom_object_files.each do |file|
      add_custom_object(file)
    end
    write_custom()
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

        exports_addr.size.times do |i|
          addr = exports_addr[i]
          n = exports_name[i]
          v = ["CUSTOM_OBJECT", name, n].join('_')
          add_define(v, addr)
        end

        add_define(["CUSTOM_OBJECT_ID", name].join('_'), 0x2000 | @objects.size)
        append_vrom_object(data)
      end
    end
  end

  def append_vrom_object(data)
    id = @objects.size | 0x2000
    @objects.push(append_vrom_dma(data))
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
