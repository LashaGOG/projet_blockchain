#Déclarations de variables
#Le compilateur
CC = gcc
#Les options du compilateurs
CFLAGS = -g -Wall
#Liste des programme à créer
PROGRAMS = main

	#Premiere regle: liste des programme à compiler
#Règle sans action, seulement avec des dépendances
all : $(PROGRAMS)

#Règle pour compiler le main
main : main.o primalite.o protocole_rsa.o keys.o generate_data.o rw_data.o 
	$(CC) $(CFLAGS) -o $@ $^ 
main.o : main.c
	$(CC) $(CFLAGS) -c $<
primalite.o : primalite.c primalite.h
	$(CC) $(CFLAGS) -c $<
protocole_rsa.o : protocole_rsa.c protocole_rsa.h
	$(CC) $(CFLAGS) -c $<
keys.o : keys.c keys.h
	$(CC) $(CFLAGS) -c $< 
generate_data.o : generate_data.c generate_data.h
	$(CC) $(CFLAGS) -c $<
rw_data.o : rw_data.c rw_data.h
	$(CC) $(CFLAGS) -c $<

#Effacer les .o et les executables
#Pour executer cette regle il faut taper dans le terminal "make clean"
clean : 
	rm *.o $(PROGRAMS)
