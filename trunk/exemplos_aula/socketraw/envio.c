/*-------------------------------------------------------------*/
/* Exemplo Socket Raw - envio de mensagens                     */
/*-------------------------------------------------------------*/

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/ether.h>

#define ETHERTYPE_LEN 2
#define MAC_ADDR_LEN 6
#define BUFFER_LEN 1518

// Atencao!! Confira no /usr/include do seu sisop o nome correto 
// das estruturas de dados dos protocolos. 

typedef unsigned char MacAddress[MAC_ADDR_LEN];
extern int errno;

int main()
{
  int i, sockFd = 0, retValue = 0;
  char buffer[BUFFER_LEN], dummyBuf[50];
  struct sockaddr_ll destAddr;
  short int etherTypeT = htons(0x8200);
  
  /* Configura MAC Origem e Destino */
  MacAddress localMac = {0x00, 0x0B, 0xCD, 0xA8, 0x6D, 0x91};
  MacAddress destMac = {0x00, 0x17, 0x9A, 0xB3, 0x9E, 0x16};

  memset(&destAddr, 0, sizeof(struct sockaddr_ll));
  memset(buffer, 0, BUFFER_LEN);

  if((sockFd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
    printf("ERROR! socket() \n");
    exit(1);
  }
  //printf("Socket creation success.\n");

  /* Configura padrao do endereço destino */
  destAddr.sll_family = htons(PF_PACKET);
  destAddr.sll_protocol = htons(ETH_P_ALL);
  destAddr.sll_halen = 6;
  destAddr.sll_ifindex = 2;
  memcpy(&(destAddr.sll_addr), destMac, MAC_ADDR_LEN);

  /* Configura cabecalho Ethernet */
  memcpy(buffer, localMac, MAC_ADDR_LEN);
  memcpy((buffer+MAC_ADDR_LEN), destMac, MAC_ADDR_LEN);
  memcpy((buffer+(2*MAC_ADDR_LEN)), &(etherTypeT), sizeof(etherTypeT));

  /* Adiciona alguns dados */
  memset(dummyBuf, 0xa0, sizeof(dummyBuf));
  memcpy((buffer+ETHERTYPE_LEN+(2*MAC_ADDR_LEN)), dummyBuf, 50);

  i=0;
  while(i<10) {
    if((retValue = sendto(sockFd, buffer, 64, 0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0) {
       printf("ERROR! sendto() \n");
       exit(1);
    }
    printf("Send success (%d).\n", retValue);
    i++;
  }
}
