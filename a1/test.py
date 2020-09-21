import os
def longjmp(m, n, seed=103, eperiod=28):
  os.system(f"echo THROWCATCH && /usr/bin/time -f '%Uu %Ss %E' ./throwcatch {m} {n} {seed} {eperiod}")
  os.system(f"echo LONGJMP && /usr/bin/time -f '%Uu %Ss %E' ./longjmp {m} {n} {seed} {eperiod}")

def returnglobaltiming(program="exception", times=2050000000, eperiod=10000, seed=1003):
  os.system(f"/usr/bin/time -f '%Uu %Ss %E' ./{program} {times} {eperiod} {seed}")
  # os.system(f"/usr/bin/time -f '%Uu %Ss %E' ./{program} {times} {eperiod} {seed}")

# returnglobaltiming('returntypec', 50000000)
# longjmp(12, 12)
for i in range(4):
  returnglobaltiming(program="returntypec", times=100000000)
  # returnglobaltiming(program="exception", times=50000000)
#   # returnglobaltiming('returntypec')
