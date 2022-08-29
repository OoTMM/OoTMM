require 'nokogiri'
require 'combo/common'
require 'combo/logic/world_graph'
require 'combo/logic/expr_builder'

module Combo::Logic
  class WorldGraphBuilder
    def initialize
      @expr_builder = ExprBuilder.new
      @expr_builder.load_macros(File.join(Combo::PATH_DATA, 'logic_macros.json'))
      @checks = []
      @links = []
      @dungeons = []
    end

    def parse(file)
      xml = Nokogiri::XML(File.read(file)) { |config| config.noblanks }
      parse_root(xml.root)
    end

    def parse_root(e)
      dungeon = nil
      if e.name == 'dungeon'
        dungeon = []
      end
      scenes = e.xpath('scene')
      scenes.each do |scene|
        parse_scene(scene, dungeon)
      end
      if dungeon
        @dungeons << dungeon
      end
    end

    def parse_scene(e, dungeon)
      scene_id = e['id'].to_i(16)
      rooms = e.xpath('room')
      if rooms.empty?
        # Single room scene
        rooms = [e]
      end
      rooms.each do |room|
        parse_room(room, scene_id, dungeon)
      end
    end

    def parse_room(e, scene_id, dungeon)
      name = e['name'].to_sym
      desc = e['desc']

      %i[chest special npm collectible].each {|type| parse_checks(e, type, scene_id, name, desc) }

      e.xpath('link').each do |link|
        parse_link(link, name)
      end

      if dungeon
        dungeon.push(name)
      end
    end

    def parse_checks(e, type, scene_id, location, location_desc)
      checks = e.xpath(type.to_s)
      checks.each do |check|
        parse_check(check, type, scene_id, location, location_desc)
      end
    end

    def parse_check(e, type, scene_id, location, location_desc)
      id = e['id'].to_i(16)
      cond = e['cond']
      desc = e['desc']
      unless cond.nil?
        cond = @expr_builder.parse(cond)
      end
      content = e['content'].to_sym
      desc = [location_desc, desc].reject(&:nil?).join(' ') || location.to_s
      check = WorldGraph::Check.new(type, id, scene_id, location, content, cond, desc)
      @checks << check
    end

    def parse_link(e, from)
      to = e['to'].to_sym
      cond = e['cond']
      unless cond.nil?
        cond = @expr_builder.parse(cond)
      end
      @links << WorldGraph::Link.new(from, to, cond)
      @links << WorldGraph::Link.new(to, from, cond)
    end

    def graph
      WorldGraph.new(@checks, @links, @dungeons)
    end

    def self.run
      builder = self.new
      Dir[File.join(Combo::PATH_DATA, 'oot', 'logic', '*.xml')].each do |path|
        builder.parse(path)
      end
      p builder.graph
      exit 1
      builder.graph
    end
  end
end
