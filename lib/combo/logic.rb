require 'combo/logic/solver'
require 'combo/logic/world_graph_builder'

module Combo
  module Logic
    def self.run
      graph = WorldGraphBuilder.run
      solver = Solver.new(graph)
      solver.run
    end
  end
end
