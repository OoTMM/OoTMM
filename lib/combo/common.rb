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
    custom_addr: 0x03f80000,
    oot: {
      dma_addr: 0x7430,
      dma_count: 1510,
      payload_addr: 0x03fa0000,
      actors_ovl_addr: 0xb5e490,
      actors_ovl_count: 471,
    },
    mm: {
      dma_addr: 0x1a500,
      dma_count: 1552,
      payload_addr: 0x03fc0000,
      actors_ovl_addr: 0xc45510,
      actors_ovl_count: 690,
    },
  }
end
