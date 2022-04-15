import re 

class Allocation():
    def __init__(self, addr, size, caller, logline):
        self.addr = addr
        self.size = size
        self.caller = caller
        self.logline = logline

    def __str__(self):
        return "{}:{}:{}:{}".format(self.logline, self.caller, self.addr, self.size)


heapModel = {}

mallocRegex = "MALLOC ([0-9xXa-fA-F]*) : ([0-9xXa-fA-F]*) : ([0-9]*)"
freeRegex = "FREE ([0-9xXa-fA-F]*) : ([0-9xXa-fA-F]*) : ([0-9]*)"

with open("log1.txt", "r") as log:
    for logline, line in enumerate(log.readlines()):
        mallocRes = re.search(mallocRegex, line)
        freeRes  = re.search(freeRegex, line)

        if(mallocRes is not None):
            addr = mallocRes.group(2)
            size = mallocRes.group(3)
            caller = mallocRes.group(1)
            heapModel[addr] = Allocation(addr, size, caller, logline)

        elif(freeRes is not None):
            addr = freeRes.group(2)
            size = freeRes.group(3)
            caller = freeRes.group(1)
            rmalloc = Allocation(addr, size, caller, logline)

            
            if(addr in heapModel.keys()):
                oldAlloc = heapModel.pop(addr)
                
                if(oldAlloc.size != size):
                    print("ERROR: Free called with size mismatch.")
                    print(">> MALLOC: {}".format(oldAlloc))
                    print(">> FREE: {}".format(rmalloc))
            else:
                print("ERROR: Free called witout alloc")
                print(">> FREE: {}".format(rmalloc))

leakedRAM = {}
for unfreed in heapModel.values():
    
    if(unfreed.caller in leakedRAM.keys()):
        leakedRAM[unfreed.caller] += int(unfreed.size)
    else:
        leakedRAM[unfreed.caller] = int(unfreed.size)


for src in leakedRAM.keys():
    print("ERROR: Source {} leaked {} bytes.".format(src, leakedRAM[src]))

