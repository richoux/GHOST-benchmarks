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
wins = 0
draws = 0
sat = 0
sat_total = 0
live = 0
live_e = 0
hp = 0
hp_e = 0
me = false

# For each line in file
file.each do |line|
  if line.include? "#"
    next
  end

  if line.include? "Satisfaction cost"
    sat_total += 1
    words = line.split(': ')
    if words[1].to_i == 0
      sat += 1
    end
  end

  if line.include? "Draw"
    draws += 1
    total += 1
    next
  end

  words = line.split(': ')
  if words[0].include? "Winner"
    if words[1].include? "You"
      wins += 1
      me = true
    else
      me = false
    end

    total += 1
  elsif words[0].include? "Diff"
    if words[1].to_i > 0
      live += words[1].to_i
    elsif words[1].to_i < 0
      live_e += words[1].to_i
    end
  elsif words[0].include? "HP"
    if me
      hp += words[1].to_f
    else
      hp_e += words[1].to_f      
    end
  end
end

mean_live = (live.to_f / wins).round(1)
mean_hp = (hp / wins).round(1)
mean_live_e = (-live_e.to_f / ( total - wins - draws ) ).round(1)
mean_hp_e = (hp_e / ( total - wins - draws ) ).round(1)
percent_sat = (100 * sat.to_f / sat_total).round(2)
percent = (100 * wins.to_f / total).round(1)

puts "#{ARGV[0]}:\n   #{percent}\% of wins (#{wins}/#{total}, #{draws} draws)"
puts "   Satisfaction rate: #{percent_sat}\% (#{sat}/#{sat_total})"
puts "   Average number of GHOST living units = #{mean_live}"
puts "   Average number of enemy living units = #{mean_live_e}"
puts "   Average GHOST HP = #{mean_hp}"
puts "   Average enemy HP = #{mean_hp_e}"

exit
