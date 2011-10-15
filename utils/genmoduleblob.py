#!/usr/bin/python

import sys, struct

if len(sys.argv) < 2:
    print 'usage:', sys.argv[0], 'module1 [module2 [module3 [...]]]'
    sys.exit(1)

magic = 0xAABB0011
modCount = len(sys.argv) - 1

moduleBlobs = []
for i in range(modCount):
    f = open(sys.argv[i + 1])
    moduleBlobs.append(f.read())
    f.close()

#output header
sys.stdout.write(struct.pack("=II", magic, modCount))

#output offsets
#8 for magic and count + 4*offsets
curOffset = 8 + 4*modCount
for i in range(modCount):
    sys.stdout.write(struct.pack("=I", curOffset))
    curOffset += len(moduleBlobs[i])

#output the blobs
for i in range(modCount):
    sys.stdout.write(moduleBlobs[i])
