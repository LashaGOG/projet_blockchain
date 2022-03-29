#include <stdio.h>
#include <stdlib.h>
#include "generate_data.h"

void generate_random_data(int nv, int nc) 
{
    /* génère nv couples de clés (publique, secrète) différents représentant les nv citoyens */

    Key tab_couple[nv][2];
        for(int i=0;i<nv;i++){
            init_pair_keys(&tab_couple[i][0],&tab_couple[i][1],5,15);
            for(int j=0;j<i;j++){
                if((tab_couple[i][0].val==tab_couple[j][0].val && tab_couple[i][0].n==tab_couple[i][0].n) || (tab_couple[i][1].val==tab_couple[j][1].val && tab_couple[i][1].n==tab_couple[j][1].n)){
                    i--;
                    continue;
                }
            }
        }
    
    /* crée un fichier keys.txt contenant tous ces couples de clés (un couple par ligne) */
    FILE *fichier = fopen("keys.txt","w");
	for(int i=0;i<nv;i++){
		fprintf(fichier,"%s %s\n",key_to_str(&tab_couple[i][0]),key_to_str(&tab_couple[i][1]));
	}
	fclose(fichier);
	
	Key tab_candidat[nc][2];
	int alea[nc];
	for(int i=0;i<nc;i++){
		alea[i] = rand()%nv;
		for(int j=0;j<i;j++){
			if(alea[j]==alea[i]){
				i--;
				continue;
			}
		}
		tab_candidat[i][0].val=tab_couple[alea[i]][0].val;
		tab_candidat[i][1].val=tab_couple[alea[i]][1].val;
		tab_candidat[i][0].n=tab_couple[alea[i]][0].n;
		tab_candidat[i][1].n=tab_couple[alea[i]][1].n;
	}
    /* sélectionne nc clés publiques aléatoirement pour définir les nc candidats et 
    crée un fichier candidates.txt contenant la clé publique de tous les candidats (une clé publique par ligne) */
	FILE *fichier2 = fopen("candidates.txt","w");
	for(int i=0;i<nc;i++){
		fprintf(fichier2,"%s %s\n",key_to_str(&tab_candidat[i][0]),key_to_str(&tab_candidat[i][1]));
	}
	fclose(fichier2);
	
    /* génère une déclaration de vote signée pour chaque citoyen (candidat choisi aléatoirement) */
	Protected *tab_vote[nv];
	int candidalea;
	for(int i=0;i<nv;i++){
		candidalea=rand()%nc;
		tab_vote[i]=init_protected(&tab_couple[i][0], key_to_str(&tab_candidat[candidalea][0]), sign(key_to_str(&tab_candidat[candidalea][0]), &tab_couple[i][1]));
	}
	
    /* crée un fichier declarations.txt contenant toutes les déclarations signées (une déclaration par ligne) */
	FILE *fichier3 = fopen("declarations.txt","w");
	for(int i=0;i<nv;i++){
		fprintf(fichier,"%s\n",protected_to_str(tab_vote[i]));
	}
	fclose(fichier3);


    


}