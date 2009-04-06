#!/bin/bash
dd if=/dev/zero of=floppyrohling_linux.img count=2880 bs=512
mkdosfs -n "MOS FLOPPY " -s 1 -S 512 -f 2 -h 0 -F 12 -r 224 -R 1 floppyrohling_linux.img
