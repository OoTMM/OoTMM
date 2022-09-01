module Combo::Logic
  class WorldGraph
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
      attr_reader :from, :to, :cond

      def initialize(from, to, cond)
        @from = from
        @to = to
        @cond = cond
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
