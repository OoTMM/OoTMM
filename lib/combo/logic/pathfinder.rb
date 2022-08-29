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

    def add_item(item)
      case item
      when :BOMBCHUS_5, :BOMBCHUS_10, :BOMBCHUS_20
        item = :BOMBCHU
      end
      @state.items[item] = true
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
          unless links.empty?
            changed = true
            links.each {|l| mark_reachable(age, l.to) }
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
