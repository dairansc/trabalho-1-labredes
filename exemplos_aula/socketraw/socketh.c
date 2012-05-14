/*-------------------------------------------------------------*/
/* Exemplo Socket Raw                                          */
/*-------------------------------------------------------------*/

/* Includes */

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

  int sockd;
  int on;
  struct ifreq ifr;

int main(int argc,char *argv[])
{

	sockd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(sockd < 0) {
		perror("socket");
		exit(-1);
	}
	printf("%d \n", sockd);

	// O procedimento abaixo � utilizado para "setar" a 
	// interface em modo prom�scuo
      
	strcpy(ifr.ifr_name, "eth0");
	if(ioctl(sockd, SIOCGIFINDEX, &ifr) < 0)
		printf("erro no ioctl!");
	ioctl(sockd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sockd, SIOCSIFFLAGS, &ifr); 

	// recep��o de pacotes
	printf("antes do while \n");
	while (1) {
      		recv(sockd,(char *) &buff1,sizeof(buff1),0x0);

		// impress�o do conte�do - exemplo
		printf("%x : %x: \n", buff1[0],buff1[1]);
	}
}
