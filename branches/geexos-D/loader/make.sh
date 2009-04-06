#!/bin/bash
mkdir -p tmp/mnt 2> /dev/null
echo "Assembling"
nasm -f bin stage1/stage1.asm -o tmp/stage1.bin
nasm -f bin stage2/stage2.asm -o tmp/stage2.bin
nasm -f bin testkernel/kernel.asm -o tmp/kernel.bin
echo "Creating floppy image"
dd if=floppy/floppyrohling_win.img of=tmp/tmp.img bs=512 count=2879 skip=1
cat tmp/stage1.bin tmp/tmp.img > floppyimage.img
echo "Copying files"
mount -o loop floppyimage.img tmp/mnt
cp tmp/stage2.bin tmp/mnt
cp tmp/kernel.bin tmp/mnt
#cp ../kernel/kernel.bin tmp/mnt/kernel.bin
umount tmp/mnt
rm -rf tmp 2> /dev/null
