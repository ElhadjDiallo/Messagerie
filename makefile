#makefile pile 
CC=gcc
OPT= -g
EXEC2=Serveur
EXEC=Client

all : $(EXEC)   $(EXEC2) 
#compile testunitaire de initialiser et ajouter ainsi que afficher et supprimer
liremessage: liremessage.c
	 $(CC) -o liremessage.o -c liremessage.c $(OPT)
connecter.o: connecter.c
	 $(CC) -o connecter.o -c connecter.c $(OPT)           
#compile testunitaire du module c de ma listecirculaire.c
serveur.o: serveur.c
	$(CC) -o serveur.o -c serveur.c $(OPT)
#compile testunitaire du module c du fichier de gestionfichier.c
suppression.o: suppression.c
	$(CC) -o suppression.o -c suppression.c $(OPT)
gestionfichier.o: gestionfichier.c
	$(CC) -o gestionfichier.o -c gestionfichier.c $(OPT)
#compile mainServeur.c
mainserveur.o: mainserveur.c
	$(CC) -o mainserveur.o -c mainserveur.c $(OPT)
#link 
client.o: client.c
	$(CC) -o client.o -c client.c $(OPT)

$(EXEC):   gestionfichier.o serveur.o  mainserveur.o
	$(CC)  gestionfichier.o serveur.o mainserveur.o -o $(EXEC2)
$(EXEC2):  liremessage.o connecter.o suppression.o client.o  mainclient.o
	$(CC)  liremessage.o connecter.o suppression.o client.o mainclient.o -o $(EXEC)		
	

clean:
	rm -rf *.o *.exe *.out $(EXEC) $(EXEC2)

