Gem::Specification.new do |s|
  s.name        = 'bitmapper'
  s.version     = '0.0.1'
  s.date        = '2012-06-11'
  s.summary     = "A fast bit map filter!"
  s.description = "PENDING"
  s.authors     = ["Goutham Vel"]
  s.email       = 'gouthamvel@gmail.com'
  s.files = Dir.glob('lib/**/*.rb') +
            Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions = ['ext/bitmapper/extconf.rb']

  s.homepage    =
    ''
end
