module Combo::Logic
  class World
    class Check
      attr_reader :game, :type, :id, :scene_id, :location, :cond, :desc
      attr_accessor :content

      def initialize(game, type, id, scene_id, location, content, cond, desc)
        @game = game
        @type = type
        @id = id
        @scene_id = scene_id
        @location = location
        @content = content
        @cond = cond
        @desc = desc
      end
    end

    class Link
      attr_reader :from, :to, :cond, :locked, :locked_boss

      def initialize(from, to, cond, flags)
        @from = from
        @to = to
        @cond = cond
        @locked = flags.include?(:locked)
        @locked_boss = flags.include?(:locked_boss)
      end
    end

    class Dungeon
      attr_reader :name, :checks, :locations, :required_keys, :before_boss

      def initialize(name, checks, locations, required_keys, before_boss)
        @name = name
        @checks = checks
        @locations = locations
        @required_keys = required_keys
        @before_boss = before_boss
      end
    end

    attr_reader :checks, :links, :dungeons

    def initialize(checks, links, dungeons)
      @checks = checks
      @links = links
      @dungeons = dungeons
    end
  end
end
