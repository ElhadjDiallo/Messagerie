
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <termios.h>
#include "client.h"
#include "couleurs.h"
#include "gesttionclient.h"
#include "connect.h"
#define TRUE 1
#define FALSE 0

#define LONGUEUR_TAMPON 4096


/* Variables cachees */

/* le socket client */
int socketClient;
/* le tampon de reception */
char tamponClient[LONGUEUR_TAMPON];
int debutTampon;
int finTampon;
//fonction qui enlève le "\n" dans une chaine de caractère
char * change(char *chaine)
{
	int i;
	char *ch;
	ch=(char*)malloc(strlen(chaine)+1*sizeof(char));

    for(i=0;i<strlen(chaine)-1;i++)
    	ch[i]=chaine[i];
    ch[strlen(chaine)-1]='\0';
    return ch;

 
}
// qui permet de lire une chaine de caractère de taille dynamique
 char * lire_chaine(char * message,int *longueur)
{
size_t taille =5;
char *buffer = malloc(sizeof(taille)); 

if (buffer == NULL) return NULL; 

char *p;
for(p=buffer ; (*p=getchar()) != '\n' && *p!=EOF ; ++p) 
if (p - buffer == taille - 1)
{ 
p = realloc(buffer, taille *= 2); 
if (p == NULL)
{ 
free(buffer);
return NULL;
}
else
buffer = p;
p += taille/2 - 1; 
}

*p = 0;
p = realloc(buffer, p - buffer + 1);
if(p==NULL) 
{

free(buffer);
return NULL;
}
 else
{
*longueur=strlen(p);
return p;
}
}
// fonction pour vider le buffer 

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

/*fonction qui permet d'envoyer un message en choisissant le destinateur*/
 void  EnvoyerMes()
 {

 	
 	int taille,tailleMs;
	char *message,*objet,*trame,*mes,*dest;
	viderBuffer();
    do
    {
    printf("Entrer le nom du destinataire \n");
    dest=lire_chaine(dest,&taille);
    if(taille>50)
    {
    printf("Vous avez le droit à 50 caractères au maximum\n");	
    }
      
   }while(taille>50);
	Emission("SENT_C_DEST\n");
	strcat(dest,"\n");
	Emission(dest);
	message=Reception();
   if(!strcmp(message,"OK_SENT_S_DEST\n"))
   {       
   	     do
   	     {
   	     printf("Entrer l'objet de votre message \n");
         // on lit l'objet du message 
         objet=lire_chaine(objet,&taille);
         if(taille>100)
         	printf("vous avez droit à 100 caractères au maximum\n");
         }while((taille>100));
         // on reserve une taille pour la trame à envoyer 
         trame=malloc(taille+strlen("SENT_C_Obj\n")+3*sizeof(char));
         // on fabrique la trame puis on l'emet 
         strcpy(trame,"SENT_C_Obj ");
         strcat(trame,objet);
         strcat(trame,"\n");
         Emission(trame);
         free(message);
         free(trame);
         // on attend la reponse du serveur 
         message=Reception();
         if(!strcmp(message,"OK_SENT_S_Obj\n"))	
         {    
         	printf("ENTRER VOTRE MESSAGE \n");
            mes=lire_chaine(mes,&tailleMs);
            // on fabrique la trame pour l'envoyer le conten du message 
            trame=malloc(tailleMs+strlen("SENT_C_Msg\n")+2*sizeof(char));
            strcpy(trame,"SENT_C_Msg ");
            strcat(trame,mes);
            strcat(trame,"\n");
            Emission(trame);
            free(mes);
            free(trame);
            free(message);
            message=Reception();

            // si l'envoie c'est bien passé le serveur nous reponds ça 
          if(!strcmp(message,"OK_SENT_S_Msg\n"))
            {
            	clrscr();
            	printf("Votre message a été bien envoyé \n");
            	
            }
            else 
            {
              // sinon il nous precise il a eu un problème 
             clrscr();
            printf("Votre message n'a pas pu etre envoyé  \n");	
            }
              
        }
            
   }         
 } 
 void menu2()
 {  couleur("34");
 	printf(" \t \t 1 Envoyer message \n");
 	printf(" \t \t 2.Pour lire un message \n");
 	printf(" \t \t 3.Supprimer un message \n");
    printf(" \t \t 4.Vider une boite  \n");
 	printf(" \t  \t5. Quitter \n");
 	couleur("0");

 }
void menu()
{
	couleur("34");
     printf("\n\n\n\n\n\n\n\n");
printf("\t\t\t");  printf("*********************************************************\n");
printf("\t\t\t");	printf("*");printf("\t \t Boite Mail Diallo_BAH\t \t\t");printf("*\n");
printf("\t\t\t");	printf("*");printf("\t \t 1.Inscription \t \t\t\t");printf("*\n");
printf("\t\t\t");	printf("*");printf("\t \t 2.Connexion\t \t\t\t");printf("*\n");
printf("\t\t\t");	printf("*");printf("\t \t 3.quitter\t \t\t\t");printf("*\n");
printf("\t\t\t");  printf("*********************************************************\n");
	couleur("0");
}
//verifie la saisie entre par l'utilisateur

void controleSaisie(int *choix)
{
	int retour;
	do
     {
	 retour=scanf("%d",choix);
     if(retour!=1)
     {
     	  printf("Erreur de saisie \n");
     	  viderBuffer();
     }
	 }while(retour!=1);
	 
}
// permet de masquer la saisie du mot de passe à l'ecran 
char *masquermodepasse()
{
    struct termios oflags, nflags;
    char password[64];

    /* disabling echo */
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        return NULL;
    }
    

    printf("password: ");
    fgets(password, sizeof(password), stdin);
    password[strlen(password) - 1] = 0;
   
      char * ch;
     ch=malloc(strlen(password)+1*sizeof(char));
     strcpy(ch,password);
     if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) 
        perror("tcsetattr");
    
     return ch;
    
    /* restore terminal */
    
}
/*******************************************
on demande à l'utilisateur d'entrer son nom 
et son prenom on verifie si le nom et le mot 
de passe n'existe pas déjà dans la base 
de donnée si non on l'ajoute à la base 
il doit repeter son mot de passe deux 
fois après quelques tentives on le demande 
d'essayer ulterieurement 



***********************************************/

void inscription()
{
	char *message;
	char *trame;
   
   int i,arret;

	char nom1[30],verif1[50],motdepasse1[50];
	char *nom,*motdepasse,*verif;
	printf("Entrer votre nom \n");
	fgets(nom1,30,stdin);
	 nom=change(nom1);
	 i=0;
	 do
	 {
     printf("Enter un mot de passe \n");
     motdepasse=masquermodepasse();
     
       
       arret=0;
	
	

	 printf("repeter votre mot de passe \n");
	 verif=masquermodepasse();
	 i++;

	 if(i==4)
	 {
	 	clrscr();
	 	printf("Vous avez ecoulé vos nombre de tentative \n");
        arret=1;
	 }
	 if(!strcmp(motdepasse,verif))
	 	arret=1;
	}while(arret!=1);

	  if(!strcmp(motdepasse,verif))
	  	{      
	  
	  trame=malloc(strlen(nom)+strlen(motdepasse)+3*sizeof(char));
      strcpy(trame,nom);

      strcat(trame," ");
      strcat(trame,motdepasse);
      strcat(trame,"\n");
      Emission("First_connexion\n");
       message=Reception();
      if(!strcmp(message,"OK_first_S\n"))
      {
      	  Emission("C_inscription\n");  
          Emission(trame);
          free(message);
          message=Reception();
       if(!strcmp(message,"S_inscription\n"))
       {
      	printf("votre compte est maintenant crée  \n");
      	
       }
       else 
       	printf("Ce nom ou mot de passe est déjà utilisé  \n");
      
      }
      }
        
}
int connexion(char*machine)
{
	machine=malloc(strlen("localhost")+1*sizeof(char));
	strcpy(machine,"localhost");

	if(Initialisation(machine) != 1) {
		printf("Erreur d'initialisation\n");
		return 1;
	}


}

/* Initialisation.
 * Connexion au serveur sur la machine donnee.
 * Utilisez localhost pour un fonctionnement local.

 */
int Initialisation(char *machine) {
	return InitialisationAvecService(machine, "13214");
}

/* Initialisation.
 * Connexion au serveur sur la machine donnee et au service donne.
 * Utilisez localhost pour un fonctionnement local.
 */
int InitialisationAvecService(char *machine, char *service) {
	int n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(machine, service, &hints, &res)) != 0)  {
     		fprintf(stderr, "Initialisation, erreur de getaddrinfo : %s", gai_strerror(n));
     		return 0;
	}
	ressave = res;

	do {
		socketClient = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketClient < 0)
			continue;	/* ignore this one */

		if (connect(socketClient, res->ai_addr, res->ai_addrlen) == 0)
			break;		/* success */

		close(socketClient);	/* ignore this one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL) {
     		perror("Initialisation, erreur de connect.");
     		return 0;
	}

	freeaddrinfo(ressave);
	printf("Connexion avec le serveur reussie.\n");

	return 1;
}

/* Recoit un message envoye par le serveur.
 */
char *Reception() {
	char message[LONGUEUR_TAMPON];
	int index = 0;
	int fini = FALSE;
	int retour = 0;
	while(!fini) {
		/* on cherche dans le tampon courant */
		while((finTampon > debutTampon) && 
			(tamponClient[debutTampon]!='\n')) {
			message[index++] = tamponClient[debutTampon++];
       
		}
		/* on a trouve ? */
		if ((index > 0) && (tamponClient[debutTampon]=='\n')) {
			message[index++] = '\n';
			message[index] = '\0';
			debutTampon++;
			fini = TRUE;
			return strdup(message);
		} else {
			/* il faut en lire plus */
			debutTampon = 0;
			retour = recv(socketClient, tamponClient, LONGUEUR_TAMPON, 0);
			if (retour < 0) {
				perror("Reception, erreur de recv.");
				return NULL;
			} else if(retour == 0) {
				fprintf(stderr, "Reception, le serveur a ferme la connexion.\n");
				return NULL;
			} else {
				/*
				 * on a recu "retour" octets
				 */
				finTampon = retour;
			}
		}
	}
	return NULL;
}

/* Envoie un message au serveur.
 * Attention, le message doit etre termine par \n
 */
int Emission(char *message) {
	if(strstr(message, "\n") == NULL) {
		fprintf(stderr, "Emission, Le message n'est pas termine par \\n.\n");
	}
	int taille = strlen(message);
	if (send(socketClient, message, taille,0) == -1) {
        perror("Emission, probleme lors du send.");
        return 0;
	}
	//printf("Emission de %d caracteres.\n", taille+1);
	return 1;
}

/* Recoit des donnees envoyees par le serveur.
 */
int ReceptionBinaire(char *donnees, size_t tailleMax) {
	int dejaRecu = 0;
	int retour = 0;
	/* on commence par recopier tout ce qui reste dans le tampon
	 */
	while((finTampon > debutTampon) && (dejaRecu < tailleMax)) {
		donnees[dejaRecu] = tamponClient[debutTampon];
		dejaRecu++;
		debutTampon++;
	}
	/* si on n'est pas arrive au max
	 * on essaie de recevoir plus de donnees
	 */
	if(dejaRecu < tailleMax) {
		retour = recv(socketClient, donnees + dejaRecu, tailleMax - dejaRecu, 0);
		if(retour < 0) {
			perror("ReceptionBinaire, erreur de recv.");
			return -1;
		} else if(retour == 0) {
			fprintf(stderr, "ReceptionBinaire, le serveur a ferme la connexion.\n");
			return 0;
		} else {
			/*
			 * on a recu "retour" octets en plus
			 */
			return dejaRecu + retour;
		}
	} else {
		return dejaRecu;
	}
}

/* Envoie des donn�es au serveur en pr�cisant leur taille.
 */
int EmissionBinaire(char *donnees, size_t taille) {
	int retour = 0;
	retour = send(socketClient, donnees, taille, 0);
	if(retour == -1) {
		perror("Emission, probleme lors du send.");
		return -1;
	} else {
		return retour;
	}
}


/* Ferme la connexion.
 */
void Terminaison() {
	close(socketClient);
}
