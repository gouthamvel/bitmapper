begin
  require 'bitmapper/bitmapper'
catch LoadError
  puts "Error loading c binding for bitmapepr"
end
class Bitmapper

  def self.hi
    puts "Hello world!"
  end
end
