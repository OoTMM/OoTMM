require 'combo/logic/solver'
require 'combo/logic/world_builder'

module Combo
  module Logic
    def self.run
      world = WorldBuilder.run
      solver = Solver.new(world)
      solver.run
    end
  end
end
