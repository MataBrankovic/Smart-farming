///////////////////////////////////////////////////////////////////////

//                         LIGHT SENSOR #1          		       //

///////////////////////////////////////////////////////////////////////






#define  BSD                // WIN for Winsock and BSD for BSD sockets

//----- Include files -------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <stdlib.h>         // Needed for memcpy() and itoa()
#ifdef WIN
  #include <winsock.h>      // Needed for all Windows stuff
#endif
#ifdef BSD
  #include <sys/types.h>    // Needed for system defined identifiers.
  #include <netinet/in.h>   // Needed for internet address structure.
  #include <sys/socket.h>   // Needed for socket(), bind(), etc...
  #include <arpa/inet.h>    // Needed for inet_ntoa()
  #include <fcntl.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <string.h>
  

 
  
  
#endif

//----- Defines -------------------------------------------------------------
#define PORT_NUM         4446             // Port number used
#define PORT_CONTROLER   4545
#define GROUP_ADDR "225.1.1.1"            // Address of the multicast group

//===== Main program ========================================================
void main(void)
{
#ifdef WIN
  WORD wVersionRequested = MAKEWORD(1,1);       // Stuff for WSA functions
  WSADATA wsaData;                              // Stuff for WSA functions
#endif

  //SENDING
  unsigned char	sending[5];
  unsigned char	firstBit = '1';
  unsigned char	secondBit = '0';
  unsigned char	thirdBit = '0';
  unsigned char	fourthBit = '0';
  unsigned char	fifthBit = '1';
  //


  unsigned int         server_s;                // Server socket descriptor
  unsigned int         multi_server_sock;       // Multicast socket descriptor
  unsigned int		multi_server_sock1;
  struct sockaddr_in   addr_dest;               // Multicast group address
  struct sockaddr_in   addr_dest1;
  struct ip_mreq       mreq;                    // Multicast group descriptor
  unsigned char        TTL;                     // TTL for multicast packets
  struct in_addr       recv_ip_addr;            // Receive IP address
  unsigned int         addr_len;                // Internet address length
  unsigned char        buffer[256];             // Datagram buffer
  unsigned char	buffer_to_controler[256];
  int                  count;                   // Loop counter
  int                  retcode;                 // Return code

#ifdef WIN
  // This stuff initializes winsock
  WSAStartup(wVersionRequested, &wsaData);
#endif

  // Create a multicast socket
 
  multi_server_sock1=socket(AF_INET, SOCK_DGRAM,0);

  // Create multicast group address information

  
  addr_dest1.sin_family = AF_INET;
  addr_dest1.sin_addr.s_addr = inet_addr(GROUP_ADDR);
  addr_dest1.sin_port = htons(PORT_CONTROLER);
  
  // Set the TTL for the sends using a setsockopt()
  TTL = 1;
  
  ////
  retcode = setsockopt(multi_server_sock1, IPPROTO_IP, IP_MULTICAST_TTL,
                       (char *)&TTL, sizeof(TTL));
  if (retcode < 0)
  {
    printf("*** ERROR - setsockopt() failed with retcode = %d \n", retcode);
    return;
  }

  // Set addr_len
  addr_len = sizeof(addr_dest1);
  
  //SENDING
  sending[0] = firstBit;
  sending[1] = secondBit;
  sending[2] = thirdBit;
  sending[3] = fourthBit;
  sending[4] = fifthBit;

  // Multicast the message forever with a period of 1 second
  count = 0;
  printf("*** Sending multicast datagrams to '%s' (port = %d) \n",
    GROUP_ADDR, PORT_NUM);

    char x;
    printf("UNESITE 1 DA BI VIDELI STANJE SISTEMA: ");
    scanf("%c", &x);

  while(1)
  {
    
    // Build the message in the buffer
    sprintf(buffer_to_controler, "%c ASASASA", x);
    
    
           
    sendto(multi_server_sock1, buffer_to_controler, sizeof(buffer_to_controler), 0,
           (struct sockaddr*)&addr_dest1, addr_len);
           
   
    memset(buffer_to_controler, 0, sizeof buffer_to_controler);
    
#ifdef WIN
    Sleep(1000);
#endif
#ifdef BSD
    sleep(1);
#endif
  }

  // Close and clean-up
#ifdef WIN
  closesocket(multi_server_sock);
  WSACleanup();
#endif
#ifdef BSD
  close(multi_server_sock1);
#endif
}

