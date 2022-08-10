require 'json'

module Combo
  ROOT = File.expand_path(File.join(__dir__, '..', '..'))
  GAMES = [:oot, :mm]
  ROM_OOTMM = File.join(ROOT, 'build', 'OoTMM.z64')

  # Paths
  PATH_ROMS = File.join(ROOT, 'roms')
  PATH_BUILD = File.join(ROOT, 'build')
  PATH_DATA = File.join(ROOT, 'data')

  # Data
  DATA_FILES = GAMES.map { |game| [game, JSON.parse(File.read(File.join(Combo::PATH_DATA, "#{game}.json"))).freeze] }.to_h.freeze

  METADATA = {
    oot: {
      dma_addr: 0x7430,
      dma_count: 1510,
      payload_addr: 0x03fc0000,
    },
    mm: {
      dma_addr: 0x1a500,
      dma_count: 1552,
      payload_addr: 0x03fe0000,
    },
  }
end
