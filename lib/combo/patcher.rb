require 'combo/file_buffer'
require 'combo/assembler'

REGS = [
  :zero, :at,
  :v0, :v1,
  :a0, :a1, :a2, :a3,
  :t0, :t1, :t2, :t3, :t4, :t5, :t6, :t7,
  :s0, :s1, :s2, :s3, :s4, :s5, :s6, :s7,
  :t8, :t9,
  :k0, :k1,
  :gp, :sp, :fp, :ra
].map.with_index{|x, i| [x, i]}.to_h.freeze

class Combo::Patcher
  def self.run()
    patcher = self.new
    patcher.run
  end

  def run
    puts "Loading the base ROM"
    @rom.load(0, File.binread(Combo::ROM_BASE))

    puts "Adding OoT loader"
    @rom.load(0x7200, File.binread(File.join(Combo::PATH_BUILD, 'loader_oot')))

    puts "Adding OoT payload"
    @rom.load(0x03fc0000, File.binread(File.join(Combo::PATH_BUILD, 'payload_oot')))

    puts "Adding patches"
    patch_jump(oot_code(0x800cd3c0), 0x80006600) # Hook InitHeap -> load OoT payload
    @rom.write32(oot_code(0x800f13e8), @sym_oot["comboPatchSceneCtor"])
    patch_lui(mm_ref("boot", 0x80093900 - 0x80080060), :t7, 0xb400) # Rebase MM

    puts "Fixing metadata"
    @rom.load(0x20, "OOT+MM COMBO       ")

    puts "Fixing the checksum"
    @rom.finalize

    @rom.close
  end

  private
  def initialize
    @rom = Combo::FileBuffer.new(Combo::ROM_OOTMM, true)
    @rom.init(128 * 1024 * 1024)

    @sym_oot = Combo::Util.parse_syms(File.join(Combo::PATH_BUILD, 'payload_oot.sym')).freeze

    @offets_oot = JSON.parse(File.read(File.join(Combo::PATH_BUILD, 'OoT_offsets.json'))).freeze
  end

  def patch_jump(offset, target)
    @rom.write32(offset, ((002 << 26) | ((target & 0x3ffffff) >> 2)))
  end

  def patch_lui(offset, reg, value)
    r = REGS[reg]
    @rom.write32(offset, (017 << 26) | ((r & 0x1f) << 16) | (value & 0xffff))
  end

  def oot_ref(file, offset)
    @offets_oot[file] + offset
  end

  def mm_ref(file, offset)
    @offets_oot[file] + offset + 0x04000000
  end

  def oot_code(vaddr)
    oot_ref("code", vaddr - 0x800110A0)
  end
end
