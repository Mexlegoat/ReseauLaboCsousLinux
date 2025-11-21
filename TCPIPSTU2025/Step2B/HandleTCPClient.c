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
    uneRequeteE = UneRequete;
    while(recvMsgSize>0) 
    {   
        printf("Bytes lus : %d\n", recvMsgSize);
        printf("RechercheHV> %d\n", uneRequeteR.Reference);
        printf("RechercheHV<\n");
        if ((rc = RechercheHV("VehiculesHV", uneRequeteR.Reference, &UnRecord)) > 0)
        {
            strncpy(uneRequeteE.Constructeur, UnRecord.Constructeur, sizeof(uneRequeteE.Constructeur));
            strncpy(uneRequeteE.Modele, UnRecord.Modele, sizeof(uneRequeteE.Modele));

            uneRequeteE.Quantite = UnRecord.Quantite;
            uneRequeteE.Puissance = UnRecord.Puissance;
            uneRequeteE.Type = OK;   // trouvé

        }
        UneRequete = uneRequeteE;
        AfficheRequete(UneRequete) ; 
        // Copie des champs trouvés

        /* Echo message back to client */
        sentBytes = send(clntSocket, &UneRequete, sizeof(struct Requete), 0);
        if (sentBytes != sizeof(struct Requete))
            DieWithError("send() failed");
        printf("Bytes écrits : %d\n", sentBytes);
        printf("res: %d", uneRequeteR.Reference);
        printf("\tReference: %s %s\n", uneRequeteE.Constructeur, uneRequeteE.Modele);

        /* See if there is more data to receive */
        if ((recvMsgSize = read(clntSocket, &UneRequete, sizeof (struct Requete))) < 0)
                DieWithError("recv() failed");
    }

    printf("Connexion Closed\n")  ;
    close(clntSocket);    /* Close client socket */
}
