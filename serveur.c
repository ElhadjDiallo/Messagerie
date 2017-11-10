#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <errno.h>

#include "serveur.h"
#include "gestionfichier.h"
#include "assert.h"
#define TRUE 1
#define FALSE 0
#define LONGUEUR_TAMPON 4096


#ifdef WIN32
#define perror(x) printf("%s : code d'erreur : %d\n", (x), WSAGetLastError())
#define close closesocket
#define socklen_t int
#endif

/* Variables cachees */

/* le socket d'ecoute */
int socketEcoute;
/* longueur de l'adresse */
socklen_t longeurAdr;
/* le socket de service */
int socketService;
/* le tampon de reception */


char tamponClient[LONGUEUR_TAMPON];
int debutTampon;
int finTampon;
/**************************************************
Structure pour creer une trame avec les differents 
champs adresse source, destination,donnée et objet du message 
***************************************************/
struct latrame
{
	char *adrs;
	char *adrdest;
	char *donnee;
	char *objet;
};
typedef struct latrame trameInfos;
/***********************************************************
strucutre qui permet de recuperer le mot de passe dans la base 
de donnée qui est un fichier texte appelé base.txt où est stocqué 
les différents informations des utilisateurs

***********************************************************/
struct Etudiant
{
char nom[100];
char motdepasse[100];
int valeur;
};
typedef struct Etudiant Element;
/*************************************************
structure d'une liste doublement chainée contenant 
pour faciliter la recuperation des informations saisies dans 
le fichier base.txt
******************************************************/
struct listedoublementChainee2
{
struct listedoublementChainee2 *precedent;
struct listedoublementChainee2 *suivant;
Element leselements;
};
typedef struct listedoublementChainee2 cellule2;
typedef struct listedoublementChainee
{
struct listedoublementChainee2 *tete;
struct listedoublementChainee2 *queue;
}cellule;typedef struct listedoublementChainee *liste;
liste supprimer(liste maliste,Element e);
liste creer();
void trouverPlace(liste maliste,cellule2 **avant,cellule2 **apres,Element e);
liste ajouter(liste maliste,Element e);
void afficher(liste maliste);
void saisirUnElement(Element *e,int position,char *nom,char *motdepasse);
void afficherLesElements(Element e);
liste lecturefichier(char *nom);
int rechercher(liste base,infos linfos);
/* Variables cachees */

/* le socket d'ecoute */
int socketEcoute;
/* longueur de l'adresse */
socklen_t longeurAdr;
/* le socket de service */
int socketService;
/* le tampon de reception */

void afficherLesElements(Element e)
{
printf("%s %s a pour position %d \n",e.motdepasse,e.nom,e.valeur);
}
/***************************************************
fonction qui permet de placer les differents champs de la structure 
élement chacun à sa place
*****************************************************/
void saisirUnElement(Element *e,int position,char *nom,char *motdepasse)
{

e->valeur=position;
strcpy(e->nom,nom);
strcpy(e->motdepasse,motdepasse);
}

/************************************************
fonction qui permet de creer la liste et qui envoie 
une liste doublement chainée 
***********************************************/
liste creer()
{
liste maliste;
maliste=(liste)malloc(sizeof(cellule));
maliste->tete=NULL;
maliste->queue=NULL;
return maliste;
}
/***************************************************
fonction qui permet d'ajouter un element dans la liste 

***************************************************/
liste ajouter(liste maliste,Element e)
{
cellule2 *avant,*apres,*temporaire;
trouverPlace(maliste,&avant,&apres,e);
temporaire=(cellule2*)malloc(sizeof(cellule2));
assert(temporaire!=NULL);
temporaire->leselements=e;
temporaire->precedent=avant;
temporaire->suivant=apres;
if(avant==NULL)
maliste->tete=temporaire;
else
temporaire->precedent->suivant=temporaire;
if(apres==NULL)
maliste->queue=temporaire;
else
temporaire->suivant->precedent=temporaire;return maliste;
}
/*******************************************************************
fonction qui permet de trouver une position où inserer un element 
dans la liste 

********************************************************************/
void trouverPlace(liste maliste,cellule2 **avant,cellule2 **apres,Element e)
{
cellule2 *courant;
courant=maliste->tete;
*avant=NULL;
*apres=courant;
while(courant!=NULL)
{
if(courant->leselements.valeur<e.valeur)
{
*avant=courant;
courant=courant->suivant;
*apres=courant;

}
else 
courant=NULL;
}
}
/*******************************************************
fonction qui permet d'afficher les elements de la liste
********************************************************/
void afficher(liste maliste)
{
cellule2 *temporaire;
temporaire=maliste->tete;
while(temporaire!=NULL)
{
afficherLesElements(temporaire->leselements);
temporaire=temporaire->suivant;
}
}

char tamponClient[LONGUEUR_TAMPON];
int debutTampon;
int finTampon;
/*********************************************
fonction qui permet de lire un fichier ET recupère le mot de passe
puis le nom ainsi que la position du champ mot de passe et nom 
puis l'insère dans la liste
********************************************/
liste lecturefichier(char *nom)
{
	liste mabase;
	Element e;
	mabase=creer();
	FILE *fichier;
	char *lenom,*motdepasse;
	char chaine[100];
	fichier=fopen(nom,"r");
	int i=0;
	if(fichier!=NULL)
	{
		while(fgets(chaine,100,fichier)!=NULL)
		{

			motdepasse=recupererPartieChaine(chaine);
			lenom=strtok(chaine," ");
			saisirUnElement(&e,i,lenom,motdepasse);
			mabase=ajouter(mabase,e);
			i++;
			
		}
		fclose(fichier);
		
	}
	else 
		fprintf(stderr, "Erreur d'ouverture du fichier \n");
	return mabase;
}
/*****************************************************
fonction qui recupère une chaine de caractère composé de 
deux sous chaines séparés par un espace et recupère 
la deuxième sous chaine après l'espace

****************************************************/
char * recupererPartieChaine(char *message)
{
	char *chaine;
	int c,i,j;
	chaine=malloc(strlen(message)+3*sizeof(char));
	for(i=0;i<strlen(message)+1;i++)
		if(message[i]==' ')
		   c=i;
		
		j=0;
    for(i=c+1;i<strlen(message)-1;i++)
    {
    	chaine[j]=message[i];
    	j++;
    }
    chaine[j++]='\0';
    
    return chaine;
     

	

}
/***********************************************
fonction qui prend en arguments les champs infos 
de l'utilisateur pour se connecter c'est à dire 
son nom et son mot de passse et verifie dans 
la base donnée si ces champs sont déjà utilisés
ou non si oui renvoie 1 sinon renvoie 0


************************************************/

int verification(liste base,infos mesInfos)
{
	cellule2 *courant;
	courant=base->tete;
	int trouve=0;
	while(courant!=NULL && ! trouve)
	{
		if(strcmp(courant->leselements.nom,mesInfos.nom)==0)
		{
            
            trouve=1;
		}
		else if(!strcmp(courant->leselements.motdepasse,mesInfos.motdepasse))
		{
			
			trouve=1;
		}
		else 
			courant=courant->suivant;

         
	}
	return trouve;


}
/*************************************
fonction qui permet de modifier une chaine 
entrée par fgets pour supprimer le '\n'
et replacer le '\0' à la bonne positon

*************************************/
char * change(char *chaine)
{
	int i;
	char *ch;
	ch=malloc(strlen(chaine)*sizeof(char));
    for(i=0;i<strlen(chaine)-1;i++)
    	ch[i]=chaine[i];
    ch[strlen(chaine)-1]='\0';
    return ch;

 
}
/**********************************************
fonction qui prend en argument le chemin vers un 
repertoire et le fichier à supprimé 
ensuite supprime le fichier si il existe 




***********************************************/
int deletefic(char *chemin,char *fichier)
{
	int ret;
int i=0;
	char chaine[1024];
	FILE*fic;
    char *placefichier;
    placefichier=malloc(strlen(chemin)+strlen(fichier)+1*sizeof(char));
    strcpy(placefichier,chemin);
    strcat(placefichier,"/");
    strcat(placefichier,fichier); 
      ret=remove(placefichier);
      return ret;
  

}
/************************************************
fonction qui prend un chemin vers un fichier et le fichier 
en question puis lit le fichier et envoie son contenu 
au client 
*******************************************************/
int  lirfic(char *chemin,char *fichier)
{
	int i=0;
	char chaine[1024];
	FILE*fic;
    char *placefichier;
    placefichier=malloc(strlen(chemin)+strlen(fichier)+1*sizeof(char));
    strcpy(placefichier,chemin);
    strcat(placefichier,"/");
    strcat(placefichier,fichier);
    fic=fopen(placefichier,"r");
    if(fic!=NULL)
    {
    while(fgets(chaine,1024,fic)!=NULL)
    {

    	Emission(chaine);
    
    	
    }
    fclose(fic);	
    }

    
}

/**************************************************
fonction qui permet de trouver là le premier champ 
espace contenu dans une chaine de caractère 
nous renvoie la sous partie de la chaine restante

*****************************************************/	
char *recupererMes(char *message)
{
	char *chaine;
	chaine=malloc(strlen(message)*sizeof(char));
	int pos,trouve,c;
	pos=0;trouve=0;
	while(pos<strlen(message) && !trouve)
	{
		if(message[pos]==' ')
		{
			trouve=1;
		}
		pos++;
	}
	c=0;
	for(trouve=pos-1;trouve<strlen(message)-1;trouve++)
	{
          chaine[c]=message[trouve];
          c++;
	}
	chaine[c++]='\0';
	return chaine;
}
/*******************************************
fonction qui prend en argument le champ 
information de l'utilisateur qu'il a utilisé 
pour se connecter et qui nous permet d'envoyer 
un message 


*******************************************/
void consulterMessage(infos Info)
{
     int nombrefic;
    trameInfos info_t;
	char *message,*rep,*s,*chemin,*newfichier,*chemin2;
	char t[3];
	/*
	on attend la reponse du serveur après avoir demandé à se connecter
	*/
	message=Reception();

	rep=malloc(3*sizeof(char));
	//on stocke le repertoire courant dans repertoire
	strcpy(rep,".");
     //on modifie le message qu'on a reçu pour bien formater l'adresse destination 
	s=change(message);
  
    info_t.adrdest=malloc(strlen(s)+1*sizeof(char));
    //copie du message dans le champ adresse destination 
    strcpy(info_t.adrdest,s);
      
    //on informe au client qu'on a bien reçu l'adresse de destination 
       Emission("OK_SENT_S_DEST\n");
       free(message);
        free(s); 
         message=Reception();
    //on attend la requête pour recevoir un ojet du message 
         if(strstr(message,"SENT_C_Obj")!=NULL)
        {
   // on le copie dans le champ d'adresse s'il n'est pas null      	
     	info_t.objet=recupererMes(message);
   // on informe que l'etape c'est bien passé 
     	Emission("OK_SENT_S_Obj\n");
     	free(message);
     	message=Reception();
   // même chose pour le champ de donnée envoyé par le client   	
         if(strstr(message,"SENT_C_Msg")!=NULL)
     	{

        info_t.donnee=recupererMes(message);
     		
     		free(message);
     	
     	info_t.adrs=malloc(strlen(Info.nom)*sizeof(char));
   // je recupère le champ adresse source que je copie dans le champ correspondant de ma structure infos  	
         strcpy(info_t.adrs,Info.nom);
    // on recupère le chemin vers la boite de reception en fonction de l'adresse destination vu que c'est là-bas on doit stocquer le message
    // meme principe avec l'adresse source vu que le message envoyé doit y être stocquer

    
      chemin=chercherRepertoire(rep,info_t.adrdest,"reception");
    
      chemin2=chercherRepertoire(rep,info_t.adrs,"envoie");
  
      if(chemin!=NULL)
       	{
     //recupère le nombre de fichier contenu dans la boite de reception on cree un fichier avec un numero+1 pour ne pas écraser celui qui s'y 
      //puis on ecrit dans le fichier et pour les deux chemeins qu'on a recupéré
     		nombrefic=nombredefichier(chemin);
       	 newfichier=malloc(strlen("fichier")+3*sizeof(char));
       	 sprintf(t,"%d",nombrefic+1);
       	 strcpy(newfichier,"fichier");
          strcat(newfichier,t);
          ecritureFichier(chemin,newfichier,info_t.adrs);
          ecritureFichier(chemin,newfichier,info_t.objet);
       	 ecritureFichier(chemin,newfichier,info_t.donnee);
       	  free(newfichier);
       	  nombrefic=nombredefichier(chemin2);
       	  newfichier=malloc(strlen("fichier")+3*sizeof(char));
       	  sprintf(t,"%d",nombrefic+1);
       	   strcpy(newfichier,"fichier");
          strcat(newfichier,t);
          ecritureFichier(chemin2,newfichier,info_t.adrdest);
         ecritureFichier(chemin2,newfichier,info_t.objet);
       	 ecritureFichier(chemin2,newfichier,info_t.donnee);
       	    
       	 
 	      Emission("OK_SENT_S_Msg\n");  
        }
        else
          Emission("BAD_SENT_Msg\n");

        }
      }
 }
 /*
 fonction qui prend prend en argument les informations d'un utilisateur
puis une chaine de caractère qui se trouvera ici la corbeille de l'utilisateure
utilisera la fonction chercherRepertoire pour trouver la boite de reception 
corbeille et renvoyera le nombre de fichier de la corbeille et permettra à 
au client de restaurer un message contenu dans sa corbeille 
*/
void  gestioncorbeille(infos lesinfos,char *boite)
{
  char **tab;
  char rep[3]=".";
  int nombre,i,indice;
  char n[3],trame[100],chemin3[100];
  char *chemin,*source,*message,*chemin2,*fic,*adresse;
  
  //recupère le chemin vers le repertoire à partir des informations du client
  chemin=chercherRepertoire(rep,lesinfos.nom,boite);
  
    strcpy(trame,chemin);
    strcat(trame,"\n");
    //retourne le nombre de fichier contenu dans la corbeille
    nombre=nombredefichier(chemin);
    sprintf(n,"%d",nombre);
    strcat(n,"\n");
    Emission(n);
 
    if(nombre!=0)
    {
      
     //qui prend en argument un chemin vers une boite et ramène un tableau de fichier 
    tab=retournerTousLesfichier(chemin);
    for(i=0;i<nombre;i++)

    {   
        tab[i]=change(tab[i]);
    
        strcpy(trame,tab[i]);
        strcat(trame,"\n");
        //on envoie le contenu de la corbeille au client pour qu'il sache ce qu'il veut restaurer
        Emission(trame);
    }
    message=Reception();
    //on recupère son choix
    indice=atoi(message);
   
    indice--;
    
    fic=recupererPartieChaine(tab[indice]);
    //restauration vers la boit de reception ou boite d'envoie 
    if(adresseRestauration(chemin,tab[indice],lesinfos.nom))
     chemin2=chercherRepertoire(rep,lesinfos.nom,"reception");
     else 
     chemin2=chercherRepertoire(rep,lesinfos.nom,"envoie");
   //puis on le supprime de la corbeille
    deplacerUnFichierbis(chemin,chemin2,tab[indice],tab[indice]);
      if(!deletefic(chemin,tab[indice]))
        Emission("BACKUP_Delete_Msg\n");
      else 
        Emission("BAD_BACKUP_delete_Msg\n");

     
      
    }
}
void suprimertout(infos lesinfos)
{
  char **tab;
  char rep[3]=".";
  
  int choix,nombre,i;
 char *message,*chemin,*trame;
  
  
  message=Reception();
  choix=atoi(message);

  if(choix==1)
  {
   chemin=chercherRepertoire(rep,lesinfos.nom,"reception");

   trame=malloc(strlen(chemin)+2*sizeof(char));
   strcpy(trame,chemin);
   strcat(trame,"\n");
   nombre=nombredefichier(chemin);
   if(nombre!=0)
   {
    tab=retournerTousLesfichier(chemin);
    for(i=0;i<nombre;i++)
    {
     tab[i]=change(tab[i]);
    deletefic(chemin,tab[i]); 
    
 
    }
    Emission("OK_deleleAll\n");
      
   }
   else 
    Emission("BAD_deleteAll\n");
  }
  else if(choix==2)
    {
   chemin=chercherRepertoire(rep,lesinfos.nom,"envoie");
   
   trame=malloc(strlen(chemin)+2*sizeof(char));
   strcpy(trame,chemin);
   strcat(trame,"\n");
   nombre=nombredefichier(chemin);
   if(nombre!=0)
   {
    tab=retournerTousLesfichier(chemin);
    for(i=0;i<nombre;i++)
    {
     tab[i]=change(tab[i]);
      deletefic(chemin,tab[i]);
    
 
    }
   
    Emission("OK_deleleAll\n");

   }
   else
   Emission("BAD_deleteAll\n");

   }
   else if(choix==3)
    {
   chemin=chercherRepertoire(rep,lesinfos.nom,"corbeille");
   
   trame=malloc(strlen(chemin)+2*sizeof(char));
   strcpy(trame,chemin);
   strcat(trame,"\n");
   nombre=nombredefichier(chemin);
   if(nombre!=0)
   {
    tab=retournerTousLesfichier(chemin);
    for(i=0;i<nombre;i++)
    {
     tab[i]=change(tab[i]);
     deletefic(chemin,tab[i]);
    
 
    }
   
       Emission("OK_deleleAll\n");

   }
   else 
    Emission("BAD_deleteAll\n");
   }
   else if(choix==4)
   {
        Emission("OK_deleleAll\n");    
   }
   else
  Emission("BAD_deleteAll\n"); 

  
 

}

 //qui prend en argument un champ information pour pouvoir determiner le chemin de la boite dans la quelle on veut 

 //suprimer un message puis le nom de la boite pour preciser entre reception et envoie et appelle la fonction 

 //delectfic pour supprimer le message
  
 void suppression(infos lesinfos,char *boite)
 {

  char **tab;
	char rep[3]=".";
  int nombre,i,indice;
	char n[3];
	char *chemin,*trame,*source,*message,*chemin2;
  
  chemin2=chercherRepertoire(rep,lesinfos.nom,"corbeille");
	chemin=chercherRepertoire(rep,lesinfos.nom,boite);
    trame=malloc(strlen(chemin)+2*sizeof(char));
    strcpy(trame,chemin);
    strcat(trame,"\n");
    
    nombre=nombredefichier(chemin);
    sprintf(n,"%d",nombre);
    strcat(n,"\n");
    Emission(n);
 
    if(nombre!=0)
    {
      
     //qui prend en argument un chemin vers une boite et ramène un tableau de fichier 
    tab=retournerTousLesfichier(chemin);
    for(i=0;i<nombre;i++)
    {
      
     tab[i]=change(tab[i]);
     strcpy(trame,tab[i]);
     
     strcat(trame,"\n");
     Emission(trame);
    }
    message=Reception();
    indice=atoi(message);
   
    indice--;
    //deplace le fichier dans la corbeille 
    deplacerUnFichier(chemin,chemin2,tab[indice],boite);
    if(!deletefic(chemin,tab[indice]))
        Emission("OK_Delete_msg\n");
      else 
        Emission("BAD_delete_Msg\n");

     
     
    }

 }
 void lireMessage(infos lesinfos,char *boite)
{

	char **tab;
	char rep[3]=".";int nombre,i,indice;
	char n[3];
	char *chemin,*trame,*source,*message;
	//on recupère le chemin vers la bote où on desire lire un message 
	chemin=chercherRepertoire(rep,lesinfos.nom,boite);
  trame=malloc(strlen(chemin)+2*sizeof(char));
    strcpy(trame,chemin);
    strcat(trame,"\n");
    //on compte le nombre de fichier qui s'y trouve 
    nombre=nombredefichier(chemin);
    sprintf(n,"%d",nombre);
    strcat(n,"\n");
    Emission(n);
    if(nombre!=0)
    {
   //si le nombre n'est pas null on recupère tous les fichiers da la boite 	
    tab=retournerTousLesfichier(chemin);
    for(i=0;i<nombre;i++)
    {
     tab[i]=change(tab[i]);
     strcpy(trame,tab[i]);
     
     strcat(trame,"\n");
     Emission(trame);
    }
    message=Reception();
    if(!strcmp(message,"List_Nb_Msg_C\n"))
    {
   //on recupère l'indice du fichier que le client desire lire 
    message=Reception();
    indice=atoi(message);

    if(indice>0 || indice<=nombre)
    {
    indice--;
    //on lit le fichier et l'envoie au client

    lirfic(chemin,tab[indice]);
    Emission("Ok_List_S_Msg\n");
    }
    else 
    Emission("Error_List_S_Msg\n");
 
     
    }
  }



    
}
/***********************************************************************
fonction qui permet la connexion au serveur à partir des identifiants 
ou s'il veut effecture n'importe quel autre traitement 

************************************************************************/
void gestionconnexion()
{
	liste base;
	char *message,*nom,*motdepasse,*baseDeDonnee;

	Emission("CONNECT_S\n");

   
  do
  {
	message=Reception();
	infos lesinfos;
  
  

   if(message!=NULL)
   {
   if(!strcmp(message,"CONNECT_M\n"))
	{
		free(message);
		message=Reception();
		
	
	    motdepasse=recupererPartieChaine(message);
		nom=strtok(message," ");
        
		strcpy(lesinfos.nom,nom);
		
		//après recuperation des informations du client on verifie s'ils sont dans la base 
		strcpy(lesinfos.motdepasse,motdepasse);
        baseDeDonnee=malloc(strlen("base.txt")*sizeof(char));
        strcpy(baseDeDonnee,"base.txt");
        base=lecturefichier(baseDeDonnee);
        
         if(rechercher(base,lesinfos)==1)
         {
         
           Emission("OK_CONNECT_M\n");
         }
         else 
         	
         Emission("BAD_CONNECT_M\n");
     
        
     }
     else if(!strcmp(message,"SENT_C_DEST\n"))
     {
     	      
     	     consulterMessage(lesinfos);
     }
     else if(!strcmp(message,"CONNECT_C\n"))
     {
           Emission("OK_CONNECT_S\n");     	
     }
     else if(!strcmp(message,"READ_C_MSG_Num\n"))
      {
            Emission("OK_READ_S_MSG\n");	
           	lireMessage(lesinfos,"reception");
     }
     else if(!strcmp(message,"READ_C_MSG_SENT\n"))
     {
             Emission("OK_READ_S_MSG_SENT\n");
      	     lireMessage(lesinfos,"envoie");
           
	  }  
	  else if(!strcmp(message,"Delete_CB_msg\n"))
	  {
         suppression(lesinfos,"reception");


	  }
	  else if(!strcmp(message,"Delete_CS_msg\n"))
	  {
         suppression(lesinfos,"envoie");
	  }
	  else if(!strcmp(message,"First_connexion\n"))
    {
      inscriptionBasedeDonnee();
    }
    else if(!strcmp(message,"Delete_Ccor_msg\n"))
    {
       
      gestioncorbeille(lesinfos,"corbeille");
    }
    else if(!strcmp(message,"deleteAll\n"))
   {
      Emission("deleteAll_S\n");

     suprimertout(lesinfos);
    }
     
    else
    printf("fin de traitement \n");
    
	  }
    else 
    {
      
      message=malloc(15*sizeof(char));
      strcpy(message,"any_activity\n");
      

	 }
	  	

       
           

  }while(strcmp(message,"any_activity\n")!=0);
     
    
 } 
 void gestionDesErreurs()
 {
    printf("BAD_REQUEST \n");
  
 }   
/************************************************
fonction qui permet de gerer l'inscription à 
la base donnée 


************************************************/

void inscriptionBasedeDonnee()
{
	liste mabase;
	
	char *nom,*message,*baseDeDonnee,*reste;
	infos monInfos;	
	nom=malloc(sizeof(char));
	Emission("OK_first_S\n");
	message=Reception();
	if(strstr(message,"C_inscription"))
	{
		
		free(message);
		message=Reception();
		reste=recupererPartieChaine(message);
    nom=strtok(message," ");
    strcpy(monInfos.nom,nom);
    strcpy(monInfos.motdepasse,reste);
    baseDeDonnee=malloc(strlen("base.txt")*sizeof(char));
    strcpy(baseDeDonnee,"base.txt");
    mabase=lecturefichier(baseDeDonnee);
    if(!verification(mabase,monInfos))
    {
    			
      creationDelaBoiteMail(monInfos.nom);
      placerLesUtilisateurDansfichier(baseDeDonnee,monInfos);
    	Emission("S_inscription\n");
    	
    }
    else 
    Emission("bad_conect\n");
   	free(message);



	}


   

}
//recherche un element dans une liste chainee
int rechercher(liste base,infos linfos)
{
	cellule2 *courant;
	courant=base->tete;
	int trouve=0;
	while(courant!=NULL && !trouve)
	{
	   if(strcmp(courant->leselements.nom,linfos.nom)==0 && strcmp(courant->leselements.motdepasse,linfos.motdepasse)==0)
		{
			
			trouve=1;
		}
		else

		courant=courant->suivant;

	}
	return trouve;

}
//inscrit les informations du client dans la base de donnée 
int placerLesUtilisateurDansfichier(char *nom,infos mesInfos)
{
	FILE *base;
	base=fopen(nom,"a");
	if(base!=NULL)
	{
	    fprintf(base, "%s %s\n", mesInfos.nom,mesInfos.motdepasse);
	    fclose(base);
	}
	else 
      fprintf(stderr, "Erreur d'ouverture du fichier ");

}            


/* Initialisation.
 * Creation du serveur.
 */
int Initialisation() {
	return InitialisationAvecService("13214");
}

/* Initialisation.
 * Creation du serveur en pr�cisant le service ou num�ro de port.
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int InitialisationAvecService(char *service) {
	int n;
	const int on = 1;
	struct addrinfo	hints, *res, *ressave;

	#ifdef WIN32
	WSADATA	wsaData;
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR)
	{
		printf("WSAStartup() n'a pas fonctionne, erreur : %d\n", WSAGetLastError()) ;
		WSACleanup();
		exit(1);
	}
	memset(&hints, 0, sizeof(struct addrinfo));
    #else
	bzero(&hints, sizeof(struct addrinfo));
	#endif

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(NULL, service, &hints, &res)) != 0)  {
     		printf("Initialisation, erreur de getaddrinfo : %s", gai_strerror(n));
     		return 0;
	}
	ressave = res;

	do {
		socketEcoute = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketEcoute < 0)
			continue;		/* error, try next one */

		setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
#ifdef BSD
		setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
#endif
		if (bind(socketEcoute, res->ai_addr, res->ai_addrlen) == 0)
			break;			/* success */

		close(socketEcoute);	/* bind error, close and try next one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL) {
     		perror("Initialisation, erreur de bind.");
     		return 0;
	}

	/* conserve la longueur de l'addresse */
	longeurAdr = res->ai_addrlen;

	freeaddrinfo(ressave);
	/* attends au max 4 clients */
	listen(socketEcoute, 4);
	printf("Creation du serveur reussie sur %s.\n", service);

	return 1;
}

/* Attends qu'un client se connecte.
 */
int AttenteClient() {
	struct sockaddr *clientAddr;
	char machine[NI_MAXHOST];

	clientAddr = (struct sockaddr*) malloc(longeurAdr);
	socketService = accept(socketEcoute, clientAddr, &longeurAdr);
	if (socketService == -1) {
		perror("AttenteClient, erreur de accept.");
		return 0;
	}
	if(getnameinfo(clientAddr, longeurAdr, machine, NI_MAXHOST, NULL, 0, 0) == 0) {
		printf("Client sur la machine d'adresse %s connecte.\n", machine);
	} else {
		printf("Client anonyme connecte.\n");
	}
	free(clientAddr);
	/*
	 * Reinit buffer
	 */
	debutTampon = 0;
	finTampon = 0;

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
#ifdef WIN32
			return _strdup(message);
#else
			return strdup(message);
#endif
		} else {
			/* il faut en lire plus */
			debutTampon = 0;
			retour = recv(socketService, tamponClient, LONGUEUR_TAMPON, 0);
			if (retour < 0) {
				perror("Reception, erreur de recv.");
				return NULL;
			} else if(retour == 0) {
				fprintf(stderr, "Reception, le client a ferme la connexion.\n");
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

/* Envoie un message au client.
 * Attention, le message doit etre termine par \n
 */
int Emission(char *message) {
	int taille;
	if(strstr(message, "\n") == NULL) {
		fprintf(stderr, "Emission, Le message n'est pas termine par \\n.\n");
		return 0;
	}
	taille = strlen(message);
	if (send(socketService, message, taille,0) == -1) {
        perror("Emission, probleme lors du send.");
        return 0;
	}
	printf("Emission de %d caracteres.\n", taille+1);
	return 1;
}


/* Recoit des donnees envoyees par le client.
 */
int ReceptionBinaire(char *donnees, size_t tailleMax) {
	size_t dejaRecu = 0;
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
		retour = recv(socketService, donnees + dejaRecu, tailleMax - dejaRecu, 0);
		if(retour < 0) {
			perror("ReceptionBinaire, erreur de recv.");
			return -1;
		} else if(retour == 0) {
			fprintf(stderr, "ReceptionBinaire, le client a ferme la connexion.\n");
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

/* Envoie des donn�es au client en pr�cisant leur taille.
 */
int EmissionBinaire(char *donnees, size_t taille) {
	int retour = 0;
	retour = send(socketService, donnees, taille, 0);
	if(retour == -1) {
		perror("Emission, probleme lors du send.");
		return -1;
	} else {
		return retour;
	}
}



/* Ferme la connexion avec le client.
 */
void TerminaisonClient() {
	close(socketService);
}

/* Arrete le serveur.
 */
void Terminaison() {
	close(socketEcoute);
}
