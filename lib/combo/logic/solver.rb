module Combo::Logic
  class Solver
    def initialize(graph)
      @graph = graph
      @items = {}
    end

    def run
      # We need to make sure that the small keys, compass map and boss keys stay in the same dungeon.
      @graph.dungeons.each {|d| fix_dungeon(d)}

      # TODO: Add real logic
      c = @graph.checks.select {|c| c.fixed == false}
      shuffle(c)

      export()
    end

    def fix_dungeon(dungeon)
      checks = dungeon.checks
      shuffle(checks)
      checks.each do |check|
        if local_to_dungeon?(check)
          check.fixed = true
        end
      end
    end

    def local_to_dungeon?(check)
      [:MAP, :COMPASS, :KEY_SMALL, :KEY_BOSS].include?(check.content)
    end

    def swap(c1, c2)
      tmp = c1.content
      c1.content = c2.content
      c2.content = tmp
    end

    def shuffle(checks)
      contents = checks.map {|c| c.content}
      contents.shuffle!
      checks.each_with_index do |c, i|
        c.content = contents[i]
      end
    end

    def export
      @graph.checks.map do |check|
        [check.type, check.room.scene_id, check.id, check.content]
      end
    end

    def self.run
      logic = self.new
      logic.run
    end
  end
end
