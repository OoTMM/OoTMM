module Combo::Logic
  class WorldGraph
    class Dungeon
      attr_accessor :checks

      def initialize
        @checks = []
      end

      def add_check(check)
        @checks << check
        check.dungeon = self
      end
    end

    class Room
      attr_reader :name, :checks, :links
      attr_accessor :scene_id, :desc, :graph, :reachable

      def initialize(name, scene_id = nil, desc = nil)
        @name = name
        @scene_id = scene_id
        @desc = desc
        @checks = []
        @links = []
        @reachable = false
      end

      def add_check(check)
        @checks << check
        check.room = self
      end

      def add_link(link)
        @links << link
      end
    end

    class Check
      attr_reader :type, :id, :cond, :desc
      attr_accessor :room, :dungeon, :fixed, :content, :reachable

      def initialize(type, id, content, cond, desc)
        @type = type
        @id = id
        @content = content
        @cond = cond
        @desc = desc
        @room = nil
        @dungeon = nil
        @fixed = false
        @reachable = false
      end
    end

    class Link
      attr_reader :to
      attr_reader :cond

      def initialize(to, cond)
        @to = to
        @cond = cond
      end
    end

    attr_reader :rooms, :dungeons

    def initialize
      @rooms = []
      @dungeons = []
      @checks = []
      @rooms_by_name = {}
    end

    def add_room(room)
      @rooms << room
      @rooms_by_name[room.name] = room
    end

    def get_room(name)
      @rooms_by_name[name]
    end

    def add_dungeon(dungeon)
      @dungeons << dungeon
    end

    def checks
      @rooms.map {|r| r.checks}.flatten
    end
  end
end
