#include <stdlib.h>
#include <stdio.h>
#include "keys.h"



void init_key(Key *key, long val, long n)
{
    /* permet d'initialiser une clé déjà allouée */
    key->val = val;
    key->n = n;
}

void init_pair_keys(Key *pKey, Key *sKey, long low_size, long up_size)
{
    /* Utilise le protocole RSA pour initialiser une clé publique et une clé secrète (déjà allouées). */
    long p = random_prime_number(low_size, up_size, 5000);
    long q = random_prime_number(low_size, up_size, 5000);

    while (p == q)
    {
        q = random_prime_number(low_size, up_size, 5000);
    }
    long n, s, u;
    generate_keys_values(p, q, &n, &s, &u);
    // pour avoir les clés positives
    if (u < 0)
    {
        long t = (p - 1) * (q - 1);
        u = u + t; // on aura toujours s*u mod t = 1
    }
    // initialisation des clés
    init_key(pKey, s, n);
    init_key(sKey, u, n);
}

char *key_to_str(Key *key)
{
    char *c1 = (char *)malloc(20 * sizeof(char));
    char *c2 = (char *)malloc(20 * sizeof(char));

    sprintf(c1, "(%lx,", key->val);
    sprintf(c2, "%lx)", key->n);

    int taille = strlen(c1);
    taille += strlen(c2);

    char *chaine = (char *)(malloc((taille + 1) * sizeof(char)));
    strcat(chaine, c1);
    strcat(chaine, c2);

    free(c1);
    free(c2);

    return chaine;
}

Key *str_to_key(char *str)
{
    // on recupere les valeurs d'une clé et les stocke dans les variables val, n
    unsigned long val, n;
    sscanf(str, "(%lx,%lx)", &val, &n);

    // allocation et initialsation d'une clée à retourner
    Key *clef = (Key *)(malloc(sizeof(Key)));
    init_key(clef, val, n); //

    return clef;
}

Signature *init_signature(long *content, int size)
{
    /* permet d'allouer et de remplir une signature avec un tableau de long déjà alloué et initialisé */

    Signature *sign = (Signature *)malloc(sizeof(Signature));
    sign->taille = size;
    sign->tab = (long *)(malloc(size * sizeof(long)));

    // remplir le tableau
    for (int i = 0; i < size; i++)
    {
        sign->tab[i] = content[i];
    }
    return sign;
}

Signature *sign(char *mess, Key *sKey)
{
    /* crée une signature à partir du message mess (déclaration de vote) et de la clé secrète de l’émetteur */
    int taille = strlen(mess);
    long *tab = encrypt(mess, sKey->val, sKey->n);
    Signature *s = init_signature(tab, taille);
    return s;
}

char *signature_to_str(Signature *sgn)
{
    char *result = malloc(10 * sgn->taille * sizeof(char));
    result[0] = '#';
    int pos = 1;
    char buffer[156];
    for (int i = 0; i < sgn->taille; i++)
    {
        sprintf(buffer, "%lx", sgn->tab[i]);
        for (int j = 0; j < strlen(buffer); j++)
        {
            result[pos] = buffer[j];
            pos = pos + 1;
        }
        result[pos] = '#';
        pos = pos + 1;
    }
    result[pos] = '\0';
    result = realloc(result, (pos + 1) * sizeof(char));
    return result;
}

Signature *str_to_signature(char *str)
{
    int len = strlen(str);
    long *content = (long *)malloc(sizeof(long) * len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for (int i = 0; i < len; i++)
    {
        if (str[i] != '#')
        {
            buffer[pos] = str[i];
            pos = pos + 1;
        }
        else
        {
            if (pos != 0)
            {
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num = num + 1;
                pos = 0;
            }
        }
    }
    content = realloc(content, num * sizeof(long));
    return init_signature(content, num);
}

Protected *init_protected(Key *pKey, char *mess, Signature *sgn)
{
    /* alloue et initialise la structure Protected */

    Protected *protec = (Protected *)(malloc(sizeof(Protected)));
    protec->pKey = pKey;
    protec->signature = sgn;
    protec->message = strdup(mess);

    return protec;
}

int verify(Protected *pr)
{
    /* vérifie que la signature contenue dans pr correspond bien au message et à la personne contenus dans pr */

    char *messdec = decrypt(pr->signature->tab, strlen(pr->message), pr->pKey->val, pr->pKey->n);
    if (strcmp(messdec, pr->message) == 0)
        return 1;

    return 0;
}

char *protected_to_str(Protected *pr)
{
    /* permet de passer d’un Protected à sa représentation sous forme de chaîne de caractères */

    char *c1 = key_to_str(pr->pKey);
    char *c2 = signature_to_str(pr->signature);
    int taille = strlen(c1) + strlen(c2);
    taille += strlen(pr->message);

    char *chaine = (char *)malloc((taille + 3) * sizeof(char)); // on écrit taille +  3 pour mettre les espaces
    strcat(chaine, c1);
    strcat(chaine, " ");
    strcat(chaine, pr->message);
    strcat(chaine, " ");
    strcat(chaine, c2);

    return chaine;
}

Protected *str_to_protected (char *chaine)  
{
    /* permet de passer d'une chaine de caractères à un Protected */
    
    int i=0;
	while(chaine[i]!=' '){
		i++;
	}
	char c1[i+1];
	for(int k=0;k<i;k++){
		c1[k]=chaine[k];
	}
	c1[i]='\0';
	
	int j=i+1;
	while(chaine[j]!=' '){
		j++;
	}
	char c2[j-i+1];
	for(int k=i+1;k<j;k++){
		c2[k]=chaine[k];
	}
	c2[j-i]='\0';
	
	i=j+1;
	while(chaine[i]!='\0'){
		i++;
	}
	char c3[i-j+1];
	for(int k=j+1;k<i;k++){
		c3[k]=chaine[k];
	}
	c3[i-j]='\0';

    Protected *pr = (Protected *) malloc (sizeof (Protected)); 
    pr -> pKey = str_to_key (c1); 
    pr -> message = strdup (c2); 
    pr -> signature = str_to_signature (c3); 

    return pr; 
}

// int main ( void ) {
//     srand ( time ( NULL ) ) ;
// 	//Testing Init Keys
// 	Key * pKey = malloc ( sizeof ( Key ) ) ;
// 	Key * sKey = malloc ( sizeof ( Key ) ) ;
// 	init_pair_keys ( pKey , sKey ,3 ,7) ;
// 	printf ("pKey: %lx, %lx\n", pKey->val, pKey->n);
// 	printf ("sKey: %lx, %lx\n", sKey->val, sKey->n);
// 	//Testing Key Serialization
// 	char * chaine = key_to_str ( pKey ) ;
// 	printf ("key to str:%s\n", chaine);
// 	Key * k = str_to_key ( chaine ) ;
// 	printf ("str to key: %lx, %lx\n",k->val, k->n) ;

// 	//Testing signature
// 	//Candidate keys:
// 	Key * pKeyC = malloc ( sizeof ( Key ) ) ;
// 	Key * sKeyC = malloc ( sizeof ( Key ) ) ;
// 	init_pair_keys ( pKeyC , sKeyC ,3 ,7) ;
// 	//Declaration:
// 	char * mess = key_to_str ( pKeyC ) ;
// 	printf ("%s votepour %s\n" , key_to_str ( pKey ) , mess ) ;
// 	Signature *sgn = sign ( mess , sKey ) ;
// 	printf ("signature:") ;
// 	print_long_vector ( sgn->tab, sgn->taille) ;
// 	chaine = signature_to_str ( sgn ) ;
// 	printf ("signature to str:%s \n" , chaine ) ;
// 	sgn = str_to_signature ( chaine ) ;
// 	printf ("str to signature:") ;
// 	print_long_vector (sgn->tab, sgn->taille) ;
// 	//Testing protected:
// 	Protected * pr = init_protected ( pKey , mess , sgn ) ;
// 	//Verification:
// 	if (verify(pr)) {
// 		printf ("Signature valide\n");
// 	} 
// 	else {
// 		printf ("Signature non valide\n");
// 	}
// 	chaine = protected_to_str ( pr ) ;
// 	printf ("protected to str:%s\n",chaine);
// 	pr = str_to_protected ( chaine ) ;
// 	printf ( "str to protected: %s %s %s \n" , key_to_str(pr->pKey), pr->message ,signature_to_str (pr->signature));
// 	free ( pKey ) ;
// 	free ( sKey ) ;
// 	free ( pKeyC ) ;
// 	free ( sKeyC ) ;
// 	return 0;
// }

