// Trabalho 1 - LabRedes 2012 - Bruno e Dairan

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <unistd.h>

/* Diret�rios: net, netinet, linux cont�m os includes que descrevem */
/* as estruturas de dados do header dos protocolos   		    */
/* Cuidado: se uitlizar netinet n�o uitlize o diret�rio linux e     */
/* vice-versa						            */

#include <net/if.h>  //estrutura ifr
#include <netinet/ether.h> //header ethernet
#include <netinet/in.h> //defini��o de protocolos
#include <arpa/inet.h> //fun��es para manipula��o de endere�os IP
#include <netinet/in_systm.h> //tipos de dados 

#if !defined(IPVERSION)
#define IPVERSION 4
#endif

#define BUFFSIZE 1500

// Atencao!! Confira no /usr/include do seu sisop o nome correto 
// das estruturas de dados dos protocolos. 

  unsigned char buff1[BUFFSIZE]; // buffer de recepcao
  struct ifreq ifr; // ?????
  struct ether_header *etherhdr; // struct de Ethernet
  struct arphdr *arphdrptr; // struct ARP
  struct iphdr *iphdrptr; // struct IP
  int sockd; // socket
  int on; // ?????


int main(int argc,char *argv[])
{
	// Test o numero de argumentos
    if ((argc < 2) || (argc > 3))    
    {
       fprintf(stderr, "Usage: %s <Interface>\n",argv[0]);
       exit(1);
    }

	// Cria o socket
	sockd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(sockd < 0) {	perror("socket"); exit(-1);	}

	// O procedimento abaixo � utilizado para "setar" a interface em modo prom�scuo
	strcpy(ifr.ifr_name, argv[1]);
	if(ioctl(sockd, SIOCGIFINDEX, &ifr) < 0) {printf("erro no ioctl!"); }
	ioctl(sockd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sockd, SIOCSIFFLAGS, &ifr); 

	// recep��o de pacotes
	printf(":: Capturando pacotes ::\n");
	while (1) 
	{
	    int i = 0;
        recv(sockd,(char*)&buff1,sizeof(buff1),0x0);

		//Define um ponteiro para o ether_header
		etherhdr = (struct ether_header *)buff1;

		//Capturando pacotes IP :: Os tipos de ether_type podem ser vistos em ethernet.h
		if(ntohs(etherhdr->ether_type) == ETHERTYPE_IP)
		{
		    iphdrptr = (struct iphdr *)(buff1 + sizeof(etherhdr)); // converte buff em ip header
		     
		    printf("pacote IP: ");
			printf("%d\n", iphdrptr->saddr);

			//printf("%x\n", etherhdr->ether_dhost[0]);
		}
		// Capturando pacotes ARP
		else if (ntohs(etherhdr->ether_type) == ETHERTYPE_ARP)
		{
		   arphdrptr = (struct arphdr *)(buff1 + sizeof(etherhdr)); // converte buff em arp header

		   if ((arphdrptr->ar_op) == ARPOP_REQUEST)
			{
			   		printf("pacote ARP REQUEST: ");
					printf("tam mac: %d\n", arphdrptr->ar_hln);
					printf("tam ip: %d\n", arphdrptr->ar_pln);
			}
		   else if ((arphdrptr->ar_op) == ARPOP_REPLY)
		   {
			   		   printf("pacote ARP REPLY: ");
					   printf("tam mac: %d\n", arphdrptr->ar_hln);
					   printf("tam ip: %d\n", arphdrptr->ar_pln);
		   }
		   else 
		   {
				 printf("pacote ARP [?]: ");
					   printf("tam mac: %d\n", arphdrptr->ar_hln);
					   printf("tam ip: %d\n", arphdrptr->ar_pln);
			}
		}



			// Protocolo Ethernet
			//printf("-------------  ==   == -------------\n");
			//printf("       MAC Source: %02x:%02x:%02x:%02x:%02x:%02x \n", buff1[6],buff1[7],buff1[8],buff1[9],buff1[10],buff1[11]);
			//printf("  MAC Destination: %02x:%02x:%02x:%02x:%02x:%02x \n", buff1[0],buff1[1],buff1[2],buff1[3],buff1[4],buff1[5]);
			//printf("             Type: %02x:%02x \n", buff1[12],buff1[13]);

			// Protocolo IPv4
			//if((buff1[12] == 8) && (buff1[13] == 0))
			//{
				//if (buff1[14] == 69)  printf("       Version IP: 4\n", buff1[14]);
				//else printf("       Version IP: %02x\n", buff1[14]);
				//printf("              DSF: %02x\n", buff1[15]);
				//printf("     Total Lenght: %02x%02x\n", buff1[16], buff1[17]);
				//printf("               ID: %02x%02x\n", buff1[18], buff1[19]);
				//printf("             Flag: %02x%02x\n", buff1[20], buff1[21]);
				//printf("              TTL: %02x\n", buff1[22]);
				//printf("        Protocolo: %02x\n", buff1[23]);
				//printf("         Ckecksum: %02x%02x\n", buff1[24], buff1[25]);
				//printf("        IP Source: %d.%d.%d.%d\n", buff1[26], buff1[27], buff1[28], buff1[29]);
				//printf("          IP Dest: %d.%d.%d.%d\n", buff1[30], buff1[31], buff1[32], buff1[33]);

				//if (buff1[34] == 8) { printf("         Comando: PING");  } // PING

			//}
			//else
			//{
				//for(i=14;i<34;i++)
				//{
				   //printf("%x",buff1[i]);
				//}
				//printf("\n");
			//}
		


	}

}