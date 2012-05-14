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
#include <netinet/in_systm.h> //tipos de dados

//#include "LibraryC.h"

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


int main(int argc,char *argv[])
{
	/* Test for correct number of arguments */
    if ((argc < 2) || (argc > 3))
    {
       fprintf(stderr, "Usage: %s <Interface>\n",argv[0]);
       exit(1);
    }

	sockd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));

	if(sockd < 0)
	{
		perror("socket");
		exit(-1);
	}

	//printf("%d \n", sockd);

	// O procedimento abaixo é utilizado para "setar" a
	// interface em modo promíscuo

	strcpy(ifr.ifr_name, argv[1]);
	if(ioctl(sockd, SIOCGIFINDEX, &ifr) < 0) {printf("erro no ioctl!"); }
	ioctl(sockd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sockd, SIOCSIFFLAGS, &ifr);

	// recepção de pacotes
	printf(":: Capturando pacotes ::\n");
	while (1)
	{
	    int i = 0;
        recv(sockd,(char*)&buff1,sizeof(buff1),0x0);

		//printf("  Size:%d\n",sizeOfCharArray(buff1));


		// Protocolo Ethernet
		printf("-------------  ==   == -------------\n");
		printf("       MAC Source: %02x:%02x:%02x:%02x:%02x:%02x \n", buff1[6],buff1[7],buff1[8],buff1[9],buff1[10],buff1[11]);
		printf("  MAC Destination: %02x:%02x:%02x:%02x:%02x:%02x \n", buff1[0],buff1[1],buff1[2],buff1[3],buff1[4],buff1[5]);
		printf("             Type: %02x:%02x \n", buff1[12],buff1[13]);

		// Protocolo IPv4
		if((buff1[12] == 8) && (buff1[13] == 0))
		{
		    if (buff1[14] == 69)  printf("       Version IP: 4\n");
		    else printf("       Version IP: %02x\n", buff1[14]);
			//printf("              DSF: %02x\n", buff1[15]);
			printf("     Total Lenght: %02x%02x\n", buff1[16], buff1[17]);
			//printf("               ID: %02x%02x\n", buff1[18], buff1[19]);
			//printf("             Flag: %02x%02x\n", buff1[20], buff1[21]);
			//printf("              TTL: %02x\n", buff1[22]);
			//printf("        Protocolo: %02x\n", buff1[23]);
			//printf("         Ckecksum: %02x%02x\n", buff1[24], buff1[25]);
			printf("        IP Source: %d.%d.%d.%d\n", buff1[26], buff1[27], buff1[28], buff1[29]);
			printf("          IP Dest: %d.%d.%d.%d\n", buff1[30], buff1[31], buff1[32], buff1[33]);

			if (buff1[34] == 8) { printf("         Comando: PING\n");  } // PING

		}
		else
		{
		    for(i=14;i<34;i++)
			{
			   printf("%x",buff1[i]);
			}
			printf("\n");
		}

	}

}
