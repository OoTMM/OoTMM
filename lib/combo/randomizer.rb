require 'json'
require 'combo/common'
require 'combo/logic'

class Combo::Randomizer
  def initialize
    gi_header = File.read(File.join(Combo::ROOT, 'include', 'combo', 'gi.h'))
    @gi = gi_header.scan(/^#define\s+GI_([A-Z0-9_]+)\s+(.+)$/).map{ |m|
      [m[0].to_sym, Integer(m[1])]
    }.to_h.freeze
  end

  def run()
    rando = Combo::Logic.run

    data = ("\xff" * 0x20000).force_encoding('ASCII-8BIT')

    oot = overrides(:oot, rando)
    mm = overrides(:mm, rando)
    data[0x1000, oot.length] = oot
    data[0x2000, mm.length] = mm

    write_spoiler(rando)

    data
  end

  def random_block(game)
    chests = JSON.parse(File.read(File.join(Combo::PATH_DATA, game.to_s, 'chests.json'))).freeze
    data = ("\xff\xff\xff\xff" * 256).force_encoding('ASCII-8BIT')
    chests.keys.each_with_index do |key, i|
      #value = Random.random_number(0x01..0x7b)
      #value = 4
      #value = 0x178
      #value = 0x189
      value = 0x29
      if game == :mm
        value = 0x12d
      end
      key = key.to_i(16)
      raw = [key, value].pack('S>2')
      data[i * 4,4] = raw
    end
    data
  end

  def foreign_item?(game, item)
    prefix = game.to_s.upcase
    !item.to_s.start_with?(prefix)
  end

  def get_item(game, item)
    case item
    when :OOT_SWORD
      item = :OOT_SWORD_KOKIRI
    when :OOT_SHIELD
      item = :OOT_DEKU_SHIELD
    when :OOT_OCARINA
      item = :OOT_OCARINA_FAIRY
    when :OOT_STRENGTH
      item = :OOT_GORON_BRACELET
    when :OOT_SCALE
      item = :OOT_SILVER_SCALE
    when :OOT_HAMMER
      item = :OOT_MEGATON_HAMMER
    when :OOT_BOW
      item = :OOT_FAIRY_BOW
    when :MM_SWORD
      item = :MM_KOKIRI_SWORD
    when :MM_SHIELD
      item = :MM_HYLIAN_SHIELD
    end
    x = @gi[item]
    if x.nil?
      raise "Unknown GI item: #{item}"
    end
    if foreign_item?(game, item)
      x |= 0x100
    end
    x
  end

  def overrides(game, rando)
    data = []
    rando.data.each do |override|
      next if override[0] != game
      type = override[1]
      scene_id = override[2]
      id = override[3]
      case type
      when :special
        scene_id = 0xf0
      when :npc
        scene_id = 0xf1
      when :collectible
        id |= 0x20
      end
      content = override[4]
      key = (scene_id << 8) | id
      gi = get_item(game, content)
      data << [key, gi].pack('S>2')
    end
    data.join('')
  end

  def write_spoiler(rando)
    File.write(File.join(Combo::ROOT, 'dist', 'spoiler.txt'), rando.spoiler)
  end

  def self.run
    rnd = self.new
    rnd.run
  end
end
