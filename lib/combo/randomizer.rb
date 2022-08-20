require 'json'
require 'combo/common'

module Combo::Randomizer
  def self.run()
    data = ("\x00" * 0x20000).force_encoding('ASCII-8BIT')

    oot_chests = random_block(:oot)
    mm_chests = random_block(:mm)
    data[0x1000, oot_chests.length] = oot_chests
    data[0x2000, mm_chests.length] = mm_chests

    data
  end

  def self.random_block(game)
    chests = JSON.parse(File.read(File.join(Combo::PATH_DATA, game.to_s, 'chests.json'))).freeze
    data = ("\xff\xff\xff\xff" * 256).force_encoding('ASCII-8BIT')
    chests.keys.each_with_index do |key, i|
      #value = Random.random_number(0x01..0x7b)
      #value = 4
      #value = 0x178
      #value = 0x189
      value = 0x3b
      if game == :mm
        value = 0x12d
      end
      key = key.to_i(16)
      raw = [key, value].pack('S>2')
      data[i * 4,4] = raw
    end
    data
  end
end
