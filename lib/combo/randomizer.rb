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
    # DEBUG
    overrides = Combo::Logic.run

    data = ("\xff" * 0x20000).force_encoding('ASCII-8BIT')

    oot_chests = chest_overrides(overrides)
    mm_chests = random_block(:mm)
    data[0x1000, oot_chests.length] = oot_chests
    data[0x2000, mm_chests.length] = mm_chests

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
    end
    x = @gi["OOT_#{item}"]
    if x.nil?
      raise "Unknown GI item: #{item}"
    end
    x
  end

  def chest_overrides(overrides)
    data = []
    overrides.each do |override|
      type = override[0]
      next if type != :chest
      scene_id = override[1]
      chest_id = override[2]
      content = override[3]
      key = (scene_id << 8) | chest_id
      gi = get_item(content)
      data << [key, gi].pack('S>2')
    end
    data.join('')
  end

  def self.run
    rnd = self.new
    rnd.run
  end
end
