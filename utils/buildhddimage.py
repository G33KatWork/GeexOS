#!/usr/bin/env python
# -*- coding: utf-8 -*-

# sample json syntax:
#
# {
#     "hddsize": 204800,
#     "mbrCodeFile": "/path/to/mbr.bin",
#     "partitions": [
#         {
#             "active": true,
#             "startSector": 63,
#             "endSector": 204800,
#             "type": 6,
#             "fstype": "fat16",
#             "mbrCodeFile": "/path/to/partition/mbr.bin",
#             "filesToCopy": [
#                 {
#                     "source": "/source/file",
#                     "destination": "destinationfile"
#                 }
#             ]
#         }
#     ]
# }

import sys
import json
import struct
import subprocess
import re
import os
import tempfile
import shutil

SECTORSIZE = 512

def main(argv):
  if len(argv) != 3:
    print >> sys.stderr, "Usage: %s <json hdd definition> <hdd image file>" % argv[0]
    sys.exit(-1)
  
  imagefile = argv[2]
  
  jsonfile = file(argv[1])
  partdefinition = json.load(jsonfile)

  imagelen = partdefinition["hddsize"]

  print "-> Creating an hdd image with %d bytes" % (SECTORSIZE*imagelen)
  hddimage = file(imagefile, "w+b")
  hddimage.write("\0" * (SECTORSIZE*imagelen))
  hddimage.close()

  if partdefinition["mbrCodeFile"] != None:
    print "-> Using MBR code file %s" % partdefinition["mbrCodeFile"]
    m = MBR(file(partdefinition["mbrCodeFile"], "rb").read())
  else:
    m = MBR()

  print "-> Creating %d partitions" % len(partdefinition["partitions"])

  i = 0
  for partition in partdefinition["partitions"]:
    m.setPartition(i, PrimaryPartition(partition["startSector"], partition["endSector"], partition["type"], partition["active"]))
    i += 1
  
  print "-> Writing MBR to file"
  
  hddimage = file(imagefile, "r+b")
  hddimage.write(m.getMBR())
  hddimage.close()

  fs = getFilesystemCreator(imagefile)
  i = 0
  for partition in partdefinition["partitions"]:
    if partition["fstype"] != None:
      fs.createFilesystem(i, partition["fstype"], partition["mbrCodeFile"])
    i += 1
  
  #HACK HACK
  #If the loopbackdev is not recreated under Mac OS before mounting
  #hdiutil doesnt find any filesystems on the device
  fs.__del__()

  print "-> Copying files to partitions"
  i = 0
  for partition in partdefinition["partitions"]:
    if partition["filesToCopy"] != None:
      target = fs.mountFilesystem(i)
      
      for fileToCopy in partition["filesToCopy"]:
        #TODO: create subdirectories on partition
        print "-> Copying from %s to %s" % (fileToCopy["source"], fileToCopy["destination"])
        shutil.copyfile(fileToCopy["source"], os.path.join(target, fileToCopy["destination"]))
    i += 1


def getFilesystemCreator(imagefile):
  if sys.platform == 'darwin':
    return MacOSFilesystemCreator(imagefile)
  else:
    raise Exception("Unsupported platform")



class BootCodeInstaller:
  def install(self, blockDevice, filesystemType, codefile):
    print "-> Installing bootcode from %s into %s" % (codefile, blockDevice)
    if filesystemType == "ext2":
      self.__installExt2(blockDevice, codefile)
    elif filesystemType == "fat16" or filesystemType == "fat12":
      self.__installFat16(blockDevice, codefile)
    elif filesystemType == "fat32":
      self.__installFat32(blockDevice, codefile)
    else:
      raise Exception("Unknown filesystem type supplied")
  
  
  def __installExt2(self, blockDevice, codefile):
    bootcode = open(codefile, "rb").read()
    blockdev = open(blockDevice, "r+b")
    blockdev.write(bootcode)
  
  def __installFat16(self, blockDevice, codefile):
    bootcode = open(codefile, "rb").read()
    
    blockdev = open(blockDevice, "r+b")
    blockdev.seek(3, os.SEEK_SET)
    bpb = blockdev.read(0x3B)   #BPB is 0x3B bytes
    
    blockdev.seek(0, os.SEEK_SET)
    blockdev.write(bootcode)
    blockdev.seek(3, os.SEEK_SET)
    blockdev.write(bpb)
  
  def __installFat32(self, blockDevice, codefile):
    raise Exception("Not implemented")



class MacOSFilesystemCreator:
  def __init__(self, imagefile):
    self.__imagefile = imagefile
    self.__loopbackDevice = None
  
  
  def __del__(self):
    self.__destroyLoopbackDevice()
  
  
  def __createLoopbackDevice(self):
    print "-> Creating loopback device for %s" % self.__imagefile
    
    output = subprocess.check_output(['hdiutil', 'attach', '-nomount', self.__imagefile]).strip()
    
    match = re.search("^(/dev/disk[0-9]+)*", output)
    if not match:
      raise Exception("Device string returned looks malformed")
    
    self.__loopbackDevice = match.group(0)
  
  
  def __destroyLoopbackDevice(self):
    print "-> Destroying loopback device %s" % self.__loopbackDevice
    if self.__loopbackDevice != None:
      try:
        subprocess.check_call(['hdiutil', 'detach', self.__loopbackDevice])
      except Exception:
        raise Exception("Destroying of loopback device didn't succeed")
    
      self.__loopbackDevice = None
  
  
  def createFilesystem(self, partitionIndex, filesystemType, bootcodeFile):
    if self.__loopbackDevice == None:
      self.__createLoopbackDevice()
  
    partdevice = self.__getPartdeviceFromIndex(partitionIndex)
    print "-> Creating filesystem %s on partition %s" % (filesystemType, partdevice)
    
    if filesystemType == "ext2":
      try:
        subprocess.check_call(['fuse-ext2.mke2fs', partdevice])
      except Exception:
        raise Exception("Creating ext2 filesystem on {0} failed".format(partdevice))
    elif filesystemType == "fat12":
      try:
        subprocess.check_call(['newfs_msdos', "-F", "12", partdevice])
      except Exception:
        raise Exception("Creating FAT12 filesystem on {0} failed".format(partdevice))
    elif filesystemType == "fat16":
      try:
        subprocess.check_call(['newfs_msdos', "-F", "16", partdevice])
      except Exception:
        raise Exception("Creating FAT16 filesystem on {0} failed".format(partdevice))
    elif filesystemType == "fat32":
      try:
        subprocess.check_call(['newfs_msdos', "-F", "32", partdevice])
      except Exception:
        raise Exception("Creating FAT32 filesystem on {0} failed".format(partdevice))
    else:
      raise Exception("Unknown filesystem type supplied")
  
    if bootcodeFile != None:
      bc = BootCodeInstaller()
      bc.install(partdevice, filesystemType, bootcodeFile)
  
  
  def mountFilesystem(self, partitionIndex):
    if self.__loopbackDevice == None:
      self.__createLoopbackDevice()
    
    partdevice = self.__getPartdeviceFromIndex(partitionIndex)
    print "-> Mounting filesystem on partition %s" % (partdevice)
    
    mountDest = tempfile.mkdtemp()
    
    try:
      subprocess.check_call(['hdiutil', 'mount', partdevice, '-mountpoint', mountDest])
    except Exception:
      raise Exception("Mounting filesystem on {0} failed".format(partdevice))
    
    return mountDest
  
  
  def __getPartdeviceFromIndex(self, partitionIndex):
    return self.__loopbackDevice + "s" + str(partitionIndex+1)



class PrimaryPartition:
  def __init__(self, startSector, endSector, type, active):
    self.startSector = startSector
    self.endSector = endSector
    self.type = type
    self.active = active



class MBR:
  #def __init__(self, existingMBR):
  #  self.__init__(self)
  
  def __init__(self, mbrCode = 512*"\0"):
    if len(mbrCode) > 512:
      raise ValueError("MBR code may not be bigger than 512 Byte")
    
    if len(mbrCode) < 512:
      mbrCode = mbrCode + ((512 - len(mbrCode)) * "\0")
    
    self.mbrCode = mbrCode
    self.partitionList = [None, None, None, None]
  
  
  def __calcCHSValue(self, cylinders, heads, sectors):
    byte1 = (cylinders & 0x300) >> 2 | sectors
    byte2 = cylinders & 0xFF
    return (heads, byte1, byte2)
  
  
  def getMBR(self):
    mbr = self.mbrCode[:446]
    partitionTable = ''
    
    index = 0
    for partition in self.partitionList:
      if partition == None:
        partitionTable = partitionTable + (16*'\x00')
        continue
        
      activeByte = 0
      if partition.active:
        activeByte = 0x80
      
      #TODO: calculate real CHS values
      if index == 0:
        (chs_start1, chs_start2, chs_start3) = self.__calcCHSValue(0, 1, 1)
      else:
        (chs_start1, chs_start2, chs_start3) = self.__calcCHSValue(1023, 254, 63)
      
      (chs_end1, chs_end2, chs_end3) = self.__calcCHSValue(1023, 254, 63)
      
      partitionTable = partitionTable + struct.pack("<BBBBBBBBII",
        activeByte,
        chs_start1, chs_start2, chs_start3,
        partition.type,
        chs_end1, chs_end2, chs_end3,
        partition.startSector, partition.endSector
      )
      
      index += 1
  
    mbr = mbr + partitionTable + "\x55\xAA"
    return mbr
  
  
  def setPartition(self, index, partition):
    if not isinstance(partition, PrimaryPartition):
      raise ValueError("Given partition object is not of type PrimaryPartition")
    
    if index < 0 or index > 4:
      raise ValueError("Index for partition out of bounds")
    
    self.partitionList[index] = partition



if __name__ == '__main__':
  main(sys.argv)
