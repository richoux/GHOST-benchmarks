#!/usr/bin/ruby

def usage
  puts  "Usage: " + $0 + " FILE"
end

def median(array)
  sorted = array.sort
  len = sorted.length
  (sorted[(len - 1) / 2] + sorted[len / 2]) / 2.0
end

def pop_standard_deviation(array)
  m = array.sum(0.0) / array.length
  sum = array.inject(0){|accum, i| accum +(i-m)**2 }
  Math.sqrt( sum/array.length.to_f )
end

# We must have at least a file name
if ARGV.length == 0
  usage
  exit
end

file = File.open(ARGV[0])

times = []
time_budget = 0
number_success = 0
iterations = []
total_iterations = []

# For each line in file
file.each do |line|
  words = line.split(':')
  if not words[0] == nil
    if words[0].include? "Satisfaction error"
      if words[1].to_f == 0
        number_success += 1
      end
    elsif words[0].include? "Total number of search iterations"
      total_iterations.push(words[1].to_i)
    elsif words[0].include? "Number of search iterations"
      iterations.push(words[1].to_i)
    elsif words[0].include? "Time budget"
      block = words[1].split(', ') 
      get_rid_of_s = block[1].split('s')
      time_budget = get_rid_of_s[0].to_f    
    elsif words[0].include? "Search time"
      block = words[1].split(', ') 
      get_rid_of_s = block[1].split('s')
      runtime = get_rid_of_s[0].to_f
      if runtime >= time_budget
        runtime *= 2
      end
      times.push( runtime )
    end
  end
end

if number_success < 10
  puts "Warning: we only have #{number_success} solutions.\nTimeout penalty is '2 x Search time'"
end

mean = ( times.sum(0.0) / times.length ).round(2)                
mean_it = ( iterations.sum(0.0) / iterations.length ).round(2)                
mean_tot_it = ( total_iterations.sum(0.0) / total_iterations.length ).round(2)                

puts "Mean time: #{mean}s\n"
puts "Median time: #{median(times).round(2)}s\n"
puts "Min time: #{times.min.round(2)}s\n"
puts "Max time: #{times.max.round(2)}s\n"
puts "Std dev: #{pop_standard_deviation(times).round(2)}s\n\n"

puts "Mean iterations: #{mean_it}\n"
puts "Median iterations: #{median(iterations).round(2)}\n"
puts "Min iterations: #{iterations.min}\n"
puts "Max iterations: #{iterations.max}\n"
puts "Std dev: #{pop_standard_deviation(iterations).round(2)}\n\n"

puts "Mean total iterations: #{mean_tot_it}\n"
puts "Median total iterations: #{median(total_iterations).round(2)}\n"
puts "Min total iterations: #{total_iterations.min}\n"
puts "Max total iterations: #{total_iterations.max}\n"
puts "Std dev: #{pop_standard_deviation(total_iterations).round(2)}\n"

exit
