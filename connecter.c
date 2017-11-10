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
/***************************************************
l'utilisateur envoie ses informations aux serveurs 
si il existe dans la base de donnée il accède au service 
pour soit envoyer un message,lire ou supprimer un message 
****************************************************************/

void connecter()
{
	char *message,*nom,*motdepasse;
	char nom1[50],motdepasse1[50],trame[150];
	int i,retour,verif;
	char choix;
	verif=0;
	i=0;
	do
	{
	Emission("CONNECT_C\n");
	message=Reception();
     if(strcmp(message,"OK_CONNECT_S"))
       {      
    	free(message);
    	
    	
        
    	printf("Entrer votre nom \n");
        fgets(nom1,50,stdin);
        nom=change(nom1);
        
    	printf("Entrer votre motdepasse\n");
    	
      motdepasse=masquermodepasse();
    	Emission("CONNECT_M\n");
    	strcpy(trame,nom);
    	strcat(trame," ");
    	strcat(trame,motdepasse);
    	strcat(trame,"\n");
    	Emission(trame);
    	message=Reception();
    
    	if(strstr(message,"OK_CONNECT_M"))
    	{  clrscr();
    		printf("Connexion reussie \n");
    		verif=1;

    	}
    	else 
    	{
    	  printf("NOM OU motdepasse incorrect\n");
    	  clrscr();
    	}
    }
        i++;
        if(i>3 && verif!=1)
        { clrscr();
        	printf("Vous avez écoulé le nombre de tentative \n");
        	verif=1;
            
             
        }
         

     }while(verif!=1);

     int ch;
     if(!strcmp(message,"OK_CONNECT_M\n"))
     {
     do
     {
      printf("\t\t***********************************\n");
      printf("\t\t*\t\t\t\t  *\n");
      printf("\t\t*\t\t\t\t  *\n");
      printf("\t\t*\t");printf("Utilisateur:%s\t\t  \n",nom);
      printf("\t\t*\t\t\t\t  *\n");
      printf("\t\t*\t\t\t\t  *\n");
      printf("\t\t***********************************\n");
      menu2();
      controleSaisie(&ch);
      switch(ch)
      {
      	case 1:
      	{   
      		    clrscr();
              EnvoyerMes();
      		
      		break;

      	}
      	case 2:
      	{
      		clrscr();
      		lireMesboite();
      		break;
      	}
      	case 3:
      	{   clrscr();
      		supprimerMessage();
      		break;
      	}
      	case 4:
      	{   clrscr();
            deleteAll();

            break; 
      	 
        }
        case 5:
        {
          clrscr();  
          Emission("any_activity\n");
          break;
     

        }
          

      }
     }while(ch!=5);

    }	
    
   

 }
 void sousMenu()
 {
  couleur("34");
  printf("\t \t 1.BOITE DE  Reception \n");
  printf("\t \t 2.BOITE D'ENVOIE \n");
  printf("\t \t 3.Corbeille \n");
  printf("\t \t 4.QUITTER \n");
  couleur("0");
 }
 void deleteAll()
 {
  char c[4];
  char *message;
  int choix;
  Emission("deleteAll\n");
  message=Reception();
   
   if(!strcmp(message,"deleteAll_S\n"))
   {
    do
    {
      sousMenu();
      controleSaisie(&choix);

   
   }while(choix<1 || choix >4);


   
    
    sprintf(c,"%d",choix);
     strcat(c,"\n");
     Emission(c);
     message=Reception();

     if(!strcmp(message,"OK_deleleAll\n"))
     {
      if(choix==1)
      {
       couleur("34");
       printf("BOITE DE RÉCEPTION VIDÉE \n");
       couleur("0");
      }
      else if(choix==2)
      {
        couleur("34");
        printf("BOITE D'ENVOIE VIDÉE \n");
        couleur("0");
      }
      else if(choix==3)
      {
        couleur("34");
        printf("CORBEILLE VIDÉE  \n");
        couleur("0");
      }
      else if(choix==4)
      {
        printf("\n");
      }
     }
     else
     {
      couleur("34");
       printf("BOITE DEJA VIDE \n ");
      couleur("0");
     }
      
                
   }


 }

