// Trabalho 1 - LabRedes 2012 - Bruno e Dairan


// http://www.mutekh.org/www/mutekh_api/Headers_documentation.html

/* Includes */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

/* Diretórios: net, netinet, linux contém os includes que descrevem */
/* as estruturas de dados do header dos protocolos   		    */
/* Cuidado: se uitlizar netinet não uitlize o diretório linux e     */
/* vice-versa						            */

#include <net/if.h>  //estrutura ifr
#include <netinet/ether.h> //header ethernet
#include <netinet/in.h> //definição de protocolos
#include <arpa/inet.h> //funções para manipulação de endereços IP
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in_systm.h> //tipos de dados

#include "slist.c"

#if !defined(IPVERSION)
#define IPVERSION 4
#endif

#define BUFFSIZE 1500

// Atencao!! Confira no /usr/include do seu sisop o nome correto
// das estruturas de dados dos protocolos.

  unsigned char buff1[BUFFSIZE]; // buffer de recepcao

  int sockd;
  int on;
  struct ifreq ifr;

typedef struct maquinas {
    uint8_t mac[6];
    uint32_t ip;

} MAQUINAS;

int main(int argc,char *argv[])
{
    int i;
	struct ether_header *etherhdr; // struct de Ethernet
	struct arphdr *arphdrptr; // struct ARP
	struct iphdr *iphdrptr; // struct IP
	struct icmphdr *icmphdrptr;

	struct in_addr source, dest;

	NODE *lista;
	lista=list_create((void*)"");

	sockd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(sockd < 0) {
		perror("socket");
		exit(-1);
	}
	printf("%d \n", sockd);

	// O procedimento abaixo é utilizado para "setar" a
	// interface em modo promíscuo

	strcpy(ifr.ifr_name, "eth0");
	if(ioctl(sockd, SIOCGIFINDEX, &ifr) < 0)
		printf("erro no ioctl!");
	ioctl(sockd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sockd, SIOCSIFFLAGS, &ifr);

	// recepção de pacotes
	printf(":: Capturando pacotes ::\n");
	while (1) {
		recv(sockd,(char *) &buff1,sizeof(buff1),0x0);

		// Protocolo Ethernet
		etherhdr = (struct ether_header *)buff1;



		if(ntohs(etherhdr->ether_type) == ETHERTYPE_IP){



			if (iphdrptr->protocol == 0x01) {
				icmphdrptr = (struct icmphdr *)(buff1+sizeof(struct ether_header)+iphdrptr->ihl*4);
				if(icmphdrptr->type == ICMP_ECHO){
                    printf("-------------  ==   == -------------\n");
                    printf("       MAC Source: ");
                    for(i=0; i<6; i++){
                        printf("%02X:", etherhdr->ether_shost[i]);
                    }
                    printf("\n");
                    printf("  MAC Destination: ");

                    for(i=0; i<6; i++){
                        printf("%02X:", etherhdr->ether_dhost[i]);
                    }
                    printf("\n             Type: %04X\n", ntohs(etherhdr->ether_type));


                    printf("IP\n");
                    iphdrptr = (struct iphdr *)(buff1+sizeof(struct ether_header));
                    memcpy(&source, &iphdrptr->saddr, 4);
                    memcpy(&dest, &iphdrptr->daddr, 4);
                    printf("        IP Source: %s\n", inet_ntoa(source));
                    printf("   IP Destination: %s\n", inet_ntoa(dest));

                    printf("ICMP\n");
                    //printf("\n               ID: %04X\n", ntohs(icmphdrptr->un & 0x0000FFFF);
                    //printf("\n  Sequence number: %04X\n", ntohs((icmphdrptr->un & 0xFFFF0000) >> 16);
                }
			}
		}/*
		else if(ntohs(etherhdr->ether_type) == ETHERTYPE_ARP) {
			printf("ARP\n");
			arphdrptr = (struct arphdr *)(buff1+sizeof(struct ether_header));
		}*/
	}
}
