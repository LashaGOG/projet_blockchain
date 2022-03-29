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
    if (fichier == NULL)  {
        printf("Erreur a l’ouverture de source.txt\n");
        return NULL; 
    }

    char buffer[256] ;
    char *line;
    char pKey[256]; //pour stocker la clé publique
    char sKey[256]; //pour stocker la clé sécrete
    CellKey *res = NULL; // liste résultat

    do {
        line = fgets(buffer,256,fichier);
        sscanf (buffer, "%s %s", pKey,sKey); 
        push_lst_key (&res,str_to_key(pKey));
    } while (line) ; 

    fclose(fichier); 
    return res; 
}

void print_list_keys(CellKey* LCK) {
    /* permet d'afficher une liste chaînée de clés */
    if (LCK == NULL) {
        printf ("La liste est vide\n"); 
    }
    CellKey *temp = LCK; 
    char *data; 
    do {
        data = key_to_str (temp->data); 
        printf ("%s\n",data); 
        free (data); 
        temp = temp ->next ; 
    } while (temp); 
}

void delete_cell_key(CellKey* c) {
    /* supprime une cellule de liste chaînée de clés */
    free (c->data);
    c->next = NULL;
    free (c);  
}

void delete_list_keys (CellKey* c) {
    /* supprime une liste chaînée de clés */
    CellKey *temp = NULL; 
    while (c) {
        temp = c ->next ;
        delete_cell_key(c);
        c = temp;
    }
    free (c); 
}

CellProtected* create_cell_protected(Protected* pr) {
    /* alloue et initialise une cellule de liste chaînée */
    CellProtected *cProtec = (CellProtected *) malloc (sizeof(CellProtected)); 
    if (cProtec == NULL) {
        printf ("Erreur d'allocation de CellProtected \n");
        return NULL; 
    }
    cProtec ->data = pr; 
    cProtec ->next = NULL; 
    return cProtec; 
}

void push_lst_cellProtected (CellProtected **lst_protec, Protected* pr) {
    /* ajoute une déclaration signée en tête de liste */
    CellProtected *cProtec = create_cell_protected (pr); 
    cProtec->next = (*lst_protec);
    (*lst_protec) = cProtec;  
}

CellProtected* read_protected () {
    /* lit le fichier declarations.txt et crée une liste contenant toutes les déclarations signées du fichier */
    FILE *fichier = fopen("declarations.txt","w"); 
    if (fichier == NULL)  {
        printf("Erreur a l’ouverture de source.txt\n");
        return NULL; 
    }
    char buffer[256]; 
    char *line;
    char declarPr[256]; 
    CellProtected *res = NULL; // liste résultat
    
    do {
        line = fgets (buffer,256,fichier);
        sscanf (buffer,"%s",declarPr); 
        push_lst_cellProtected(&res, str_to_protected(declarPr)); 

    }while (line); 
    
    fclose(fichier); 
    return res; 
}

void print_list_protec (CellProtected* LCP) {
    /* affiche la liste de Protected */
    if (LCP == NULL) {
        printf ("La liste est vide\n"); 
    }
    CellProtected *temp = LCP; 
    
    char *data; 
    do {
        data = protected_to_str (temp ->data);
        printf ("%s\n",data);
        free(data); 
        temp = temp ->next ;
    } while (temp); 
}

void delete_cell_protected(CellProtected* c) {
    /* supprime une cellule de liste chaînée de déclarations signées */
    // supprimer data de type protected
    free(c->data->message);
    free(c->data->pKey); 
    free(c->data->signature->tab);
    free(c->data->signature); 
    free(c->data);
    // couper l'enchainement et supprimer CellProtected
    c->next = NULL; 
    free (c); 
}

void delete_list_protected (CellProtected *c) {
    /* supprime entièrement une liste chaînée c */
    CellProtected *temp = NULL; 
    while (c) {
        temp = c->next;
        delete_cell_protected(c); 
        c = temp; 
    }
    free(c); 
} 