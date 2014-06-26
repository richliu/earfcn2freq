earfcn2freq
===========

Transfer between earfcn and Freq for LTE

usage
===========
usage : earfcn2freq [option] <earfcn or freq>
 option -B <band> : input number is freq, assign Band
 option -b : output band only(input is earfcn
 option -f : output freq only(input is earfcn)
 option -e : output earfcn only(input is freq)


make program
===========
$ cmake CMakeLists.txt
$ make 

it will create execute file "earfcn2freq"

example
===========
Transfer from FREQ to EARFCN (band parameter is necessary)
$ ./earfcn2freq -B 3 1750
UL EARFCN: 19600

only output value 
$ ./earfcn2freq -B 3 1750 -e
19600


Transfer from EARFCN to FREQ
$ ./earfcn2freq 19600
UL EARFCN: 19600
FREQ : 1750.0
BAND : 3

only output value band
$ ./earfcn2freq 19600 -b
3


only output value freq
$ ./earfcn2freq 19600 -f
1750.0

