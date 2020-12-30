require 'combo/common'

class Combo::Assembler
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

  def self.instr(mnemo, type, op, descr)
    define_method(mnemo) do |*args|
      encode(type, op, descr, args)
    end
  end

  instr :j,    :jtype,   002, [:off]
  instr :jal,  :jtype,   003, [:off]
  instr :bne,  :itype,   005, [:rs, :rt, :off]
  instr :andi, :itype,   014, [:rt, :rs, :imm]
  instr :ori,  :itype,   015, [:rt, :rs, :imm]
  instr :xori, :itype,   016, [:rt, :rs, :imm]
  instr :lui,  :itype,   017, [:rt, :imm]
  instr :lw,   :itype,   043, [:rt, :imm, :rs]
  instr :sw,   :itype,   053, [:rt, :imm, :rs]

  instr :sll,  :special, 000, [:rd, :rt, :sa]
  instr :jr,   :special, 010, [:rs]
  instr :jalr, :special, 011, [:rd, :rs]

  # Pseudo-instructions
  def mov(dst, src)
    sll dst, src, 0
  end

  def la(reg, addr)
    li reg, addr
  end

  def li(reg, imm)
    lui reg, (imm >> 16)
    ori reg, reg, (imm & 0xffff)
  end

  def nop
    sll :zero, :zero, 0
  end

  def initialize(file, addr)
    @file = file
    @addr = addr
  end

  private
  def encode(type, op, descr, args)
    rs = 0
    rt = 0
    rd = 0
    sa = 0
    imm = 0

    descr.each do |t|
      v = args.shift
      case t
      when :imm
        imm = v
      when :off
        imm = v / 4
      when :rs
        rs = REGS[v]
      when :rt
        rt = REGS[v]
      when :rd
        rd = REGS[v]
      when :sa
        sa = v
      end
    end

    payload = case type
    when :itype
      (op << 26) | ((rs & 0x1f) << 21) | ((rt & 0x1f) << 16) | (imm & 0xffff)
    when :jtype
      (op << 26) | (imm & 0x3ffffff)
    when :special
      (op) | ((sa & 0x1f) << 6) | ((rd & 0x1f) << 11) | ((rt & 0x1f) << 16) | ((rs & 0x1f) << 21)
    end

    @file.write32_inplace(payload)
    @addr += 4
  end
end
