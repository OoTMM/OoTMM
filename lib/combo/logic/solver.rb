require 'set'
require 'combo/logic/state'

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
      @checks_reachable = Set.new
      @checks_fixed = Set.new
      @checks_unfixed = Set.new(@graph.checks)
      @checks_unreachable = Set.new(graph.checks)
      @checks_unreachable_per_location = make_hashset(graph.checks.map {|c| [c.location, c]})
      @checks_per_location = make_hashset(graph.checks.map {|c| [c.location, c]})
      @links_per_location = make_hashset(graph.links.map {|l| [l.from, l]})
      @links_per_to_location = make_hashset(graph.links.map {|l| [l.to, l]})
    end

    def make_hashset(data)
      h = {}
      data.each do |pair|
        k = pair[0]
        v = pair[1]
        h[k] ||= Set.new
        h[k] << v
      end
      h
    end

    def mark_reachable(age, location)
      @state.reachable_locations[age].add(location)
      if location == :TIME_TRAVEL
        @state.reachable_locations[:adult].add(:TIME_TRAVEL)
      end
    end

    def eval_cond(age, cond)
      return true if cond.nil?
      cond.eval(age, @state)
    end

    def run
      # Mark link's house as reachable
      mark_reachable(:child, :KF_LinkHouse)

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

    def fix_goals()
      checks = @graph.checks.select {|c| goal_item?(c)}
      shuffle(checks)
      @checks_fixed.merge(checks)
      @checks_unfixed.subtract(checks)
    end

    def fix_dungeon(dungeon)
      checks = dungeon.map{|x| @checks_per_location[x]}.reject(&:nil?).reduce(&:|)
      checks.reject! {|c| @checks_fixed.include?(c) }
      shuffle(checks)
      checks.each do |check|
        if dungeon_item?(check)
          @checks_fixed.add(check)
          @checks_unfixed.delete(check)
        end
      end
    end

    def fix_required()
      loop do
        # Propagate and check if everything is reachable
        propagate_locations()
        propagate_checks()

        #p @checks_unreachable
        break if @checks_unreachable.empty?

        # Everything is not reachable, we need to add a progression item
        fix_new_progression()
      end
    end

    def propagate_locations()
      loop do
        changed = false
        [:child, :adult].each do |age|
          reachable = @state.reachable_locations[age]
          links = reachable.map{|loc| @links_per_location[loc].to_a}.flatten
          links.reject! {|l| reachable.include?(l.to) }
          links.select! {|l| eval_cond(age, l.cond) }
          unless links.empty?
            changed = true
            links.each {|l| mark_reachable(age, l.to) }
          end
        end
        break unless changed
      end
    end

    def propagate_checks()
      [:child, :adult].each do |age|
        locs = @state.reachable_locations[age]
        locs.each do |loc|
          unreachable_checks = @checks_unreachable_per_location[loc] || []
          unreachable_checks.each do |check|
            if eval_cond(age, check.cond)
              @checks_reachable.add(check)
              @checks_unreachable.delete(check)
              unreachable_checks.delete(check)
            end
          end
        end
      end
    end

    def fix_new_progression()
      missing = []

      [:child, :adult].each do |age|
        # Collect missing items for locations
        cond_locs = @graph.links.reject{|l| @state.reachable_locations[age].include?(l.to) }.map{|l| l.cond}.reject(&:nil?)
        cond_checks = @checks_unreachable.map{|c| c.cond}.reject(&:nil?)
        missing << (cond_locs + cond_checks).map{|c| c.missing(age, @state)}.reduce(&:|)
      end
      missing = missing.reduce(&:|)
      check_src = @checks_unfixed.select{|c| missing.include?(c.content) }.to_a.sample
      check_dst = (@checks_unfixed & @checks_reachable).to_a.sample
      item = check_src.content

      swap(check_dst, check_src)
      @checks_fixed.add(check_dst)
      @checks_unfixed.delete(check_dst)
      add_item(item)
    end

    def fix_all()
      shuffle(@checks_unfixed)
    end

    def add_item(item)
      case item
      when :BOMBCHUS_5, :BOMBCHUS_10, :BOMBCHUS_20
        item = :BOMBCHU
      end
      @state.items[item] = true
    end

    def dungeon_item?(check)
      [:MAP, :COMPASS, :KEY_SMALL, :KEY_BOSS].include?(check.content)
    end

    def goal_item?(check)
      %i[
        STONE_EMERALD
        STONE_RUBY
        STONE_SAPPHIRE
        MEDALLION_FOREST
        MEDALLION_FIRE
        MEDALLION_WATER
        MEDALLION_SPIRIT
        MEDALLION_SHADOW
        MEDALLION_LIGHT
      ].include?(check.content)
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
        [check.type, check.scene_id, check.id, check.content]
      end
    end

    def export_spoiler
      data = []
      @graph.checks.each do |check|
        data << "%s: %s\n" % [check.desc, check.content]
      end
      data.sort!
      data.join()
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
