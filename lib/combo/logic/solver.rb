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
      @items = {}
    end

    def run
      # Mark link's house as reachable
      @graph.get_room('KF_LinkHouse').reachable = true

      # Fix the goal items
      fix_goals()

      # We need to make sure that the small keys, compass map and boss keys stay in the same dungeon.
      @graph.dungeons.each {|d| fix_dungeon(d)}

      # Fix required items
      fix_required()

      # Shuddle remaining items
      fix_all()

      export()
    end

    def propagate()
      loop do
        changed = false
        @graph.rooms.each do |room|
          next unless room.reachable
          room.links.each do |link|
            next if link.to.reachable
            if eval_cond(link.cond, @graph, @items)
              link.to.reachable = true
              changed = true
            end
          end
          room.checks.each do |check|
            next if check.reachable
            if eval_cond(check.cond, @graph, @items)
              check.reachable = true
              changed = true
            end
          end
        end
        break unless changed
      end
    end

    def fix_goals()
      checks = @graph.checks.select {|c| goal_item?(c)}
      shuffle(checks)
      checks.each {|c| c.fixed = true}
    end

    def fix_dungeon(dungeon)
      checks = dungeon.checks.select{|x| !x.fixed}
      shuffle(checks)
      checks.each do |check|
        if dungeon_item?(check)
          check.fixed = true
        end
      end
    end

    def fix_required()
      loop do
        # Propagate and check if everything is reachable
        propagate()
        break if @graph.checks.all? {|c| c.reachable}

        # Everything is not reachable, we need to add a progression item
        fix_new_progression()
      end
    end

    def fix_new_progression()
      # Find a suitable item
      unreachable_links = @graph.rooms.map {|r| r.links}.flatten.select {|l| l.to.reachable == false}
      unreachable_checks = @graph.checks.select {|x| x.reachable == false}

      conds = unreachable_links.map {|l| l.cond} + unreachable_checks.map {|c| c.cond}
      conds.select! {|c| !c.nil?}

      missing = conds.map {|c| c.eval(@graph, @items)}.map{|x| x.missing}.reduce(&:|)

      # Find a check giving a missing item
      check_src = pool.select {|c| missing.include?(c.content)}.shuffle.first
      check_dst = pool(reachable: true).shuffle.first

      # Swap and fix
      swap(check_src, check_dst)
      check_dst.fixed = true

      # Mark the item as found
      add_item(check_dst.content)
    end

    def fix_all()
      shuffle(pool())
    end

    def add_item(item)
      case item
      when :BOMBCHUS_5, :BOMBCHUS_10, :BOMBCHUS_20
        item = :BOMBCHU
      end
      @items[item] = true
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
        [check.type, check.room.scene_id, check.id, check.content]
      end
    end

    def export_spoiler
      data = []
      @graph.checks.each do |check|
        data << "%s: %s\n" % [check_nice_name(check), check.content]
      end
      data.sort!
      data.join()
    end

    def check_nice_name(check)
      desc = check.desc
      room_desc = check.room.desc
      if room_desc.nil?
        room_desc = check.room.name
      end
      [room_desc, desc].select{|x| !x.nil?}.join(' - ')
    end

    def export
      data = export_data()
      spoiler = export_spoiler()
      Output.new(data, spoiler)
    end

    def eval_cond(expr, *args)
      if expr.nil?
        true
      else
        expr.eval(*args).result
      end
    end

    def pool(opts = {})
      pool = @graph.checks.select{|c| c.fixed == false}
      if opts[:reachable]
        pool.select!{|c| c.reachable}
      end
      if opts[:unique]
        pool = pool.uniq{|c| c.content}
      end
      pool
    end

    def self.run
      logic = self.new
      logic.run
    end
  end
end
