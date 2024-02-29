//===================================================== file = mclient.c =====
//=  A multicast client to receive multicast datagrams                       =
//============================================================================
//=  Notes:                                                                  =
//=    1) This program receives on a multicast address and outputs the       =
//=       received buffer to the screen.                                     =
//=    2) Conditionally compiles for Winsock and BSD sockets by setting the  =
//=       initial #define to WIN or BSD as appropriate.                      =
//=    3) The multicast group address is GROUP_ADDR.                         =
//=--------------------------------------------------------------------------=
//=  Build: bcc32 mclient.c or cl mclient.c wsock32.lib for Winsock          =
//=         gcc mclient.c -lsocket -lnsl for BSD                             =
//=--------------------------------------------------------------------------=
//=  History:  JNS (07/11/02) - Genesis                                      =
//============================================================================
#define  BSD                // WIN for Winsock and BSD for BSD sockets

//----- Include files -------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <stdlib.h>         // Needed for memcpy()
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
#define PORT_NUM         4545             // Port number used
#define GROUP_ADDR "225.1.1.1"            // Address of the multicast group

//===== Main program ========================================================
void main(void)
{
#ifdef WIN
  WORD wVersionRequested = MAKEWORD(1,1); // Stuff for WSA functions
  WSADATA wsaData;                        // Stuff for WSA functions
#endif
  unsigned int         multi_server_sock; // Multicast socket descriptor
  struct ip_mreq       mreq;              // Multicast group structure
  struct sockaddr_in   client_addr;       // Client Internet address
  unsigned int         addr_len;          // Internet address length
  unsigned char        buffer[256];       // Datagram buffer
  
  //Helper buffers
  unsigned char        bufferTempSens[256];       
  unsigned char        bufferMoistureSens[256];       
  unsigned char        bufferLightSens[256];   
  unsigned char        bufferLightAct[256];    
  unsigned char        bufferTempAct[256];
  unsigned char        bufferMoistureAct[256];
  unsigned char        bufferUserApp[256];
  
  
  
  int                  retcode;           // Return code
  int   cnt = 0;
  int seconds_to_sleep = 1;

#ifdef WIN
  // This stuff initializes winsock
  WSAStartup(wVersionRequested, &wsaData);
#endif

  // Create a multicast socket and fill-in multicast address information
  multi_server_sock=socket(AF_INET, SOCK_DGRAM,0);
  mreq.imr_multiaddr.s_addr = inet_addr(GROUP_ADDR);
  mreq.imr_interface.s_addr = INADDR_ANY;

  // Create client address information and bind the multicast socket
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port = htons(PORT_NUM);
  retcode = bind(multi_server_sock,(struct sockaddr *)&client_addr,
                 sizeof(struct sockaddr));
  if (retcode < 0)
  {
    printf("*** ERROR - bind() failed with retcode = %d \n", retcode);
    return;
  }

  // Have the multicast socket join the multicast group
  retcode = setsockopt(multi_server_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
             (char*)&mreq, sizeof(mreq)) ;
  if (retcode < 0)
  {
    printf("*** ERROR - setsockopt() failed with retcode = %d \n", retcode);
    return;
  }

  // Set addr_len
  addr_len = sizeof(client_addr);

  while(1)
  {
  
  
  
  
  ////////////////////////////////////////////////////////////////////////////////////////
    // Receive a datagram from the multicast server
    if( (retcode = recvfrom(multi_server_sock, buffer, sizeof(buffer), 0,
      (struct sockaddr *)&client_addr, &addr_len)) < 0){
        printf("*** ERROR - recvfrom() failed \n");
        exit(1);
      }
    
    //////////////////////////////
    //Fill helper buffers to display all data
    
    //Temp sens buffer
    if(buffer[0] == 'T' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferTempSens[i] = buffer[i];
    	}

    //Temp act buffer
    if(buffer[0] == 'T' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferTempAct[i] = buffer[i];
      }
    
    //Moisture sens buffer
    if(buffer[0] == 'M' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferMoistureSens[i] = buffer[i];
    	}

    //Moisture act buffer
    if(buffer[0] == 'M' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferMoistureAct[i] = buffer[i];
      }
    	
    //Light sens buffer
    if(buffer[0] == 'L' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferLightSens[i] = buffer[i];
    	}  

    //Light act buffer
    if(buffer[0] == 'L' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferLightAct[i] = buffer[i];
      }
    
    //User App buffer
    if(buffer[0] == '1')
      for(int i = 0; i < 256; i++){
        bufferUserApp[i] = buffer[i];
      }

    
    
    	
    ////////////////////////////////////////////////////////////////////////////////////////
    //Case if they are empty
    if(bufferTempSens[0] != 'T' && bufferTempSens[1] != 'e')
    	sprintf(bufferTempSens, "Temperature Sensor(NOT ACTIVE)");

    if(bufferTempAct[0] != 'T' && bufferTempAct[1] != ' ')
      sprintf(bufferTempAct, "T Temp Actuator(NOT ACTIVE)");
    	
    if(bufferMoistureSens[0] != 'M' && bufferMoistureSens[1] != 'o')
    	sprintf(bufferMoistureSens, "Moisture Sensor(NOT ACTIVE)");

    if(bufferMoistureAct[0] != 'M' && bufferMoistureAct[1] != ' ')
      sprintf(bufferMoistureAct, "M Moisture Actuator(NOT ACTIVE)");
    	
    if(bufferLightSens[0] != 'L' && bufferLightSens[1] != 'i')
    	sprintf(bufferLightSens, "Light Sensor(NOT ACTIVE)");

    if(bufferLightAct[0] != 'L' && bufferLightAct[1] != ' ')
      sprintf(bufferLightAct, "L Ligh Actuator(NOT ACTIVE)");

    if(bufferUserApp[0] != '1')
      sprintf(bufferUserApp, "000");
    	
    system("clear"); //Clearing terminal
    // Print all buffers
    if(bufferUserApp[0] == '1'){
      printf("%s\n",bufferTempSens);
      printf("%s\n",bufferMoistureSens);
      printf("%s\n",bufferLightSens);
      printf("%s\n",bufferLightAct);
      printf("%s\n",bufferTempAct);
      printf("%s\n",bufferMoistureAct);
    }
    
    //Clear buffers
    /*memset(bufferTempSens, 0, sizeof bufferTempSens);
    memset(bufferMoistureSens, 0, sizeof bufferMoistureSens);
    memset(bufferLightSens, 0, sizeof bufferLightSens);*/

    //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
    // Receive a datagram from the multicast server
    if( (retcode = recvfrom(multi_server_sock, buffer, sizeof(buffer), 0,
      (struct sockaddr *)&client_addr, &addr_len)) < 0){
        printf("*** ERROR - recvfrom() failed \n");
        exit(1);
      }
    
    //////////////////////////////
    //Fill helper buffers to display all data
    
    //Temp sens buffer
    if(buffer[0] == 'T' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferTempSens[i] = buffer[i];
    	}

    //Temp act buffer
    if(buffer[0] == 'T' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferTempAct[i] = buffer[i];
      }
    
    //Moisture sens buffer
    if(buffer[0] == 'M' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferMoistureSens[i] = buffer[i];
    	}

    //Moisture act buffer
    if(buffer[0] == 'M' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferMoistureAct[i] = buffer[i];
      }
    	
    //Light sens buffer
    if(buffer[0] == 'L' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferLightSens[i] = buffer[i];
    	}  

    //Light act buffer
    if(buffer[0] == 'L' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferLightAct[i] = buffer[i];
      }

    //User App buffer
    if(buffer[0] == '1')
      for(int i = 0; i < 256; i++){
        bufferUserApp[i] = buffer[i];
      }

    
    
    	
    ////////////////////////////////////////////////////////////////////////////////////////
    //Case if they are empty
    if(bufferTempSens[0] != 'T' && bufferTempSens[1] != 'e')
    	sprintf(bufferTempSens, "Temperature Sensor(NOT ACTIVE)");

    if(bufferTempAct[0] != 'T' && bufferTempAct[1] != ' ')
      sprintf(bufferTempAct, "T Temp Actuator(NOT ACTIVE)");
    	
    if(bufferMoistureSens[0] != 'M' && bufferMoistureSens[1] != 'o')
    	sprintf(bufferMoistureSens, "Moisture Sensor(NOT ACTIVE)");

    if(bufferMoistureAct[0] != 'M' && bufferMoistureAct[1] != ' ')
      sprintf(bufferMoistureAct, "M Moisture Actuator(NOT ACTIVE)");
    	
    if(bufferLightSens[0] != 'L' && bufferLightSens[1] != 'i')
    	sprintf(bufferLightSens, "Light Sensor(NOT ACTIVE)");

    if(bufferLightAct[0] != 'L' && bufferLightAct[1] != ' ')
      sprintf(bufferLightAct, "L Ligh Actuator(NOT ACTIVE)");

    if(bufferUserApp[0] != '1')
      sprintf(bufferUserApp, "000");
    	
    system("clear"); //Clearing terminal
    // Print all buffers
    if(bufferUserApp[0] == '1'){
      printf("%s\n",bufferTempSens);
      printf("%s\n",bufferMoistureSens);
      printf("%s\n",bufferLightSens);
      printf("%s\n",bufferLightAct);
      printf("%s\n",bufferTempAct);
      printf("%s\n",bufferMoistureAct);
    }
    
    //Clear buffers
    /*memset(bufferTempSens, 0, sizeof bufferTempSens);
    memset(bufferMoistureSens, 0, sizeof bufferMoistureSens);
    memset(bufferLightSens, 0, sizeof bufferLightSens);*/

    //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
    // Receive a datagram from the multicast server
    if( (retcode = recvfrom(multi_server_sock, buffer, sizeof(buffer), 0,
      (struct sockaddr *)&client_addr, &addr_len)) < 0){
        printf("*** ERROR - recvfrom() failed \n");
        exit(1);
      }
    
    //////////////////////////////
    //Fill helper buffers to display all data
    
    //Temp sens buffer
    if(buffer[0] == 'T' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferTempSens[i] = buffer[i];
    	}

    //Temp act buffer
    if(buffer[0] == 'T' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferTempAct[i] = buffer[i];
      }
    
    //Moisture sens buffer
    if(buffer[0] == 'M' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferMoistureSens[i] = buffer[i];
    	}

    //Moisture act buffer
    if(buffer[0] == 'M' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferMoistureAct[i] = buffer[i];
      }
    	
    //Light sens buffer
    if(buffer[0] == 'L' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferLightSens[i] = buffer[i];
    	}  

    //Light act buffer
    if(buffer[0] == 'L' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferLightAct[i] = buffer[i];
      }

    //User App buffer
    if(buffer[0] == '1')
      for(int i = 0; i < 256; i++){
        bufferUserApp[i] = buffer[i];
      }
    
    	
    ////////////////////////////////////////////////////////////////////////////////////////
    //Case if they are empty
    if(bufferTempSens[0] != 'T' && bufferTempSens[1] != 'e')
    	sprintf(bufferTempSens, "Temperature Sensor(NOT ACTIVE)");

    if(bufferTempAct[0] != 'T' && bufferTempAct[1] != ' ')
      sprintf(bufferTempAct, "T Temp Actuator(NOT ACTIVE)");
    	
    if(bufferMoistureSens[0] != 'M' && bufferMoistureSens[1] != 'o')
    	sprintf(bufferMoistureSens, "Moisture Sensor(NOT ACTIVE)");

    if(bufferMoistureAct[0] != 'M' && bufferMoistureAct[1] != ' ')
      sprintf(bufferMoistureAct, "M Moisture Actuator(NOT ACTIVE)");
    	
    if(bufferLightSens[0] != 'L' && bufferLightSens[1] != 'i')
    	sprintf(bufferLightSens, "Light Sensor(NOT ACTIVE)");

    if(bufferLightAct[0] != 'L' && bufferLightAct[1] != ' ')
      sprintf(bufferLightAct, "L Ligh Actuator(NOT ACTIVE)");

    if(bufferUserApp[0] != '1')
      sprintf(bufferUserApp, "000");
    	
    system("clear"); //Clearing terminal
    // Print all buffers
    if(bufferUserApp[0] == '1'){
      printf("%s\n",bufferTempSens);
      printf("%s\n",bufferMoistureSens);
      printf("%s\n",bufferLightSens);
      printf("%s\n",bufferLightAct);
      printf("%s\n",bufferTempAct);
      printf("%s\n",bufferMoistureAct);
    }
    
    //Clear buffers
    /*memset(bufferTempSens, 0, sizeof bufferTempSens);
    memset(bufferMoistureSens, 0, sizeof bufferMoistureSens);
    memset(bufferLightSens, 0, sizeof bufferLightSens);*/

    //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
    // Receive a datagram from the multicast server
    if( (retcode = recvfrom(multi_server_sock, buffer, sizeof(buffer), 0,
      (struct sockaddr *)&client_addr, &addr_len)) < 0){
        printf("*** ERROR - recvfrom() failed \n");
        exit(1);
      }
    
    //////////////////////////////
    //Fill helper buffers to display all data
    
    //Temp sens buffer
    if(buffer[0] == 'T' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferTempSens[i] = buffer[i];
    	}

    //Temp act buffer
    if(buffer[0] == 'T' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferTempAct[i] = buffer[i];
      }
    
    //Moisture sens buffer
    if(buffer[0] == 'M' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferMoistureSens[i] = buffer[i];
    	}

    //Moisture act buffer
    if(buffer[0] == 'M' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferMoistureAct[i] = buffer[i];
      }
    	
    //Light sens buffer
    if(buffer[0] == 'L' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferLightSens[i] = buffer[i];
    	}  

    //Light act buffer
    if(buffer[0] == 'L' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferLightAct[i] = buffer[i];
      }

    //User App buffer
    if(buffer[0] == '1')
      for(int i = 0; i < 256; i++){
        bufferUserApp[i] = buffer[i];
      }
    
    	
    ////////////////////////////////////////////////////////////////////////////////////////
    //Case if they are empty
    if(bufferTempSens[0] != 'T' && bufferTempSens[1] != 'e')
    	sprintf(bufferTempSens, "Temperature Sensor(NOT ACTIVE)");

    if(bufferTempAct[0] != 'T' && bufferTempAct[1] != ' ')
      sprintf(bufferTempAct, "T Temp Actuator(NOT ACTIVE)");
    	
    if(bufferMoistureSens[0] != 'M' && bufferMoistureSens[1] != 'o')
    	sprintf(bufferMoistureSens, "Moisture Sensor(NOT ACTIVE)");

    if(bufferMoistureAct[0] != 'M' && bufferMoistureAct[1] != ' ')
      sprintf(bufferMoistureAct, "M Moisture Actuator(NOT ACTIVE)");
    	
    if(bufferLightSens[0] != 'L' && bufferLightSens[1] != 'i')
    	sprintf(bufferLightSens, "Light Sensor(NOT ACTIVE)");

    if(bufferLightAct[0] != 'L' && bufferLightAct[1] != ' ')
      sprintf(bufferLightAct, "L Ligh Actuator(NOT ACTIVE)");

    if(bufferUserApp[0] != '1')
      sprintf(bufferUserApp, "000");
    	
    system("clear"); //Clearing terminal
    // Print all buffers
    if(bufferUserApp[0] == '1'){
      printf("%s\n",bufferTempSens);
      printf("%s\n",bufferMoistureSens);
      printf("%s\n",bufferLightSens);
      printf("%s\n",bufferLightAct);
      printf("%s\n",bufferTempAct);
      printf("%s\n",bufferMoistureAct);
    }
    
    //Clear buffers
    /*memset(bufferTempSens, 0, sizeof bufferTempSens);
    memset(bufferMoistureSens, 0, sizeof bufferMoistureSens);
    memset(bufferLightSens, 0, sizeof bufferLightSens);*/

    //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
    // Receive a datagram from the multicast server
    if( (retcode = recvfrom(multi_server_sock, buffer, sizeof(buffer), 0,
      (struct sockaddr *)&client_addr, &addr_len)) < 0){
        printf("*** ERROR - recvfrom() failed \n");
        exit(1);
      }
    
    //////////////////////////////
    //Fill helper buffers to display all data
    
    //Temp sens buffer
    if(buffer[0] == 'T' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferTempSens[i] = buffer[i];
    	}

    //Temp act buffer
    if(buffer[0] == 'T' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferTempAct[i] = buffer[i];
      }
    
    //Moisture sens buffer
    if(buffer[0] == 'M' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferMoistureSens[i] = buffer[i];
    	}

    //Moisture act buffer
    if(buffer[0] == 'M' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferMoistureAct[i] = buffer[i];
      }
    	
    //Light sens buffer
    if(buffer[0] == 'L' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferLightSens[i] = buffer[i];
    	}  

    //Light act buffer
    if(buffer[0] == 'L' && buffer[1] == ' '){
      for(int i = 0; i < 256; i++){
        bufferLightAct[i] = buffer[i];
      }
    }

    //User App buffer
    if(buffer[0] == '1')
      for(int i = 0; i < 256; i++){
        bufferUserApp[i] = buffer[i];
      }
    	
    ////////////////////////////////////////////////////////////////////////////////////////
    //Case if they are empty
    if(bufferTempSens[0] != 'T' && bufferTempSens[1] != 'e')
    	sprintf(bufferTempSens, "Temperature Sensor(NOT ACTIVE)");

    if(bufferTempAct[0] != 'T' && bufferTempAct[1] != ' ')
      sprintf(bufferTempAct, "T Temp Actuator(NOT ACTIVE)");
    	
    if(bufferMoistureSens[0] != 'M' && bufferMoistureSens[1] != 'o')
    	sprintf(bufferMoistureSens, "Moisture Sensor(NOT ACTIVE)");

    if(bufferMoistureAct[0] != 'M' && bufferMoistureAct[1] != ' ')
      sprintf(bufferMoistureAct, "M Moisture Actuator(NOT ACTIVE)");
    	
    if(bufferLightSens[0] != 'L' && bufferLightSens[1] != 'i')
    	sprintf(bufferLightSens, "Light Sensor(NOT ACTIVE)");

    if(bufferLightAct[0] != 'L' && bufferLightAct[1] != ' ')
      sprintf(bufferLightAct, "L Ligh Actuator(NOT ACTIVE)");

    if(bufferUserApp[0] != '1')
      sprintf(bufferUserApp, "000");
    	
    system("clear"); //Clearing terminal
    // Print all buffers
    if(bufferUserApp[0] == '1'){
      printf("%s\n",bufferTempSens);
      printf("%s\n",bufferMoistureSens);
      printf("%s\n",bufferLightSens);
      printf("%s\n",bufferLightAct);
      printf("%s\n",bufferTempAct);
      printf("%s\n",bufferMoistureAct);
    }
    
    //Clear buffers
    /*memset(bufferTempSens, 0, sizeof bufferTempSens);
    memset(bufferMoistureSens, 0, sizeof bufferMoistureSens);
    memset(bufferLightSens, 0, sizeof bufferLightSens);*/
    
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////
    //////////////////////////////
    // Receive a datagram from the multicast server
    if( (retcode = recvfrom(multi_server_sock, buffer, sizeof(buffer), 0,
      (struct sockaddr *)&client_addr, &addr_len)) < 0){
        printf("*** ERROR - recvfrom() failed \n");
        exit(1);
      }
    
    //////////////////////////////
    //Fill helper buffers to display all data
    
    //Temp sens buffer
    if(buffer[0] == 'T' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferTempSens[i] = buffer[i];
    	}

    //Temp act buffer
    if(buffer[0] == 'T' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferTempAct[i] = buffer[i];
      }
    
    //Moisture sens buffer
    if(buffer[0] == 'M' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferMoistureSens[i] = buffer[i];
    	}

    //Moisture act buffer
    if(buffer[0] == 'M' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferMoistureAct[i] = buffer[i];
      }
    	
    //Light sens buffer
    if(buffer[0] == 'L' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferLightSens[i] = buffer[i];
    	}

      //Light act buffer
    if(buffer[0] == 'L' && buffer[1] == ' '){
      for(int i = 0; i < 256; i++){
        bufferLightAct[i] = buffer[i];
      }
    }

    //User App buffer
    if(buffer[0] == '1')
      for(int i = 0; i < 256; i++){
        bufferUserApp[i] = buffer[i];
      }
    	
    ////////////////////////////////////////////////////////////////////////////////////////
    //Case if they are empty
    if(bufferTempSens[0] != 'T' && bufferTempSens[1] != 'e')
    	sprintf(bufferTempSens, "Temperature Sensor(NOT ACTIVE)");

    if(bufferTempAct[0] != 'T' && bufferTempAct[1] != ' ')
      sprintf(bufferTempAct, "T Temp Actuator(NOT ACTIVE)");
    	
    if(bufferMoistureSens[0] != 'M' && bufferMoistureSens[1] != 'o')
    	sprintf(bufferMoistureSens, "Moisture Sensor(NOT ACTIVE)");

    if(bufferMoistureAct[0] != 'M' && bufferMoistureAct[1] != ' ')
      sprintf(bufferMoistureAct, "M Moisture Actuator(NOT ACTIVE)");
    	
    if(bufferLightSens[0] != 'L' && bufferLightSens[1] != 'i')
    	sprintf(bufferLightSens, "Light sensor(NOT ACTIVE)");

    if(bufferLightAct[0] != 'L' && bufferLightAct[1] != ' ')
      sprintf(bufferLightAct, "L Ligh Actuator(NOT ACTIVE)");

    if(bufferUserApp[0] != '1')
      sprintf(bufferUserApp, "000");
    	
    // Print all buffers
    system("clear"); //Clearing terminal
    if(bufferUserApp[0] == '1'){
      printf("%s\n",bufferTempSens);
      printf("%s\n",bufferMoistureSens);
      printf("%s\n",bufferLightSens);
      printf("%s\n",bufferLightAct);
      printf("%s\n",bufferTempAct);
      printf("%s\n",bufferMoistureAct);
    }

    //Clear buffers
   /* memset(bufferTempSens, 0, sizeof bufferTempSens);
    memset(bufferMoistureSens, 0, sizeof bufferMoistureSens);
    memset(bufferLightSens, 0, sizeof bufferLightSens);*/
    
  
    /////////////////////////////////////////////////////////////////////////////////////////
    	
    /////////////////////////////////
    /////////////////////////////////
    // Receive a datagram from the multicast server
    if( (retcode = recvfrom(multi_server_sock, buffer, sizeof(buffer), 0,
      (struct sockaddr *)&client_addr, &addr_len)) < 0){
        printf("*** ERROR - recvfrom() failed \n");
        exit(1);
      }
    
    //////////////////////////////
    //Fill helper buffers to display all data
    
    //Temp sens buffer
    if(buffer[0] == 'T' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferTempSens[i] = buffer[i];
    	}

    //Temp act buffer
    if(buffer[0] == 'T' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferTempAct[i] = buffer[i];
      }
    
    //Moisture sens buffer
    if(buffer[0] == 'M' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferMoistureSens[i] = buffer[i];
    	}

    //Moisture act buffer
    if(buffer[0] == 'M' && buffer[1] == ' ')
      for(int i = 0; i < 256; i++){
        bufferMoistureAct[i] = buffer[i];
      }
    	
    //Light sens buffer
    if(buffer[0] == 'L' && buffer[1] != ' ')
    	for(int i = 0; i < 256; i++){
    		bufferLightSens[i] = buffer[i];
    	}

      //Light act buffer
    if(buffer[0] == 'L' && buffer[1] == ' '){
      for(int i = 0; i < 256; i++){
        bufferLightAct[i] = buffer[i];
      }
    }

    //User App buffer
    if(buffer[0] == '1')
      for(int i = 0; i < 256; i++){
        bufferUserApp[i] = buffer[i];
      }
    	
    ////////////////////////////////////////////////////////////////////////////////////////
    //Case if they are empty
    if(bufferTempSens[0] != 'T' && bufferTempSens[1] != 'e')
    	sprintf(bufferTempSens, "Temperature Sensor(NOT ACTIVE)");

    if(bufferTempAct[0] != 'T' && bufferTempAct[1] != ' ')
      sprintf(bufferTempAct, "T Temp Actuator(NOT ACTIVE)");
    	
    if(bufferMoistureSens[0] != 'M' && bufferMoistureSens[1] != 'o')
    	sprintf(bufferMoistureSens, "Moisture Sensor(NOT ACTIVE)");

    if(bufferMoistureAct[0] != 'M' && bufferMoistureAct[1] != ' ')
      sprintf(bufferMoistureAct, "M Moisture Actuator(NOT ACTIVE)");
    	
    if(bufferLightSens[0] != 'L' && bufferLightSens[1] != 'i')
    	sprintf(bufferLightSens, "Light sensor(NOT ACTIVE)");

    if(bufferLightAct[0] != 'L' && bufferLightAct[1] != ' ')
      sprintf(bufferLightAct, "L Ligh Actuator(NOT ACTIVE)");

    if(bufferUserApp[0] != '1')
      sprintf(bufferUserApp, "000");
    	
    // Print all buffers
    system("clear"); //Clearing terminal
    if(bufferUserApp[0] == '1'){
      printf("%s\n",bufferTempSens);
      printf("%s\n",bufferMoistureSens);
      printf("%s\n",bufferLightSens);
      printf("%s\n",bufferLightAct);
      printf("%s\n",bufferTempAct);
      printf("%s\n",bufferMoistureAct);
    }
    
    //Clear buffers
    memset(bufferTempSens, 0, sizeof bufferTempSens);
    memset(bufferMoistureSens, 0, sizeof bufferMoistureSens);
    memset(bufferLightSens, 0, sizeof bufferLightSens);
    memset(bufferLightAct, 0, sizeof bufferLightAct);
    memset(bufferTempAct, 0, sizeof bufferTempAct);
    memset(bufferMoistureAct, 0, sizeof bufferMoistureAct);
    memset(bufferUserApp, 0, sizeof bufferUserApp);
    /////////////////////////////////////////////////////////////////////////////////////////
    
    sleep(1);
    system("clear"); //Clearing terminal
    
  }

  // Close and clean-up
#ifdef WIN
  closesocket(multi_server_sock);
  WSACleanup();
#endif
#ifdef BSD
  close(multi_server_sock);
#endif
}
