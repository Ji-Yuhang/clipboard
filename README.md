# clipboard
qt编写的系统剪贴板监听小工具

1. 使用 TCP 连接端口 11011, 剪贴板变化时候 会发送一条带回车的utf-8字符串。使用 readline阻塞即可"
```ruby
require 'socket'

s = TCPSocket.new 'localhost', 11011

while line = s.gets # Read lines from socket
  puts line         # and print them
end

s.close             # close socket when done
```
2. 在系统目录下建立 ~/.clipboard_callback 文件, 剪贴板变化时会被调用"
```ruby
#!/usr/bin/env ruby
puts "callback begin"
puts ARGV
require 'rest-client'
rc = RestClient.post "http://localhost:3000/api/v1/clipboard",{word:ARGV[0]}
puts rc

puts "callback end"
```
