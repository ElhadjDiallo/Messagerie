#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>

/*******************************************************************
prend un chemin vers un fichier puis le fichier pour completer le chemin 
et recupère l'adresse du client destinataire ou expeditaire dans le fichier 


******************************************************************/
int adresseRestauration(char *chemin,char *fichier,char *nom)
{

FILE*fic;
char chaine[100];    
char *placefichier;
int i=0;
placefichier=malloc(strlen(chemin)+strlen(fichier)+strlen(".txt")+1*sizeof(char));
strcpy(placefichier,chemin);
strcat(placefichier,"/");
strcat(placefichier,fichier);
fic=fopen(placefichier,"r");
    if(fic!=NULL)
    {
        while(fgets(chaine,100,fic)!=NULL)
        {
            
            if(i==0 && strstr(chaine,nom)!=NULL)
            {
                
                return 1;
            }
          i++;         

        }
        fclose(fic);
    }
    return 0;

    
        
}
/******************************************************************************
permet de deplacer un fichier d'un repertoire vers un autre repertoire 


********************************************************************************/
int deplacerUnFichierbis(char *chemin,char *chemin2,char *fichier_in,char *fic_out)
{

     char chaine[100];
     FILE*fic,*fic2;
     char placefichier2[50];
    char *placefichier;
    placefichier=malloc(strlen(chemin)+strlen(fichier_in)+strlen(".txt")+1*sizeof(char));
    strcpy(placefichier,chemin);
    strcat(placefichier,"/");
    strcat(placefichier,fichier_in);
    strcpy(placefichier2,chemin2);

     strcat(placefichier2,"/");

    strcat(placefichier2,fic_out);
    fic=fopen(placefichier,"r");
     fic2=fopen(placefichier2,"w");
    if(fic!=NULL)
    {
        while(fgets(chaine,100,fic)!=NULL)
        {
            fprintf(fic2,"%s",chaine);

        }
        fclose(fic2);
        fclose(fic);

    }

      
}
/*********************************************************************************
deplace un fichier d'un repertoire vers un autre mais cette fois utilise une boite
de reception pour trouver le chemin 
********************************************************************************/

int deplacerUnFichier(char *chemin,char *chemin2,char *fichier,char *boite)
{
    char chaine[100];
     FILE*fic,*fic2;
     char placefichier2[50];
    char *placefichier;
   
    placefichier=malloc(strlen(chemin)+strlen(fichier)+strlen(".txt")+1*sizeof(char));
    strcpy(placefichier,chemin);
    strcat(placefichier,"/");
    strcat(placefichier,fichier);

     strcpy(placefichier2,chemin2);
     strcat(placefichier2,"/");
     strcat(placefichier2,fichier);
    fic=fopen(placefichier,"r");

    fic2=fopen(placefichier2,"w");
    if(fic!=NULL)
    {
        while(fgets(chaine,100,fic)!=NULL)
        {
            fprintf(fic2,"%s",chaine);

        }
        fclose(fic2);
        fclose(fic);

    }
    

}
/**********************************************
fonction qui prend en argument une chaine de 
caratère dans laquelle est concatené un fichier 
et recupère le fichier


********************************************/
char *recupererFichier(char *chaine)
{
    char *fichier;

    int taille,i,trouve,pos,c;
    taille=strlen(chaine);
    i=taille-1;
    trouve=0;
    
    while(i>-1 && ! trouve)
    {
        if(chaine[i]=='/')
        {
            
           pos=i;
           trouve=1;
        }
       i--;
    }
    fichier=malloc(strlen(chaine)-pos+1*sizeof(char));
    c=0;
    for(i=pos+1;i<taille;i++)
    {
     fichier[c]=chaine[i];
     c++;
    }
    fichier[c+1]='\0';
    return fichier; 
}
char *recupererFichier2(char *chaine)
{
    char *fichier;

    int taille,i,trouve,pos,c;
    taille=strlen(chaine);
    i=taille-1;
    trouve=0;

    while(i>-1 && ! trouve)
    {
        if(chaine[i]=='/')
        {
            
           pos=i;
           trouve=1;
        }
       i--;
    }
    fichier=malloc(strlen(chaine)-pos+1*sizeof(char));
    c=0;
    for(i=pos+1;i<=taille;i++)
    {
     fichier[c]=chaine[i];
     c++;
    }
    fichier[c+1]='\0';
    return fichier; 
}

/*************************************
parcours un repertoire et nous renvoie le nombre de fichier 
qui est contenu 

************************************/
int   nombredefichier(char *repertoire)
{

    int nombre=0;
    struct dirent * courant;
    struct stat infos;
    char  nom[220];
    
    DIR *rep;
    if(repertoire[strlen(repertoire)-1]=='/')
        repertoire[strlen(repertoire)-1]='\0';
        rep=opendir(repertoire);
    while((courant=readdir(rep))!=NULL)
    {
        if(strcmp(courant->d_name,".") && strcmp(courant->d_name,".."))
        {               
            strcpy(nom,repertoire);
            strcat(nom,"/");
            strcat(nom,courant->d_name);
            stat(nom,&infos);
            if(S_ISDIR(infos.st_mode))
            {
                nombre=nombredefichier(nom);
                
            }   
            else
            {
                nombre++;
                
            }
            
        }
    }
    closedir(rep);
    

    return nombre;
}
/*****************************************
parcours un repertoire et nous ramène tous les fichiers 
contenu dans le repertoire 


******************************************/
char **  retournerTousLesfichier(char *repertoire)
{

    int nombre=0;
    struct dirent * courant;
    struct stat infos;
    long taille,t;
    char *fichier;
    char  nom[220];
    char **tabChaine;
    tabChaine=(char**)malloc(100*sizeof(char*));    
    DIR *rep;
    if(repertoire[strlen(repertoire)-1]=='/')
        repertoire[strlen(repertoire)-1]='\0';
        rep=opendir(repertoire);
    taille=0;
    while((courant=readdir(rep))!=NULL)
    {
        if(strcmp(courant->d_name,".") && strcmp(courant->d_name,".."))
        {               
            strcpy(nom,repertoire);
            strcat(nom,"/");
            strcat(nom,courant->d_name);
            stat(nom,&infos);
            if(S_ISDIR(infos.st_mode))
            {
                tabChaine=retournerTousLesfichier(nom);
                
            }   
            else
            {
                
       
                fichier=recupererFichier(nom);
                tabChaine[nombre]=malloc(strlen(fichier)*sizeof(char));
                strcpy(tabChaine[nombre],fichier);
                nombre++;
                
            }
            
        }
    }
    closedir(rep);
    

    return tabChaine;
}
/***********************************************
reçoit un chemin vers un fichier en paramètre puis le fichier 
concatène les deux pour former le chemin finale et ensuite 
ecrit les données dans le fichier 

**********************************************/
int ecritureFichier(char *chemin,char *fichier,char *donnee)
{
    FILE*fic;
    char *placefichier;
    placefichier=malloc(strlen(chemin)+strlen(fichier)+strlen(".txt")+1*sizeof(char));
    strcpy(placefichier,chemin);
    strcat(placefichier,"/");
    strcat(placefichier,fichier);
    strcat(placefichier,".txt");
    fic=fopen(placefichier,"a+");
    fprintf(fic,"%s \n",donnee);
     fclose(fic);


}
            
/************************************************************
prend en argument le repertoire courant et le sous repertoire 
c'est à dire boite d'envoie ou boite de reception vu que ce sont des 
dossiers plus l'adresse ou le nom de la personne à qui appartient 
la boite et nous renvoie le chemin vers la boite de reception 
ou la boite d'envoie

***********************************************************/
char * chercherRepertoire(char *repertoire,char *adresse,char *sousrep)
{   
    FILE *fic;

     char *place,*fichier;
    int trouve=0;
struct dirent * courant;
    struct stat infos;
    long taille,t;
    char  nom[120]; 
    DIR *rep;
    if(repertoire[strlen(repertoire)-1]=='/')
        repertoire[strlen(repertoire)-1]='\0';
        rep=opendir(repertoire);
    
    while((courant=readdir(rep))!=NULL && !trouve)
    {
        if( strcmp(courant->d_name,".") && strcmp(courant->d_name,"..") && !trouve)
        {               
            strcpy(nom,repertoire);
            strcat(nom,"/");
            strcat(nom,courant->d_name);
            stat(nom,&infos);
            if(S_ISDIR(infos.st_mode))
             {
                
                
                fichier=recupererFichier2(nom);

                

                
                if(!strcmp(fichier,adresse))
                {
                

                    place=malloc(strlen(nom)+strlen(sousrep)+4*sizeof(char));
                    strcpy(place,"./");
                    strcat(place,nom);
                    strcat(place,"/");
                    strcat(place,sousrep);
                    strcat(place,"/");
                      trouve=1;

                }
                else
               chercherRepertoire(nom,adresse,sousrep);
                
            }   
            
        }
    }
    closedir(rep);

    if(trouve==0)
        return NULL;
    else
        return place;

}
/**********************************************
prend en argument le nom du client et cree un dossier correspondant 
à son nom puis sous dossier reception considéré comme boite de 
reception et autre sous dossier considerer comme boite d'envoie 
***********************************************/
void creationDelaBoiteMail(char *nom)
{


  char boite[100],reception[100],envoie[100],corbeille[100];
	
    strcpy(boite,"./");
    strcat(boite,nom);

    strcpy(reception,boite);
    strcat(reception,"/reception");
    strcpy(envoie,boite);
    strcat(envoie,"/envoie");

    strcpy(corbeille,boite);
    strcat(corbeille,"/corbeille");
	mkdir(boite, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir(reception, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir(envoie, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(corbeille, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
}

