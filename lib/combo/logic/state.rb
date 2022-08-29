module Combo::Logic
  class State
    attr_accessor :items, :reachable_locations

    def initialize
      @items = {}
      @reachable_locations = [:child, :adult].map{|age| [age, Set.new] }.to_h
    end
  end
end
