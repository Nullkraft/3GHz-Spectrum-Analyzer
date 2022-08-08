'display_message.bas for testing Binary USB Communication Laptop<>Arduino
' For Use with CmdProcessor.ino

dim shared as string portspeed, ComStr
dim shared as ulong dataWord            ' Using ulong forces 'put' to send 4 bytes

portSpeed="1000000"
ComStr="/dev/ttyUSB0:" + portSpeed + ",N,8,1"

print ComStr
open com ComStr for Binary as #1

dataWord = 6143         ' Turn LED off
put #1, , dataWord

print " Press any key to exit"
print " Then cycle the Arduino serial monitor to actually close the port"

close #1

sleep
