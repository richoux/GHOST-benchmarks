#!/usr/bin/ruby

def usage
  puts "#{$0}"
  puts  "Usage: " + $0 + " FILE"
end

# We must have at least a file name
if ARGV.length == 0
  usage
  exit
end

# Open file
file = File.open(ARGV[0])
hmap = Hash.new(0)
hnb = Hash.new(0)
mapname = ""
fullname = ""
#inputs = 0
#loops = 0

# For each line in file
file.each do |line|
  words = line.split(': ')
  if words[0] == "File name"
    path = words[1].split('/')
    fullname = path[2][0..-2]
    mapnamesplit = fullname.split('-')
    mapname = mapnamesplit[0]
    hnb[mapname] += 1
  end
  if words[0] == "Global cost" and words[1].to_i == 0
    hmap[mapname] += 1
  end
end

hmap.each do |k,v|
  hmap[k] = v * 100 / hnb[k]
end

mapsort = hmap.sort_by {|mapname,nb| nb}.reverse
mapsort.each do |mapname,nb|
  if mapname.size > 21
    puts "#{mapname}: \t #{nb}\% of success rate"
  elsif mapname.size < 14
    puts "#{mapname}: \t\t\t #{nb}\% of success rate"
  else
    puts "#{mapname}: \t\t #{nb}\% of success rate"
  end    
end

exit
