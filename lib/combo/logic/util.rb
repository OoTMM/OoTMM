require 'set'

module Combo::Logic
  module Util
    def self.make_multihash_set(data)
      make_multihash(Set, data)
    end

    def self.make_multihash_array(data)
      make_multihash(Array, data)
    end

    def self.make_multihash(klass, data)
      h = {}
      data.each do |pair|
        k = pair[0]
        v = pair[1]
        h[k] ||= klass.new
        h[k] << v
      end
      h
    end
  end
end
