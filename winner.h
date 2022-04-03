#ifndef COMPUTE_WINNER
#define COMPUTE_WINNER

#include <stdlib.h>
#include <math.h>
#include "rw_data.h"

typedef struct hashcell {
    Key *key ; 
    int val; 
} HashCell ; 

typedef struct hashtable {
    HashCell **tab ; 
    int size; 
} HashTable ; 

void remove_fraud(CellProtected **LCP); 

HashCell* create_hashcell(Key* key); 
int hash_function(Key* key, int size); 
int find_position(HashTable* t, Key* key);
HashTable* create_hashtable(CellKey* keys, int size); 
void delete_hashcell (HashCell* hc);
void delete_hashtable(HashTable* t); 

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) ;

#endif //COMPUTE_WINNER