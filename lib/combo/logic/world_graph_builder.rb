require 'nokogiri'
require 'combo/common'
require 'combo/logic/world_graph'
require 'combo/logic/expr_builder'

module Combo::Logic
  class WorldGraphBuilder
    def initialize
      @expr_builders = Combo::GAMES.map {|game|
        e = ExprBuilder.new(game)
        e.load_macros(File.join(Combo::PATH_DATA, 'logic_macros.json'))
        [game, e]
      }.to_h
      @checks = []
      @links = []
      @dungeons = []
    end

    def parse(game, file)
      xml = Nokogiri::XML(File.read(file)) { |config| config.noblanks }
      xml.errors.each do |e|
        raise "XML error: #{e}"
      end
      parse_root(game, xml.root)
    end

    def parse_root(game, e)
      dungeon = nil
      if e.name == 'dungeon'
        dungeon = []
      end
      desc = [e['desc']]
      scenes = e.xpath('scene')
      scenes.each do |scene|
        parse_scene(game, scene, dungeon, desc)
      end
      if dungeon
        @dungeons << dungeon
      end
    end

    def parse_scene(game, e, dungeon, desc)
      scene_id = e['id'].to_i(16)
      rooms = e.xpath('room')
      desc = desc + [e['desc']]
      if rooms.empty?
        # Single room scene
        rooms = [e]
      end
      rooms.each do |room|
        parse_room(game, room, scene_id, dungeon, desc)
      end
    end

    def parse_room(game, e, scene_id, dungeon, desc)
      name = e['name'].to_sym
      if e.name == 'room'
        desc = desc + [e['desc']]
      end

      %i[chest special npc collectible].each {|type| parse_checks(game, e, type, scene_id, name, desc) }

      e.xpath('link').each do |link|
        parse_link(game, link, name)
      end

      if dungeon
        dungeon.push(name)
      end
    end

    def parse_checks(game, e, type, scene_id, location, desc)
      checks = e.xpath(type.to_s)
      checks.each do |check|
        parse_check(game, check, type, scene_id, location, desc)
      end
    end

    def parse_check(game, e, type, scene_id, location, desc)
      id = e['id'].to_i(16)
      cond = e['cond']
      desc = desc + [e['desc']]
      desc = desc.reject(&:nil?)
      if desc.empty?
        desc = location.to_s
      else
        desc = desc.join(' ')
      end
      unless cond.nil?
        cond = @expr_builders[game].parse(cond)
      end
      content = Util.game_id(game, e['content'])
      check = WorldGraph::Check.new(game, type, id, scene_id, location, content, cond, desc)
      @checks << check
    end

    def parse_link(game, e, from)
      to = e['to'].to_sym
      cond = e['cond']
      flags = (e['flags'] || "").split(',').map{|x| x.gsub('-', '_').to_sym }
      unless cond.nil?
        cond = @expr_builders[game].parse(cond)
      end
      @links << WorldGraph::Link.new(from, to, cond)
      unless flags.include?(:one_way)
        @links << WorldGraph::Link.new(to, from, cond)
      end
    end

    def graph
      WorldGraph.new(@checks, @links, @dungeons)
    end

    def self.run
      builder = self.new
      Combo::GAMES.each do |game|
        Dir[File.join(Combo::PATH_DATA, game.to_s, 'logic', '*.xml')].each do |path|
          builder.parse(game, path)
        end
      end
      builder.graph
    end
  end
end
