import math

def binsearch(x, A, n):
    #requires (0 <= n <= len(A))
    #requires is_sorted(A, 0, n)
    #ensures (-1 == result && !is_in(x, A, 0, n)) || 
    #        (0 <= result <= n && A[result] == x)
    lower = 0
    upper = n

    while(lower < upper):
        #loop_invariant 0 <= lower <= upper <= n
        #loop_invariant (lower == 0 || A[lower - 1] < x)
        #loop_invariant (upper == n || A[upper] > x)

        #print("A[{}, {})".format(lower,upper))

        mid = lower + math.floor((upper-lower)/2)

        if(x == A[mid]): 
            return mid
        elif(x < A[mid]):
            upper = mid
        else: # x > A[mid]
            lower = mid+1
    
    return -1
