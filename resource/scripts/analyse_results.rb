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

total = 0
success = 0
dps = 0
max = 0
satloop = 0
optiloop = 0

if ARGV[0].include? "zerg"
  opti = "11400"
elsif ARGV[0].include? "protoss"
  opti = "4916.36"
elsif ARGV[0].include? "terran"
  opti = "6632.73"
else
  puts "Bad file name"
  exit
end

puts opti
  
# For each line in file
file.each do |line|
  words = line.split(':')
  if not words[0] == nil
    if words[0].include? "Satisfaction cost"
      total += 1
      if words[1].to_f == 0
        success += 1
      end
    elsif words[0].include? "Optimization cost"
      if words[1].to_f == opti.to_f
        max += 1
      end
      dps += words[1].to_i
    elsif words[0].include? "Number of optization loops"
      optiloop += words[1].to_i
    elsif words[0].include? "Number of satisfaction loops"
      satloop += words[1].to_i
    end
  end
end

mean_dps = (dps.to_f / total).round(1)
mean_satloop = (satloop.to_f / total).round(1)
mean_optiloop = (optiloop.to_f / total).round(1)
percent = (100 * max.to_f / total).round(1)
percent_success = (100 * success.to_f / total).round(1)

puts "#{ARGV[0]}:\n   #{percent}\% of perfects (#{max}/#{total})"
puts "   #{percent_success}\% of success (#{success}/#{total})\n"
puts "   Average dps = #{mean_dps}\n"
puts "   Average sat loop = #{mean_satloop}\n"
puts "   Average opt loop = #{mean_optiloop}\n"

exit
