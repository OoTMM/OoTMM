module Combo::Logic
  class WorldGraph
    class Check
      def initialize(type, id, scene_id, location, content, cond, desc)
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
