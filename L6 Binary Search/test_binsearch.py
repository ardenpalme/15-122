from binsearch import binsearch

A = [5, 7, 11, 19, 34, 42, 65, 65, 89, 123]
x = 33

idx = binsearch(x, A, len(A))
if(idx > 0):
    print("Found! A[{}]=={}".format(idx,A[idx]))
else:
    print("{} not in A".format(x))

