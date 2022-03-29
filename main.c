#include <stdio.h>
#include <stdlib.h>

#include "rw_data.h"
#include "protocole_rsa.h" 
#include "primalite.h" 
#include "keys.h" 
#include "generate_data.h"

void test_modpow () {
	clock_t temps_initial;
	clock_t temps_final;
	double temps1, temps2;
	long a = 5821352136521;
	long m;
	long n = 6525251414;
	char nom[10] = "temps.txt";
	FILE *fichier = fopen(nom, "w");
	for (m = 10000000; m <= 300000000; m += 10000000)
	{
		temps_initial = clock();
		mod_pow_naive(a, m, n);
		temps_final = clock();
		temps1 = (double)(temps_final - temps_initial) / CLOCKS_PER_SEC;

		temps_initial = clock();
		mod_pow(a, m, n);
		temps_final = clock();
		temps2 = (double)(temps_final - temps_initial) / CLOCKS_PER_SEC;

		fprintf(fichier, "%ld %lf %lf\n", m, temps1, temps2);
	}
	fclose(fichier);
}

void test_protocole_rsa () {
    srand(time(NULL));
    //Generation de cle :
    long p = random_prime_number(3,7, 5000);
    long q = random_prime_number(3,7, 5000);
    while(p==q) 
    {
        q = random_prime_number(3,7, 5000);
    }
    long n, s, u;
    generate_keys_values(p,q,&n,&s,&u);
    //Pour avoir des cles positives :
    if (u<0)
    {
        long t = (p-1)*(q-1);
        u = u+t; //on aura toujours s*u mod t = 1
    }
    //Afichage des cles en hexadecimal
    printf("cle publique = (%lx , %lx ) \n",s,n);
    printf("cle privee = (%lx , %lx) \n",u,n);

    //Chiffrement:
    char mess[10] = "Hello";
    int len = strlen(mess);
    long* crypted = encrypt(mess, s, n); 

    printf ("Initial message : %s \n",mess);
    printf("Encoded representation : \n");
    print_long_vector(crypted, len);

    //Dechiffrement:

    char* decoded = decrypt(crypted,len, u, n);
    printf("Decoded : %s\n", decoded); 
}

void test_keys () {
    
    srand ( time ( NULL ) ) ;
	//Testing Init Keys
	Key * pKey = malloc ( sizeof ( Key ) ) ;
	Key * sKey = malloc ( sizeof ( Key ) ) ;
	init_pair_keys ( pKey , sKey ,3 ,7) ;
	printf ("pKey: %lx, %lx\n", pKey->val, pKey->n);
	printf ("sKey: %lx, %lx\n", sKey->val, sKey->n);
	//Testing Key Serialization
	char * chaine = key_to_str ( pKey ) ;
	printf ("key to str:%s\n", chaine);
	Key * k = str_to_key ( chaine ) ;
	printf ("str to key: %lx, %lx\n",k->val, k->n) ;

	//Testing signature
	//Candidate keys:
	Key * pKeyC = malloc ( sizeof ( Key ) ) ;
	Key * sKeyC = malloc ( sizeof ( Key ) ) ;
	init_pair_keys ( pKeyC , sKeyC ,3 ,7) ;
	//Declaration:
	char * mess = key_to_str ( pKeyC ) ;
	printf ("%s votepour %s\n" , key_to_str ( pKey ) , mess ) ;
	Signature *sgn = sign ( mess , sKey ) ;
	printf ("signature:") ;
	print_long_vector ( sgn->tab, sgn->taille) ;
	chaine = signature_to_str ( sgn ) ;
	printf ("signature to str:%s \n" , chaine ) ;
	sgn = str_to_signature ( chaine ) ;
	printf ("str to signature:") ;
	print_long_vector (sgn->tab, sgn->taille) ;
	//Testing protected:
	Protected * pr = init_protected ( pKey , mess , sgn ) ;
	//Verification:
	if (verify(pr)) {
		printf ("Signature valide\n");
	} 
	else {
		printf ("Signature non valide\n");
	}
	chaine = protected_to_str ( pr ) ;
	printf ("protected to str:%s\n",chaine);
	pr = str_to_protected ( chaine ) ;
	printf ( "str to protected: %s %s %s \n" , key_to_str(pr->pKey), pr->message ,signature_to_str (pr->signature));
	free ( pKey ) ;
	free ( sKey ) ;
	free ( pKeyC ) ;
	free ( sKeyC ) ;
}

void test_rw_data () {
    printf ("Test de la fonction : rw_data réussi. \n");
}

int main () {
    test_rw_data(); 
    return 1; 
}