def merge(arr, lo, mid, hi):
    #reqiures (0 <= lo <= mid <= hi)
    print(arr);
    tmp_arr = list()
    tmp_arr_len = 0

    idx1 = lo
    idx2 = mid
    while(tmp_arr_len < (hi-lo)) :
        #print("{", idx1, idx2, "}")

        # one segment exhausted, flush write the other
        if(idx1 == mid or idx2 == hi):
            if(idx1 == mid):
                for ele in arr[idx2:hi]:
                    tmp_arr.append(ele)
                tmp_arr_len += (hi-idx2)
                idx2 = hi

            elif(idx2 == hi):
                for ele in arr[idx1:mid]:
                    tmp_arr.append(ele)
                tmp_arr_len += (mid-idx1)
                idx1 = mid

            else:
                print("error")
                return
        else:
            if(arr[idx1] < arr[idx2]):
                tmp_arr.append(arr[idx1])
                tmp_arr_len += 1

                if(idx1 < mid):
                    idx1 += 1

            else:
                tmp_arr.append(arr[idx2])
                tmp_arr_len += 1

                if(idx2 < hi):
                    idx2 += 1

    #print("merge({}, {}, {})".format(lo,mid,hi), tmp_arr, "\n")

    for i in range(hi-lo):
        arr[lo+i] = tmp_arr[i]


def mergesort(arr, lo, hi):
    #reqiures (0 <= lo <= hi <= \len(arr))
    #ensures is_sorted(arr, lo, hi)
    #print("mergesort: [{}, {}]".format(lo,hi))
    if((hi-lo) <= 1):
        #print("\r{}".format(arr[lo]))
        return

    mid = lo + int((hi-lo)/2)

    mergesort(arr, lo, mid)
    mergesort(arr, mid, hi)
    merge(arr, lo, mid, hi)
