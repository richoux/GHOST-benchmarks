#!/usr/bin/ruby

def usage
  puts  "Usage: " + $0 + " FILE"
end

# We must have at least a file name
if ARGV.length == 0
  usage
  exit
end

file = File.open(ARGV[0])

map = Hash.new(0)
  
# For each line in file
file.each do |line|
  words = line.split('ghost::')
  if not words[1] == nil
    instruction = words[1].split(' (')
    map["#{instruction[0]}"] += 1
  end
end

map.sort_by{|key, value| value}.reverse.each {|key, value| puts "#{key}: #{value}" }

exit
