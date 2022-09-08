require 'combo/logic/world_parser'
require 'combo/logic/world'

module Combo::Logic
  class WorldBuilder
    def run()
      parse = WorldParser.run
      @checks = parse[:checks]
      @links = parse[:links]
      @checks_per_location = @checks.group_by{|x| x.location}
      @links_per_to = @links.group_by{|x| x.to}
      @dungeons = parse[:dungeons].map{|k, v| [k, build_dungeon(k, v)]}.to_h


      World.new(@checks, @links, @dungeons)
    end

    def build_dungeon(name, locs)
      checks = locs.map{|x| @checks_per_location[x]}.compact.flatten
      required_keys = {}
      entrances = Set.new(locs.map{|x| @links_per_to[x]}.flatten.compact.select{|x| !locs.include?(x.from)}.map{|x| x.to})
      internal_links = @links.select{|x| locs.include?(x.from) && locs.include?(x.to)}.group_by(&:from)
      required_keys = dungeon_reach(entrances, internal_links)
      before_boss = dungeon_reach_boss(entrances, internal_links)
      World::Dungeon.new(name, checks, locs, required_keys, before_boss)
    end

    def dungeon_reach(r, links)
      reachable = r.dup
      reachable_current = reachable.dup
      locks = Set.new
      key_count = 0
      data = {}
      loop do
        loop do
          change = false
          reachable.dup.each do |loc|
            l = links[loc] || []
            l.each do |link|
              unless reachable.include?(link.to)
                if link.locked
                  locks << link.to
                else
                  reachable_current << link.to
                  reachable << link.to
                  change = true
                end
              end
            end
          end
          break unless change
        end
        # We have propagated everything up to locked doors
        reachable_current.each do |loc|
          data[loc] = key_count
        end
        reachable_current = locks.dup
        locks.each {|l| reachable << l}
        key_count += locks.size
        break if locks.empty?
        locks = Set.new
      end
      data
    end

    def dungeon_reach_boss(r, links)
      reachable = r.dup
      loop do
        change = false
        reachable.dup.each do |loc|
          l = links[loc] || []
          l.each do |link|
            unless reachable.include?(link.to) || link.locked_boss
              reachable << link.to
              change = true
            end
          end
        end
        break unless change
      end
      reachable
    end

    def self.run()
      self.new.run()
    end
  end
end
