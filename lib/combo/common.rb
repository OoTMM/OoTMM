module Combo
  ROOT = File.expand_path(File.join(__dir__, '..', '..'))
  GAMES = [:oot, :mm]
  ROM_BASE = File.join(ROOT, 'build', 'base.z64')
  ROM_OOTMM = File.join(ROOT, 'build', 'OoTMM.z64')

  # Paths
  PATH_ROMS = File.join(ROOT, 'roms')
  PATH_BUILD = File.join(ROOT, 'build')
  PATH_EXTRACT = File.join(PATH_BUILD, 'extracted')
  PATH_DATA = File.join(ROOT, 'data')

  # Stamps
  STAMP_EXTRACT = File.join(PATH_BUILD, 'extracted.stamp')

  # Data
  DATA_FILES = GAMES.map { |game| [game, JSON.parse(File.read(File.join(Combo::PATH_DATA, "#{game}.json"))).freeze] }.to_h.freeze
end
