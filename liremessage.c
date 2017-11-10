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
void menu7()
{
  couleur("34");
  printf("\t \t 1.POUR LISTER LES MESSAGES REÇU \n");
  printf("\t \t 2.POUR LISTER LES MESSAGES ENVOYES \n");
  printf("\t \t 3.Pour quitter \n");
  couleur("0");         
      
  
}
/***************************************************
cette fonction permet à l'utilisateur de consulter 
les messages dans ses boites de reception et envoie 
on propose à l'utilisateur un menu ou il peut effectuer 
le choix du message qu'il desire lire on effectue 
notre choix le serveur nous renvoie 
la liste des message contenu dans la boite sinon 
nous renvoie rien si on a la liste on effectue notre choix 
sur le fichier qu'on desire lire ensuite le serveur nous 
renvoie son contenu 

****************************************************/

void lireMesboite()
{
	char *message;
	char c[3];
	            
	int nombre,i,nb;
	int  ch;
	int option,opt;
	do
	{    menu7();
	    controleSaisie(&ch);
	    clrscr();
	 	  if(ch==1)
	 	      {
             menu7();
	 	      	Emission("READ_C_MSG_Num\n");
     
            message=Reception();
          if(!strcmp(message,"OK_READ_S_MSG\n"))
          {
	 	      	message=Reception();
	 	      
	 	      	nombre=atoi(message);
                 
	 	      	couleur("34");
	 	      	if(nombre!=0)
	 	      	{
	 	      	printf("Liste Des Message \n");
                 
            for(i=0;i<nombre;i++)
            {
                 message=Reception();
  	 	           printf("message num %d  : %s \n",i+1,message);
	 	         
            }
                couleur("0");
            
                
                 do
                {
                

                printf("Entrer le numero du message que vous desirez lire \n");
                 
                controleSaisie(&nb);
                               
                 
                 } while (nb<1 || nb>nombre);              
                  

                  Emission("List_Nb_Msg_C\n");
                  sprintf(c,"%d",nb);
                  strcat(c,"\n");

                  Emission(c);
                 
	 	          
	 	        
	 	        
	 	         clrscr();
	 	         i=0;
          
              do
              {
                    message=Reception();
                    couleur("34");
                    if(i==0 && strcmp(message,"Ok_List_S_Msg\n")!=0)
                    printf("EXPEDITEUR: %s\n",message);
                    else  if(i==1 && strcmp(message,"Ok_List_S_Msg\n")!=0)
                    printf("objet: %s\n",message);
                    else if(strcmp(message,"Ok_List_S_Msg\n")!=0)
                    printf("%s ",message);
                    i++;

                   
                    
               }while((int)strcmp(message,"Ok_List_S_Msg\n")!=0);
                 
                         couleur("0");
              
	               
              }
              else 
              printf("Vous boite de reception est vide \n");
          }

	 	      }
	 	      else if(ch==2)
	 	      {
                Emission("READ_C_MSG_SENT\n");
                message=Reception();
                if(!strcmp(message,"OK_READ_S_MSG_SENT\n"))
                {
                  message=Reception();
	 	               
	 	      	      nombre=atoi(message);
                 

	 	      	    if(nombre!=0)
	 	      	    {
	 	      	       printf("Liste Des Message \n");
                   for(i=0;i<nombre;i++)
                  {
                   message=Reception();
                   couleur("34");
	 	               printf("message num %d  : %s \n",i+1,message);
	 	               couleur("0");
                  }
                do
                {
                

                printf("Entrer le numero du message que vous desirez lire \n");
                 
                controleSaisie(&nb);
                               
                 
              } while (nb<1 || nb >nombre);              
                
                  Emission("List_Nb_Msg_C\n");
                  sprintf(c,"%d",nb);
                  strcat(c,"\n");
                  Emission(c);
                
                  
              
	 	          
	 	            
	 	             clrscr();                 
	 	             sprintf(c,"%d",nb);
	 	             strcat(c,"\n");
	 	             Emission(c);
	 	             i=0;

                 
                 do
                 {
                 	  message=Reception();
                 	  couleur("34");
                    if(i==0 && strcmp(message,"Ok_List_S_Msg\n")!=0)
                    printf("destinataire: %s\n",message);
                    else  if(i==1 && strcmp(message,"Ok_List_S_Msg\n")!=0)
                    printf("objet: %s\n",message);
                    else if(strcmp(message,"Ok_List_S_Msg\n")!=0)
                    printf("%s ",message);
                    i++;

                   
                    
               }while((int)strcmp(message,"Ok_List_S_Msg\n")!=0);
                    couleur("0");
                    printf("\n\n\n\n\n");
	             
               }
               else 
               	printf("Vous boite d'envoie  est vide \n");

	 	      	}
	 	 }
	 	}while(ch!=3);
	 	      

}
