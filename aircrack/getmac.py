from uuid import getnode as get_mac
import fcntl, socket, struct

def getHwAddr(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    info = fcntl.ioctl(s.fileno(), 0x8927,  struct.pack('256s', ifname[:15]))
    return ':'.join(['%02x' % ord(char) for char in info[18:24]])

print getHwAddr('wlan0')

mac = get_mac()

mac1 = hex(mac)

mac2 = ':'.join(("%012X" % mac)[i:i+2] for i in range(0, 12, 2))

print mac1

print mac2

