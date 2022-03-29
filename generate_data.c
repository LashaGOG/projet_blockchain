#include <stdio.h>
#include <stdlib.h>
#include "generate_data.h"

void generate_random_data(int nv, int nc) 
{
    /* génère nv couples de clés (publique, secrète) différents représentant les nv citoyens */

    Key tab_pKeys[nv];
    Key tab_sKeys[nv];

    for (int i = 0; i<nv; i++) 
    {
        init_pair_keys ( &tab_pKeys [i] , &tab_sKeys [i] ,3 ,7) ;
    }
    /* pour voir le pb; dans le tab, tous les clefs sotn les memes, à part le premier*/
    for (int i = 0; i< nv; i++) 
    {
        printf ("%s\t",key_to_str(&tab_pKeys[i])); 
        printf ("%s\n",key_to_str(&tab_sKeys[i])); 
    }
    
    /* crée un fichier keys.txt contenant tous ces couples de clés (un couple par ligne) */

    FILE *fichier = fopen ("keys.txt","w"); 
    
    char *c1, *c2 ;

    for (int i =  0; i < nv; i ++)
    {   
        c1 = strdup (key_to_str (&tab_pKeys[i]));
        c2 = strdup (key_to_str (&tab_sKeys[i]));
        fprintf (fichier,"%s\t",c1); 
        fprintf (fichier,"%s\n",c2); 
        free(c1); 
        free(c2);
    }
    // fclose(fichier); 
    /* sélectionne nc clés publiques aléatoirement pour définir les nc candidats et 
    crée un fichier candidates.txt contenant la clé publique de tous les candidats (une clé publique par ligne) */
    int j ;
    char *cand; 
    FILE *fichier_Cand = fopen ("candidates.txt","w");    
    for (int i = 0; i < nc; i++)
   {
       j = nv % rand(); 
       cand = strdup (key_to_str (&tab_pKeys[j]));
       fprintf (fichier_Cand,"%s\n",cand);
   } 
   fclose(fichier_Cand);

    /* génère une déclaration de vote signée pour chaque citoyen (candidat choisi aléatoirement) */

    
    /* crée un fichier declarations.txt contenant toutes les déclarations signées (une déclaration par ligne) */


}

// int main () {
//     generate_random_data(100,3); 
//     return 1; 
// }