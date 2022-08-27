require 'nokogiri'
require 'combo/common'
require 'combo/logic/world_graph'
require 'combo/logic/expr_builder'

module Combo::Logic
  class WorldGraphBuilder
    attr_reader :graph

    def initialize
      @graph = WorldGraph.new
      @expr_builder = ExprBuilder.new
      @expr_builder.load_macros(File.join(Combo::PATH_DATA, 'logic_macros.json'))
    end

    def parse(file)
      xml = Nokogiri::XML(File.read(file)) { |config| config.noblanks }
      parse_root(xml.root)
    end

    def parse_root(e)
      dungeon = nil
      if e.name == 'dungeon'
        dungeon = WorldGraph::Dungeon.new
        @graph.add_dungeon(dungeon)
      end
      scenes = e.xpath('scene')
      scenes.each do |scene|
        parse_scene(scene, dungeon)
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
      name = e['name']
      desc = e['desc']
      room = @graph.get_room(name)
      if room.nil?
        room = WorldGraph::Room.new(name, scene_id, desc)
        @graph.add_room(room)
      else
        room.scene_id = scene_id
        room.desc = desc
      end

      parse_checks(e, 'chest', :chest, room)
      parse_checks(e, 'special', :special, room)
      parse_checks(e, 'npc', :npc, room)
      parse_checks(e, 'collectible', :collectible, room)

      e.xpath('link').each do |link|
        parse_link(link, room)
      end

      if dungeon
        room.checks.each do |check|
          dungeon.add_check(check)
        end
      end
    end

    def parse_checks(e, key, type, room)
      checks = e.xpath(key)
      checks.each do |check|
        parse_check(check, type, room)
      end
    end

    def parse_check(e, type, room)
      id = e['id'].to_i(16)
      cond = e['cond']
      desc = e['desc']
      unless cond.nil?
        cond = @expr_builder.parse(cond)
      end
      content = e['content'].to_sym
      check = WorldGraph::Check.new(type, id, content, cond, desc)
      room.add_check(check)
    end

    def parse_link(e, from)
      to_name = e['to']
      cond = e['cond']
      unless cond.nil?
        cond = @expr_builder.parse(cond)
      end
      to = @graph.get_room(to_name)
      if to.nil?
        to = WorldGraph::Room.new(to_name)
        @graph.add_room(to)
      end
      link = WorldGraph::Link.new(to, cond)
      from.add_link(link)
      link_reverse = WorldGraph::Link.new(from, cond)
      to.add_link(link_reverse)
    end

    def self.run
      builder = self.new
      Dir[File.join(Combo::PATH_DATA, 'oot', 'logic', '*.xml')].each do |path|
        builder.parse(path)
      end
      builder.graph
    end
  end
end
