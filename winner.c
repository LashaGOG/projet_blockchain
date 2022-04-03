#include <stdio.h>
#include "winner.h"

void remove_fraud(CellProtected **LCP) {
    /* etant donnée une liste chaînée de déclarations signées, supprime toutes les déclarations dont la signature n’est pas valide */
    if (!(*LCP)) return ; // si la liste est vide rien à faire
    
    CellProtected *prec = *LCP; 
    CellProtected *temp = *LCP; 
    while (temp) {
        if (! verify(temp->data)) {
            if (temp ->next) { // si la déclaration à supprimer n'est pas à la fin de la liste
                prec->next = temp ->next; 
                delete_cell_protected(temp);
            }
            else { // si la déclaration à suppromer est à la fin de la liste
                delete_cell_protected(temp); 
            }
        }
        if (temp -> next) {
            prec = temp ; 
            temp = temp -> next;
        }

    }
}

HashCell *create_hashcell (Key *key) {
    /* alloue une cellule de la table de hachage, et initialise ses champs en mettant la valeur à zéro */
    HashCell *hCell = (HashCell *) malloc (sizeof(HashCell)) ; 
    if (!hCell) {
        printf ("Erreur d'allocation\n"); 
        return NULL; 
    }
    hCell ->key = key; 
    hCell ->val = 0; 

    return hCell; 
}

int hash_function(Key* key, int size) {
    /* retourne la position d’un élément dans la table de hachage, inspirée de TME 2 */
    
    double A = (sqrt(5) - 1)/2 ; //nombre d’or diminué de 1
    long k = key -> val; 
    int pos = (int)(size * (k*A - (int)(k*A))); 

    return pos;
}

int find_position(HashTable* t, Key* key) {
    /* cherche dans la table t s’il existe un  élément dont la clé publique est key */
    int i = hash_function (key, t -> size); 
    // Si l’élément a été trouvé, la fonction retourne sa position dans la table,
    if ( (t->tab[i]->key ->n == key ->val) || (t->tab[i]->key ->val == key ->val) ) return i ; 
    // sinon on cherche la position où il aurait dû etre, qui est la case la plus proche qui est libre si la case est déjà occupé; 
    while (t -> tab[i] != 0) {
        i++; 
    } 
    return i ; 
}

HashTable* create_hashtable(CellKey* keys, int size) {
    /* crée et initialise une table de hachage de taille size contenant une cellule pour chaque clé de la liste chainée keys */
    if (!keys) return NULL; 
    
    int i;
    HashTable *hTable = (HashTable *) malloc (sizeof(HashTable)) ; 
    if (!hTable) {
        printf ("Erreur d'allocation \n"); 
        return NULL ;
    }
    hTable ->size = size ; 
    CellKey *temp = keys; // parcoureur
    // on cherche et insere chaque clé à sa position
    do {
        i = find_position(hTable, keys->data);
        hTable ->tab[i] = create_hashcell(keys ->data) ;  
        temp = temp->next ; 
    } while (temp) ; 

    return hTable; 
}

void delete_hashcell (HashCell* hc) {
    /* supprime une cellule de clé */
    free(hc ->key);
    free (hc); 
}

void delete_hashtable(HashTable* t) {
    /* supprime une table de hachage */
    if (!t) return; 
    for (int i = 0; i < t -> size; i++) {
        delete_hashcell(t->tab[i]); 
    }
    free (t->tab);
    free(t); 
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
    /* calcule le vainqueur de l’élection étant donnés une liste de déclarations avec signatures valides (decl), une liste de candidats (candidates), et une liste de personnes autorisées à voter (voters) */

    // On commence par création de deux tables de hachage
    // Hc : une table de hachage (de taille sizeC) pour la liste des candidats
    HashTable *Hc = create_hashtable (candidates,sizeC); 
    // Hv : une table de hachage (de taille sizeV) pour la liste des votants
    HashTable *Hv = create_hashtable (voters,sizeV); 

    // parcourir la liste des déclarations
    CellProtected *temp = decl; //parcoureur 
    int i,j;
    
    remove_fraud(&decl); // supprime fraudes

    while (temp) {
        // on vérifie : que la personne qui vote a le droit de voter et qu’elle n’a pas déjà voté
        i = find_position(Hv,temp ->data ->pKey); 
        if ( (Hv ->tab[i] ->key != temp ->data ->pKey) || (Hv -> tab[i] ->val != 0) ) {
            break;
        } 
        // on vérifie : que la personne sur qui porte le vote est bien un candidat de l’élection
        Key *kCand = str_to_key(temp->data ->message); 
        j = find_position(Hc,kCand);
        if (Hc ->tab[i]->key  != kCand) {
            break;
        }
        // Si toutes ces conditions sont vérifiées, le vote est comptabilisé dans la table Hc et la table Hv est mise à jour pour indiquer que ce votant vient de vote
        Hc ->tab[j] ->val ++; 
        Hv ->tab[i] ->val ++ ; 
        free(kCand); 
        temp = temp ->next; 
    }

    // Une fois que tous les votes ont etre comptabilisés, la fonction détermine le gagnant en utilisant la table Hc
    // Question : Si 2 candidats avec les memes votes à prendre en compte? 
    HashCell *cand_winner = Hc ->tab[0];   ; 
    for (int k = 1; k < sizeC; k++ ) {
        if (Hc -> tab[k] ->val > cand_winner ->val) {
            cand_winner = Hc ->tab[k] ; 
        }
    }
    return cand_winner->key; 
}