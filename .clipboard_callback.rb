#!/usr/bin/env ruby
require 'rest-client'
clipboard = ARGV[0]
selection = ARGV[1]
find_buffer = ARGV[2]

def http_post( clipboard, selection, find_buffer) 
    path = "http://127.0.0.1:7001/ping"
    rc = RestClient.get path, params:{clipboard: clipboard, selection: selection, find_buffer: find_buffer}
    puts rc
end

def save(a,b,c)
	File.open(ENV['HOME']+'/.clipboard_log.txt','a+') do |io|
		io.puts(a)
		io.puts(b)
		io.puts(c)
	end
end
puts "callback begin"
puts ARGV
http_post( clipboard, selection, find_buffer)
save( clipboard, selection, find_buffer)
puts "callback end"
