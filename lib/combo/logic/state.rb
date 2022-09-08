module Combo::Logic
  class State
    attr_accessor :items, :keys, :boss_keys, :reachable_locations

    def initialize
      @items = {}
      @keys = {}
      @boss_keys = Set.new
      @reachable_locations = [:child, :adult].map{|age| [age, Set.new] }.to_h
    end
  end
end
