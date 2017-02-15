#!/usr/bin/env ruby
require 'socket'
require "open-uri"
require "json"
require 'uri'

fork do
#loop do

  path = ENV["HOME"] + "/vimrc/shanbay.rb"
  s = TCPSocket.new 'localhost', 11011

  while line = s.gets # Read lines from socket
    begin
      #puts line         # and print them
      text = line.chomp
      puts text
      json = JSON.parse text
      puts json
      selected = json["changed"]
      #word = json[selected]
      word = json.values_at(selected).first
      clipoboard = json["Clipboard"]
      selection = json["Selection"]
      findbuffer = json["FindBuffer"]
      puts word
      puts "scan begin"
      if word.scan(/[\w-]+/).empty?
        puts "raise error word"
        raise 'Error Word'
      end
      word = word.scan(/[\w-]+/).first
      puts word
      puts "scan end"

      if !File.exist?(path)
        getwordui = URI.escape("https://api.shanbay.com/bdc/search/?word=#{word}")
        open( getwordui) do |io|
          jsonstr =  io.read
          json = JSON.parse(jsonstr)
          data = json["data"]
          #cndef = data["cn_definition"]
          cndef = data["definition"]
          endef = data["en_definition"]
          word = data["content"]
          pron = data["pron"]

          audio = data["us_audio"]
          #puts data

          s.puts word + "  / "+ pron + " /  " + cndef
        end
      else
        
        command = path + " " + word
        data = `#{command}`
        s.puts data
      end

    rescue
      puts "rescue word"
    end
  end

  s.close             # close socket when done
end
