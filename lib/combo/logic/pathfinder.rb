require 'combo/logic/util'

module Combo::Logic
  class Pathfinder
    attr_reader :reachable, :unreachable

    def initialize(graph, state)
      @graph = graph
      @state = state
      @reachable = Set.new
      @unreachable = Set.new(graph.checks)
      @links_per_location = Util.make_multihash_set(graph.links.map {|l| [l.from, l]})
      @checks_unreachable_per_location = Util.make_multihash_set(graph.checks.map {|c| [c.location, c]})
      @dungeon_per_loc = graph.dungeons.values.map {|d| d.locations.map {|l| [l, d.name]}}.flatten(1).to_h
      @required_keys = graph.dungeons.values.map {|d| d.required_keys}.reduce({}, &:merge)
    end

    def mark_reachable(age, location)
      @state.reachable_locations[age].add(location)
      if location == :TIME_TRAVEL
        @state.reachable_locations[:adult].add(:TIME_TRAVEL)
      end
    end

    def propagate
      propagate_locations()
      propagate_checks()
    end

    def add_item(item, loc)
      dungeon = @dungeon_per_loc[loc]
      if Util.small_key?(item)
        @state.keys[dungeon] ||= 0
        @state.keys[dungeon] += 1
      elsif Util.boss_key?(item)
        @state.boss_keys.add(dungeon)
      else
        @state.items[item] ||= 0
        @state.items[item] += 1
      end
    end

    private
    def propagate_locations()
      loop do
        changed = false
        [:child, :adult].each do |age|
          reachable = @state.reachable_locations[age]
          links = reachable.map{|loc| @links_per_location[loc].to_a}.flatten
          links.reject! {|l| reachable.include?(l.to) }
          links.select! {|l| l.cond.nil? || l.cond.eval(age, @state) }
          links.each do |l|
            can_traverse = true
            if l.locked
              required_keys = @required_keys[l.to]
              current_keys = @state.keys[@dungeon_per_loc[l.from]] || 0
              if required_keys > current_keys
                can_traverse = false
              end
            end
            if l.locked_boss
              can_traverse = false if !@state.boss_keys.include?(@dungeon_per_loc[l.from])
            end
            if can_traverse
              mark_reachable(age, l.to)
              changed = true
            end
          end
        end
        break unless changed
      end
    end

    def propagate_checks()
      new_checks = []
      [:child, :adult].each do |age|
        locs = @state.reachable_locations[age]
        locs.each do |loc|
          unreachable_checks = @checks_unreachable_per_location[loc] || []
          unreachable_checks.each do |check|
            if check.cond.nil? || check.cond.eval(age, @state)
              new_checks << check
              @reachable.add(check)
              @unreachable.delete(check)
              unreachable_checks.delete(check)
            end
          end
        end
      end
      new_checks
    end
  end
end
