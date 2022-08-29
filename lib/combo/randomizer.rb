require 'json'
require 'combo/common'
require 'combo/logic'

class Combo::Randomizer
  def initialize
    gi_header = File.read(File.join(Combo::ROOT, 'include', 'combo', 'gi.h'))
    @gi = gi_header.scan(/^#define\s+GI_([A-Z0-9_]+)\s+(.+)$/).map{ |m|
      [m[0], Integer(m[1])]
    }.to_h.freeze
  end

  def run()
    rando = Combo::Logic.run

    data = ("\xff" * 0x20000).force_encoding('ASCII-8BIT')

    oot = overrides(rando)
    mm = random_block(:mm)
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

  def get_item(item)
    case item
    when :SWORD
      item = :SWORD_KOKIRI
    when :SHIELD
      item = :DEKU_SHIELD
    when :OCARINA
      item = :OCARINA_FAIRY
    when :STRENGTH
      item = :GORON_BRACELET
    end
    x = @gi["OOT_#{item}"]
    if x.nil?
      raise "Unknown GI item: #{item}"
    end
    x
  end

  def overrides(rando)
    data = []
    rando.data.each do |override|
      type = override[0]
      scene_id = override[1]
      id = override[2]
      case type
      when :special
        scene_id = 0xf0
      when :npc
        scene_id = 0xf1
      when :collectible
        id |= 0x20
      end
      content = override[3]
      key = (scene_id << 8) | id
      gi = get_item(content)
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
