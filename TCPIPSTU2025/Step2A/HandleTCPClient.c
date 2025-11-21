#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>
#include "Requete.h" 
#include "data.h" 
#include "LibSerHV.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */


void HandleTCPClient(int clntSocket)
{
    int recvMsgSize, sentBytes;                    /* Size of received message */
    struct Requete UneRequete ;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, &UneRequete, sizeof (struct Requete), 0)) < 0)
        DieWithError("recv() failed");
    while(recvMsgSize>0) 
    {   
        AfficheRequete(UneRequete) ; 
        printf("Bytes lus : %d\n", recvMsgSize);  
    /* Echo message back to client */
    if ((sentBytes = write(clntSocket, &UneRequete, recvMsgSize)) != recvMsgSize)
            DieWithError("send() failed");
    else
            printf("Bytes écrits : %d\n", sentBytes);
    
    /* See if there is more data to receive */
    if ((recvMsgSize = read(clntSocket, &UneRequete, sizeof (struct Requete))) < 0)
            DieWithError("recv() failed");
    }
    printf("Connexion Closed\n")  ;
    close(clntSocket);    /* Close client socket */
}
