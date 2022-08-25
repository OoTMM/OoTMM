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
      attr_reader :name, :checks
      attr_accessor :scene_id, :graph

      def initialize(name, scene_id = nil)
        @name = name
        @scene_id = scene_id
        @checks = []
        @links = []
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
      attr_reader :type, :id
      attr_accessor :room, :dungeon, :fixed, :content

      def initialize(type, id, content)
        @type = type
        @id = id
        @room = nil
        @dungeon = nil
        @content = content
        @fixed = false
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
