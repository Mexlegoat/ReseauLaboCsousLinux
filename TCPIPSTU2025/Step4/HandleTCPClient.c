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
    int recvMsgSize, sentBytes, rc;                    /* Size of received message */
    struct Requete UneRequete ;
    struct Requete uneRequeteR;   // Requête reçue
    struct Requete uneRequeteE;   // Requête envoyée
    struct VehiculeHV UnRecord;   // Record trouvé dans le fichier

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, &UneRequete, sizeof (struct Requete), 0)) < 0)
        DieWithError("recv() failed");
    uneRequeteR.Reference = UneRequete.Reference;
    uneRequeteR.Type = UneRequete.Type;
    uneRequeteE = UneRequete;
    while(recvMsgSize>0) 
    {   
      switch(uneRequeteR.Type) 
      { 
        case Question:
             rc = RechercheHV("VehiculesHV",uneRequeteR.Reference ,&UnRecord) ; 
            fprintf(stderr,"res :%d Reference:%s %s\n",rc,UnRecord.Constructeur, 
            UnRecord.Modele) ; 
                    /* reponse avec psor qui contient toujours l'adresse du dernier client */ 
                
            memset(&uneRequeteE,0, sizeof(struct Requete)) ; 
            if ( rc==1) 
            { 
                uneRequeteE.Reference = uneRequeteR.Reference ; 
                 
            strncpy(uneRequeteE.Constructeur, UnRecord.Constructeur, sizeof(uneRequeteE.Constructeur));
            strncpy(uneRequeteE.Modele, UnRecord.Modele, sizeof(uneRequeteE.Modele));

            uneRequeteE.Quantite = UnRecord.Quantite;
            uneRequeteE.Puissance = UnRecord.Puissance;
            uneRequeteE.Type = OK;   // trouvé
            } 
            else 
                 uneRequeteE.Type = Fail ; 
           rc = send(clntSocket, &uneRequeteE, sizeof(struct Requete), 0) ; 
           if ( rc == -1 ) 
           DieWithError ("SendDatagram:") ; 
           else 
              printf("bytes ecrits %d\n",rc ) ; 
           break ;
        default : fprintf(stderr,"Code incorrect %d\n",uneRequeteR.Type ) ; 
      } 

        /* See if there is more data to receive */
        if ((recvMsgSize = read(clntSocket, &UneRequete, sizeof (struct Requete))) < 0)
            DieWithError("recv() failed");
        else if ( rc !=0 )  
            { 
            printf("Bytes received:%d\n", recvMsgSize) ; 
            AfficheRequete(uneRequeteE) ; 
            } 
            else 
            printf("Fermeture de connexion\n") ; 
    }

    printf("Connexion Closed\n")  ;
    close(clntSocket);    /* Close client socket */
}
