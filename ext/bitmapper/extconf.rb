# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Give it a name
extension_name = 'bitmapper'

# The destination
dir_config("bitmapper/#{extension_name}")

# Do the work
create_makefile("bitmapper/#{extension_name}")
