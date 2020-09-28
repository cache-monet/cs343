import os

def longjmp(m: int, n: int, seed=103, eperiod=28):
  os.system(f"echo LONGJMP && ./longjmp {m} {n} {seed} {eperiod} > lj.log")
  os.system(f"echo THROWCATCH && ./throwcatch {m} {n} {seed} {eperiod} > tc.log")

# longjmp(10, 20, eperiod=40)
longjmp(4, 3, seed=61, eperiod=61)

