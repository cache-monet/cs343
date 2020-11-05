import os

def run(i):
    print(f"depth {i} ")
    os.system(f'/usr/bin/time -f "%Uu %Ss %E %Mkb" ./quicksort -t 500000000 {i}')

for i in range(6): run(i)

/usr/bin/time -f "%Uu %Ss %E %Mkb" ./buffer 50 55 50000 30 10
/usr/bin/time -f "%Uu %Ss %E %Mkb" ./bufferBUSY 50 55 50000 30 10

/usr/bin/time -f "%Uu %Ss %E %Mkb" ./buffer 55 80 25000 20 10 3
/usr/bin/time -f "%Uu %Ss %E %Mkb" ./bufferBUSY 55 100 50000 20 10 1

/usr/bin/time -f "%Uu %Ss %E %Mkb" ./q1 1 40000000
/usr/bin/time -f "%Uu %Ss %E %Mkb" ./q1 1 20000000
/usr/bin/time -f "%Uu %Ss %E %Mkb" ./q1 4 2000000
                                          20000000