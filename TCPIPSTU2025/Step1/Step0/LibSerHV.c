#include "LibSerHV.h"

void AProposServeurHV(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version ) ;
 printf("Nom1 : %s \n",Nom1 ) ;
 printf("Nom2 : %s \n",Nom2 ) ;
}
int RechercheHV(char* NomFichier,int Reference ,struct VehiculeHV *UnRecord)
{
    int fd, taille, calc, i = 0;
    fd = open(NomFichier, O_RDONLY );
    if (fd == -1)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier\n");
        return -1;
    }
    if (Reference == 0) // Empecher l'utilisateur d'utiliser 0 car la reference commence par 1
    {
        close(fd);
        return 0;
    }
    while ((i = read(fd, UnRecord, sizeof(struct VehiculeHV))) == sizeof(struct VehiculeHV)) // La variable i va lire dans le fichier structure par structure
    {
        fprintf(stderr, "Reference lu %d Position actuelle dans le fichier %d\n", UnRecord->Reference, lseek(fd, 0, SEEK_CUR)); // affichage
        if (Reference == UnRecord->Reference) // Si La reference saisie = celle qui se trouve dans la structure
        {
            close(fd); // fermer le fichier
            viderBuffer();
            return 1; // Voir admin.c
        }
    } // Si la reference saisie est differente de tout les references se trouvant dans les structures ecrits dans le fichier
    close(fd); // fermer le fichier
    return 0; // Voir admin.c
    
}

void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}