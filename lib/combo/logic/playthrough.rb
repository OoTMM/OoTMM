require 'combo/logic/state'
require 'combo/logic/pathfinder'

module Combo::Logic
  class Playthrough
    def initialize(graph)
      @graph = graph
      @state = State.new
      @pathfinder = Pathfinder.new(graph, @state)
      @pathfinder.mark_reachable(:child, :KF_LinkHouse)
      @spheres = []
    end

    def run
      loop do
        checks = @pathfinder.propagate()
        sphere_checks = checks.reject {|c| junk?(c.content) }
        checks.each {|c| @pathfinder.add_item(c.content) }
        unless sphere_checks.empty?
          @spheres << sphere_checks
        end
        break if @pathfinder.unreachable.empty?
      end
      export()
    end

    private
    def junk?(item)
      %i[
        RECOVERY_HEART
        RUPEE_GREEN
        RUPEE_BLUE
        RUPEE_RED
        RUPEE_PURPLE
        HEART_PIECE
        HEART_CONTAINER
        HEART_CONTAINER2
        MAP
        COMPASS
        BOMBS_5
        BOMBS_10
      ].include?(item)
    end

    def export()
      data = []
      @spheres.each.with_index do |sphere, i|
        data << "Sphere #{i}"
        data << sphere.map{|x| "  %s: %s" % [x.desc, x.content.to_s]}.sort.join("\n")
        data << ""
      end
      data.join("\n")
    end

    def self.run(graph)
      self.new(graph).run()
    end
  end
end
