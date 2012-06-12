begin
  require 'bitmapper/bitmapper'
rescue LoadError =>e
  puts "Error loading c binding for bitmapepr"
end
