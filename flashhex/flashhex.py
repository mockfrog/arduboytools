#!/usr/bin/python3
import sys
import argparse
import serial
from serial.tools import list_ports
from time import sleep
from subprocess import call

def startbootloader():
    # connect and disconnect with 1200 baud to trigger reset into bootloader
    for port in list_ports.comports():
        if port.vid == 9025 and port.pid == 32822:
            print ("Starting bootloader for " + port.product + " at " + port.device)
            ser = serial.Serial(port.device, 1200)
            ser.dtr = False
            ser.close()
            return True
    return False 

def flash():
    # check if the bootloader is up
    for port in list_ports.comports():
        if port.vid == 9025 and port.pid == 54:
            print ("Calling avrdude...")
            exit(call(["avrdude", "-patmega32u4", "-cavr109", "-P" + port.device, "-b19200", "-e", "-D", "-V", "-Uflash:w:" + args.infile[0].name +":i"] ))
    print ("Bootloader could not be detected!")
    exit (4)


# STARTS HERE

# parse command line
parser = argparse.ArgumentParser(description='Flash hex images onto the Arduboy')
parser.add_argument('infile', nargs=1, type=argparse.FileType('r'), default=None)
args = parser.parse_args()

if startbootloader():
    # wait 2 seconds for the bootloader to start
    sleep(2)
    flash()
else:
    print ("Please connect your Arduboy, turn it on and try again");
    exit (3)

