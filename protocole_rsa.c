#include <stdio.h>
#include "protocole_rsa.h"

long extended_gcd (long s, long t, long *u, long *v) 
{
    if (s == 0) 
    {
        *u = 0; 
        *v = 1; 
        return t; 
    }
    long uPrim, vPrim ; // Pour stocker les valeurs obtenus avec les appels recursifs
    long gcd = extended_gcd (t%s, s, &uPrim, &vPrim); 
    
    // mise à jour des valeurs de u et v en utilisant les resultats des appels recursifs
    *u = vPrim -(t/s)*uPrim; 
    *v = uPrim; 

    return gcd ; 
}

void generate_keys_values (long p, long q, long* n, long* s, long* u) 
{
    /* generer la cle publique pKey = (s,n) et la cle secrete sKey = (u,n) a partir des nombres premiers p et q en suivant le protocole RSA */ 

    *n = p * q; 
    long t = (p-1) * (q-1);
    
    long v; 
    // generer l'entier s inferieur à t jusqu'à en trouver un tel que PGCD(s, t) = 1
    
    srand(time(NULL)); 
    *s = rand() % t; 
    while (extended_gcd(*s, t, u, &v) != 1) 
    {
        *s = rand() % t;
    }
}

long *encrypt (char *chaine, long s, long n) 
{
    /* chiffre la chaine de caractère chaine avec la clé publique (s,n) */
    int taille = strlen (chaine); 
    long *msg_chiffre = (long*) malloc (taille * sizeof(long)); 
    int i = 0, j = 0 ; 
    while (chaine[i] != '\0') 
    {
        msg_chiffre[j] = mod_pow(chaine[i],s,n);
        j++; 
        i++;

    }
    return msg_chiffre; 
}
char *decrypt (long* crypted, int size, long u, long n) 
{
    /* dechiffre le msg chiffré avec la clé privée */
    char *msg = (char*)malloc ((size+1)*sizeof(char)); 
    for (int i = 0; i < size; i++) {
        printf("%d\n",mod_pow(crypted[i],u,n));
        msg[i] = (char)(mod_pow(crypted[i],u,n)) ;
    }
    msg[size] = '\0'; 
    return msg ; 
}
void print_long_vector (long *result, int size) 
{
    printf ("Vecteur: [");
    for (int i = 0; i < size; i++) 
    {
        printf ("%lx\t",result[i]);
    }
    printf("]\n");
}