import sys
import random

# $python gen_trace.py  N  (--realloc)  (--calloc)
#                          () is optional


# handle argv
if len(sys.argv) == 1:
    print("need N")
    sys.exit(1)

elif len(sys.argv) == 2:
    num = int(sys.argv[1])     # malloc and free randomly upto 'input' times
    realloc_option = False     # test realloc
    calloc_option = False      # test calloc

elif len(sys.argv) > 4:
    print("argc = 2,3,4")
    sys.exit(1)

else:
    num = int(sys.argv[1])     # malloc and free randomly upto 'input' times
    realloc_option = False     # test realloc
    calloc_option = False      # test calloc
    if("--realloc" in sys.argv):
        realloc_option = True
    if("--calloc" in sys.argv):
        calloc_option = True


# malloc and free randomly upto 'input' times
# "\n" is placeholder for metadata: total number of operations, total malloc counts
trace_lines=["\n"]
malloc_list=[]
malloc_cnt=0

for i in range(num):
    if(random.randrange(0,2) == 0):     # 0:malloc, 1:free
        malloc_list.append(malloc_cnt)
        trace_lines.append("m "+str(malloc_cnt)+ " " +
                    str(random.randrange(1,4097))+'\n' )   # malloc(1~4k)
        malloc_cnt = malloc_cnt + 1

    else:
        if(len(malloc_list) == 0):
            continue
        chosen = random.choice(malloc_list)
        malloc_list.remove(chosen)
        trace_lines.append("f "+ str(chosen) + '\n')

    # realloc and calloc
    if(i%16 == 15):
        if realloc_option and len(malloc_list)>0:
            chosen = random.choice(malloc_list)
            trace_lines.append("r "+str(chosen)+ " " +
                        str(random.randrange(1,4097))+'\n' )   # realloc(sth,1~4k)
        if(calloc_option):
            malloc_list.append(malloc_cnt)
            trace_lines.append("c "+str(malloc_cnt)+ " " +
                        str(random.randrange(1,4097))+'\n' )   # malloc(1~4k)
            malloc_cnt = malloc_cnt + 1


#now free every remaining malloc-ed regions
for m in malloc_list:
    trace_lines.append("f "+ str(m) + '\n')

trace_lines[0] = str(len(trace_lines)-1)+ " "+ str(malloc_cnt)+ " \n"   # total number of operations, total malloc counts
trace = open("traces/trace"+str(num), 'w')
trace.writelines(trace_lines)
trace.close()

print("generated trace file with "+str(len(trace_lines)-1) +" operations")