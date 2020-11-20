from os import system

args = "1000000 100"
system('/usr/bin/time -f "%Uu %Ss %E" ./q1 {} > /dev/null'.format(args))