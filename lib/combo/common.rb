require 'json'

module Combo
  ROOT = File.expand_path(File.join(__dir__, '..', '..'))
  GAMES = [:oot, :mm]

  # Paths
  PATH_ROMS = File.join(ROOT, 'roms')
  PATH_BUILD = File.join(ROOT, 'build')
  PATH_DATA = File.join(ROOT, 'data')
  PATH_DIST = File.join(ROOT, 'dist')

  # Data
  DATA_FILES = GAMES.map { |game| [game, JSON.parse(File.read(File.join(Combo::PATH_DATA, "#{game}", "files.json"))).freeze] }.to_h.freeze

  METADATA = {
    custom_addr: 0x03f80000,
    oot: {
      sha256: ["d38fd6ff19daf05320b3c23f9adc119b017408686e12aaf32f313a7582b020af", "c916ab315fbe82a22169bff13d6b866e9fddc907461eb6b0a227b82acdf5b506"],
      dma_addr: 0x7430,
      dma_count: 1510,
      payload_addr: 0x03fa0000,
      actors_ovl_addr: 0xb5e490,
      actors_ovl_count: 471,
    },
    mm: {
      sha256: ["efb1365b3ae362604514c0f9a1a2d11f5dc8688ba5be660a37debf5e3be43f2b"],
      dma_addr: 0x1a500,
      dma_count: 1552,
      payload_addr: 0x03fc0000,
      actors_ovl_addr: 0xc45510,
      actors_ovl_count: 690,
    },
  }
end
