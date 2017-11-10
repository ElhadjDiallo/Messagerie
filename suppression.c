
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
//Pour presenter à l'utilisateur des choix qu'il peut faire 
void menu3()
{
  couleur("34");
  printf("1.POUR LISTER LES MESSAGES REÇU \n");
  printf("2.POUR LISTER LES MESSAGES ENVOYES \n");
  printf("3.POUR LISTER LES MESSAGES DE LA CORBEILLE \n");
  printf("4. POUR QUITTER \n");
  couleur("0"); 

  

}
/*************************************************************************
Cette fonction permet de lister les messages contenues dans les différents 
boites des utilsateurs de supprimés ces messages de les garder dans une corbeille 
puis les restaurer s'il le souhaite

*********************************************************************/

void supprimerMessage()
{
   char *message;
	char c[3];
	            
	int nombre,i,nb;
	int  ch;
  do
  {
    //la fonction controle saisie prend en argument l'adresse d'un entier et verifie si la saisie est bonnne 
    //sinon l'utilisateur repète la saisie
    menu3();
	  controleSaisie(&ch);
    viderBuffer();
    clrscr();
   switch(ch)
   {
	         case 1:
	          {
            //on envoie une demande au serveur pour dire qu'on veut acceder à la suppression des messages
	 	      	Emission("Delete_CB_msg\n");
	 	      	message=Reception();
	 	        //recupère le nombre de fichier 
	 	      	nombre=atoi(message);
                 
	 	      	
	 	      	if(nombre!=0)
	 	      	{
                couleur("34");
	 	      	    printf("Liste Des Message \n");
                // on affiche à l'utilisateur les fichiers de son repertoires 
                for(i=0;i<nombre;i++)
                {
                message=Reception();
	 	             printf("Message numero %d :%s \n",i+1,message);
	 	         
                }
                couleur("0");

                do
                {
                  //on demande à l'utilisateur le numero du fichier qu'il desire supprimé.
                  // on s'assure que ce numero est bien dans la liste de message affichée 
                printf("Entrer le numero du message que vous desirez supprimé \n");
	 	            controleSaisie(&nb);
	 	          
	 	            }while(nb<1 || nb >nombre);
	 	        
	 	             clrscr();
                 // on recupère le numero du message qu'on envoie au serveur 
	 	             sprintf(c,"%d",nb);
	 	             strcat(c,"\n");
	 	             Emission(c);
                 i=0;
    
                 message=Reception();
                 //reponse du serveur si tout c'est bien passée 
                 if(!strcmp(message,"OK_Delete_msg\n"))
                 printf("message supprimé \n");
                 else
                 printf("message n'existe pas \n");
                    
    

	               
              }
            else 
             printf("Vous boite de reception est vide \n");
             break;
         }
         case 2:
         {   //emission de la requête pour demander à supprimer les messages de la boite d'envoie 
              Emission("Delete_CS_msg\n");
              message=Reception();
          
              nombre=atoi(message);
                 
            // liste les messages de la boite 
              if(nombre!=0)
              {
                couleur("34");
                printf("Liste Des Message \n");
                for(i=0;i<nombre;i++)
                {
                message=Reception();
                printf("Message numero %d :%s \n",i+1,message);
             
                }
                couleur("0");
                do
                {
                printf("Entrer le numero du message que vous desirez supprimé \n");
                controleSaisie(&nb);
              
              }while(nb<1 || nb >nombre);
            
               clrscr();
               sprintf(c,"%d",nb);
               strcat(c,"\n");
               Emission(c);
    
                message=Reception();
                //message reçu si la suppression c'est bien passé 
                if(!strcmp(message,"OK_Delete_msg\n"))
                printf("message supprimé \n");
                else
                printf("message n'existe pas \n");
                       

                 
                }
                else 
                printf("Vous boite d'envoie  est vide \n");
                break;

          }
          case 3:
          {
            //requête pour acceder au fichier de la corbeille du client 
          Emission("Delete_Ccor_msg\n");
            message=Reception();
          
            nombre=atoi(message);
                 
            // liste les messages de la corbeille
            if(nombre!=0)
            {
                couleur("34");
                printf("Liste Des Message de la Corbeille \n");
                for(i=0;i<nombre;i++)
                {
                 message=Reception();
                 printf("Message numero %d :%s \n",i+1,message);
             
                }
                couleur("0");

            do
            {
            printf("Entrer le numero du message que vous voulez restaurer\n");
            controleSaisie(&nb);
              
            }while(nb<1 || nb >nombre);
            
            clrscr();
            sprintf(c,"%d",nb);
            strcat(c,"\n");
            Emission(c);
            i=0;
    
             message=Reception();
          if(!strcmp(message,"BACKUP_Delete_Msg\n"))
          printf("Message restaurer \n");
          else
          printf("message n'existe pas \n");
                    
    


                  
            }
            else 
            printf("Vous boite d'envoie  est vide \n");
            break;
           default:
           {
            printf("suivez le menu \n");
            break;
           }
          }

          

         }
         }while(ch!=4);
	 	   


}
