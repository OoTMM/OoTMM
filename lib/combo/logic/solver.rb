require 'set'
require 'combo/logic/state'
require 'combo/logic/pathfinder'
require 'combo/logic/playthrough'
require 'combo/logic/util'

module Combo::Logic
  class Solver
    class Output
      attr_reader :data
      attr_reader :spoiler

      def initialize(data, spoiler)
        @data = data
        @spoiler = spoiler
      end
    end

    def initialize(graph)
      @graph = graph
      @state = State.new
      @pathfinder = Pathfinder.new(@graph, @state)
      @checks_fixed = Set.new
      @checks_unfixed = Set.new(@graph.checks)
      @checks_per_location = Util.make_multihash_set(graph.checks.map {|c| [c.location, c]})
    end

    def eval_cond(age, cond)
      return true if cond.nil?
      cond.eval(age, @state)
    end

    def run
      # Add some missing items to the pool
      # TODO: Change how this is done
      add_pool(%i[MM_MASK_DEKU MM_OCARINA_OF_TIME MM_SHIELD MM_SWORD MM_SONG_TIME MM_SONG_HEALING])

      # Mark link's house as reachable
      @pathfinder.mark_reachable(:child, :KF_LinkHouse)

      # Fix the goal items
      fix_goals()

      # We need to make sure that the small keys, compass map and boss keys stay in the same dungeon.
      @graph.dungeons.each {|d| fix_dungeon(d)}

      # Fix required items
      fix_required()

      # Shuffle remaining items
      fix_all()

      export()
    end

    def add_pool(items)
      c = @checks_unfixed.select{|x| Util.junk_item?(x.content)}.to_a.sample(items.size)
      c.each_with_index do |check, i|
        check.content = items[i]
      end
    end

    def fix_goals()
      checks = @graph.checks.select {|c| Util.goal_item?(c.content)}
      shuffle(checks)
      @checks_fixed.merge(checks)
      @checks_unfixed.subtract(checks)
    end

    def fix_dungeon(dungeon)
      checks = dungeon.map{|x| @checks_per_location[x]}.reject(&:nil?).reduce(&:|)
      checks.reject! {|c| @checks_fixed.include?(c) }
      shuffle(checks)
      checks.each do |check|
        if Util.dungeon_item?(check.content)
          @checks_fixed.add(check)
          @checks_unfixed.delete(check)
        end
      end
    end

    def fix_required()
      loop do
        # Propagate and check if everything is reachable
        loop do
          new_checks = @pathfinder.propagate()
          new_checks_fixed = false
          new_checks.each do |c|
            if @checks_fixed.include?(c)
              @pathfinder.add_item(c.content)
              new_checks_fixed = true
            end
          end
          break unless new_checks_fixed
        end

        #p @checks_unreachable
        break if @pathfinder.unreachable.empty?

        # Everything is not reachable, we need to add a progression item
        fix_new_progression()
      end
    end

    def fix_new_progression()
      missing = []

      [:child, :adult].each do |age|
        # Collect missing items for locations
        cond_locs = @graph.links.reject{|l| @state.reachable_locations[age].include?(l.to) }.map{|l| l.cond}.reject(&:nil?)
        cond_checks = @pathfinder.unreachable.map{|c| c.cond}.reject(&:nil?)
        missing << (cond_locs + cond_checks).map{|c| c.missing(age, @state)}.reduce(&:|)
      end
      missing = missing.reduce(&:|)
      check_src = @checks_unfixed.select{|c| missing.include?(c.content) }.to_a.sample
      check_dst = (@checks_unfixed & @pathfinder.reachable).to_a.sample
      item = check_src.content

      swap(check_dst, check_src)
      @checks_fixed.add(check_dst)
      @checks_unfixed.delete(check_dst)
      @pathfinder.add_item(item)
    end

    def fix_all()
      shuffle(@checks_unfixed)
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

    def export_data
      @graph.checks.map do |check|
        [check.game, check.type, check.scene_id, check.id, check.content]
      end
    end

    def export_spoiler
      data = []
      @graph.checks.each do |check|
        data << "%s: %s\n" % [check.desc, check.content]
      end
      data.sort!
      log = data.join()
      playthrough = Playthrough.new(@graph)
      log += "\n\n" + playthrough.run()
      log
    end

    def export
      data = export_data()
      spoiler = export_spoiler()
      Output.new(data, spoiler)
    end

    def self.run
      logic = self.new
      logic.run
    end
  end
end
