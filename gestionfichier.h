char* chercherRepertoire(char *repertoire,char *adresse,char *sousrep);
void creationDelaBoiteMail(char *nom);
char **  retournerTousLesfichier(char *repertoire);
int   nombredefichier(char *repertoire);
char *recupererFichier(char *chaine);
int ecritureFichier(char *chemin,char *fichier,char *donnee);
int deplacerUnFichier(char *chemin,char *chemin2,char *fichier,char *boite);
int deplacerUnFichierbis(char *chemin,char *chemin2,char *fichier_in,char *fic_out);
int adresseRestauration(char *chemin,char *fichier,char *nom);
char *recupererFichier2(char *chaine);