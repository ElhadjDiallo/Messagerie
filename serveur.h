#ifndef __SERVEUR_H__
#define __SERVEUR_H__
#include "leselements.h"
/* Initialisation.
 * Creation du serveur.
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
 
int Initialisation();

/* Initialisation.
 * Creation du serveur en pr�cisant le service ou num�ro de port.
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int InitialisationAvecService(char *service);


/* Attends qu'un client se connecte.
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int AttenteClient();

/* Recoit un message envoye par le client.
 * retourne le message ou NULL en cas d'erreur.
 * Note : il faut liberer la memoire apres traitement.
 */
char *Reception();

/* Envoie un message au client.
 * Attention, le message doit etre termine par \n
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int Emission(char *message);

/* Recoit des donnees envoyees par le client.
 * renvoie le nombre d'octets re�us, 0 si la connexion est ferm�e,
 * un nombre n�gatif en cas d'erreur
 */
int ReceptionBinaire(char *donnees, size_t tailleMax);

/* Envoie des donn�es au client en pr�cisant leur taille.
 * renvoie le nombre d'octets envoy�s, 0 si la connexion est ferm�e,
 * un nombre n�gatif en cas d'erreur
 */
int EmissionBinaire(char *donnees, size_t taille);


/* Ferme la connexion avec le client.
 */
void TerminaisonClient();

/* Arrete le serveur.
 */
void Terminaison();
//gère toutes les tâches que l'utilisateur au cours d'une connexion
void gestionconnexion();
// gère la première connexion de l'utilisateur c'est à dire son inscription 
void inscriptionBasedeDonnee();
// realise la tache de placer les utilisateur dans la base de donnée 
int placerLesUtilisateurDansfichier(char *nom,infos mesInfos);
// recupère une chaine dans une chaine delimiter par un espace
char * recupererPartieChaine(char *message);
// gère la lecture des message 
void lireMessage(infos lesinfos,char *boite);
// permet d'enlever le "\n" dans une chaine 
char * change(char *chaine);
// lit un fichier et envoie son contenu à partir de la fonction emission 
int  lirfic(char *chemin,char *fichier);
// permet d'informer au client qu'il a entrer une mauvais requête 
void gestionDesErreurs();
void suprimertout(infos lesinfos);
#endif
