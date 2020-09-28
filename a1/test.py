import os
def longjmp(program, m, n, seed=103, eperiod=28):
  os.system(f"/usr/bin/time -f '%Uu %Ss %E' ./{program} {m} {n} {seed} {eperiod} > log")

def returnglobaltiming(program="exception", times=2050000000, eperiod=10000, seed=1003):
  os.system(f"/usr/bin/time -f '%Uu %Ss %E' ./{program} {times} {eperiod} {seed} > 10")
  # os.system(f"/usr/bin/time -f '%Uu %Ss %E' ./{program} {times} {eperiod} {seed}")

# for i in range(4):
#   longjmp("throwcatch", 12, 12, seed=103, eperiod=40)
returnglobaltiming(program="exception", times=500000000)
returnglobaltiming(program="returnglobal", times=500000000)

# for i in range(12):
  # returnglobaltiming(program="returnglobal", times=100000000, eperiod=50)
#   returnglobaltiming(program="returntypec", times=100000000, eperiod=50)
