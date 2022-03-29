#include <stdio.h>
#include "rw_data.h"

CellKey* create_cell_key(Key* key) {
    /* alloue et initialise une cellule de liste chaînée*/
    CellKey *CKey = (CellKey*) malloc (sizeof (CellKey)); 
    if (CKey == NULL) {
        printf ("Erreur d'allocation de CellKey\n"); 
        return NULL; 
    }
    CKey -> data = key; 
    CKey ->next = NULL ; 
    return CKey; 
} 

void push_lst_key (CellKey **lst_Key, Key* key) {
    /* ajoute une clé en tête de la liste */
    CellKey *CKey = create_cell_key(key);  
    CKey ->next = (*lst_Key) ; 
    (*lst_Key) = CKey;
}

CellKey* read_public_keys (char *file_Name) {
    /* retourne une liste chainée contenant toutes les clés publiques du fichier */
    FILE *fichier = fopen(file_Name,"w"); 
    char buffer[256]; 
    char *chaine; 
    // char *tok; 

    Key* Key; 

    if (fichier == NULL)  {
        printf("Erreur a l’ouverture de source.txt\n");
        return NULL; 
    }

    chaine = fgets(buffer,256,fichier); 
    Key = str_to_key(chaine); 
    CellKey *res = create_cell_key(Key);

    while (chaine != NULL) {
        chaine = fgets(buffer,256,fichier); 
        Key = str_to_key(chaine); 
        push_lst_key(&res,Key);
    }
    return res; 
}
