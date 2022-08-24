require 'nokogiri'
require 'combo/common'
require 'combo/world_graph'

class Combo::WorldGraphBuilder
  attr_reader :graph

  def initialize
    @graph = Combo::WorldGraph.new
  end

  def parse(file)
    xml = Nokogiri::XML(File.read(file)) { |config| config.noblanks }
    parse_root(xml.root)
  end

  def parse_root(e)
    dungeon = nil
    if e.name == 'dungeon'
      dungeon = Combo::WorldGraph::Dungeon.new
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
    room = @graph.get_room(name)
    if room.nil?
      room = Combo::WorldGraph::Room.new(name, scene_id)
      @graph.add_room(room)
    end
    e.xpath('chest').each do |check|
      parse_check(check, :chest, room)
    end
    e.xpath('special').each do |check|
      parse_check(check, :special, room)
    end
    e.xpath('link').each do |link|
      parse_link(e, room)
    end
    if dungeon
      room.checks.each do |check|
        dungeon.add_check(check)
      end
    end
  end

  def parse_check(e, type, room)
    id = e['id'].to_i(16)
    content = e['content'].to_sym
    check = Combo::WorldGraph::Check.new(type, id, content)
    room.add_check(check)
  end

  def parse_link(e, from)
    to_name = e['to']
    to = @graph.get_room(to_name)
    if to.nil?
      to = Combo::WorldGraph::Room.new(to_name)
      @graph.add_room(to)
    end
    link = Combo::WorldGraph::Link.new(to)
    from.add_link(link)
    link_reverse = Combo::WorldGraph::Link.new(from)
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
