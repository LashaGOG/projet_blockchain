#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "primalite.c"
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
// int main () {
//     srand(time(NULL));
//     //Generation de cle :
//     long p = random_prime_number(3,7, 5000);
//     long q = random_prime_number(3,7, 5000);
//     while(p==q) 
//     {
//         q = random_prime_number(3,7, 5000);
//     }
//     long n, s, u;
//     generate_keys_values(p,q,&n,&s,&u);
//     //Pour avoir des cles positives :
//     if (u<0)
//     {
//         long t = (p-1)*(q-1);
//         u = u+t; //on aura toujours s*u mod t = 1
//     }
//     //Afichage des cles en hexadecimal
//     printf("cle publique = (%lx , %lx ) \n",s,n);
//     printf("cle privee = (%lx , %lx) \n",u,n);

//     //Chiffrement:
//     char mess[10] = "Hello";
//     int len = strlen(mess);
//     long* crypted = encrypt(mess, s, n); 

//     printf ("Initial message : %s \n",mess);
//     printf("Encoded representation : \n");
//     print_long_vector(crypted, len);

//     //Dechiffrement:

//     char* decoded = decrypt(crypted,len, u, n);
//     printf("Decoded : %s\n", decoded); 

//     return 0;
// }