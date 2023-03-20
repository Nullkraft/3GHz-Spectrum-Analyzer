'FB_COM_BLOCKTEST_8B.bas for testing Binary USB Communication Laptop<>Arduino
' For Use with Arduino_Com_Test_8.ino

dim shared as integer x, sizeArray = 4

dim shared as string portspeed,ComStr
dim shared as ubyte Marray(sizeArray)

portSpeed="1000000"
ComStr="/dev/ttyUSB0:" + portSpeed + ",N,8,1"

print ComStr
open com ComStr for Binary as #1

for x=sizeArray-1 to 0 step -1 :read MArray(x) :next
put #1, , MArray() 

'data (0),(0),(142),(255)    ' 0000-8EFF
data (0),(0),(15),(255)      ' 0000-07FF


close #1
End 1
