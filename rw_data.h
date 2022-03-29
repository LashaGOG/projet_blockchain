#ifndef READ_WRITE_DATA
#define READ_WRITE_DATA

#include <stdlib.h>
#include <string.h>
#include "keys.h" 


typedef struct cellKey {
    Key *data;  
    struct cellKey* next; 
} CellKey ; 

CellKey* create_cell_key(Key* key); 
void push_lst_key (CellKey **lst_Key, Key* key);
CellKey* read_public_keys (char *file_Name);


#endif //READ_WRITE_DATA

