#!/usr/bin/env ruby
require 'socket'
require "open-uri"
require "json"

fork do

  path = ENV["HOME"] + "/vimrc/shanbay.rb"
  s = TCPSocket.new 'localhost', 11011

  while line = s.gets # Read lines from socket
    begin
      #puts line         # and print them
      word = line.chomp

      if !File.exist?(path)
        getwordui = "https://api.shanbay.com/bdc/search/?word=#{word}"
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
    end
  end

  s.close             # close socket when done
end
