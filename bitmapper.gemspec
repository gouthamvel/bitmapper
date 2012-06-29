Gem::Specification.new do |s|
  s.name        = 'bitmapper'
  s.version     = '0.0.3'
  s.date        = '2012-06-29'
  s.summary     = "A fast, in-memory, bit map filter!"
  s.description = "A fast, in-memory, bit map filter!"
  s.authors     = ["Goutham Vel"]
  s.email       = 'gouthamvel@gmail.com'
  s.files       = Dir.glob('lib/**/*.rb') +
                  Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions  = ['ext/bitmapper/extconf.rb']

  s.homepage    = ''
end
