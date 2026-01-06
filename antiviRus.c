#include "antiviRus.h"

Database *mkdatabase() {
    Database *db;    
    Entry *p;
    int32 size;

    size = sizeof(Database);
    db = (Database *)malloc($i size);   //allocates RAM for db
    assert(db);   
    memset($1 db, 0, size);  
    db->num = 0;
    db->cap = Blocksize;
    size = Blocksize * sizeof(Entry);
    p = (Entry *)malloc($i size);   //allocates RAM for entries
    assert(p);   //Checks if he allocated the full size, if not throws an error
    memset($1 p, 0, size);   //Clears the zone that it recieved by putting 0's and recycles pointer p
    db->entries = p;

    return db;
}

void showdb(Database *db){
    int32 n;
    printf("cap:\t%d\nnum:\t%d\n", db->cap, db->num);

    for (n = 0; n < db->num; n++)
        printf("%s/%s\n", db->entries[n].dir, db->entries[n].file);

    return;
}

void destroydb(Database *db) {
    db->cap = 0;
    db->num = 0;
    free(db->entries);
    free(db);

    return;
}

void addtodb(Database *db, Entry e) {
    int32 cap, size, i;

    if(db->cap == db->num) {
        cap = db->cap + Blocksize;
        size = cap * sizeof(Entry);
        db->entries = realloc(db->entries, size);
        assert(db->entries);
        memset((db->entries + db->cap), 0, (cap - db->cap) * sizeof(Entry));   //Clears the new reallocated zone
        db->cap = cap;
    } 

    i = db->num;
    memcpy(&db->entries[i], &e, sizeof(Entry));
    db->num++;

    return;
}

bool adddir(Database *db, int8 *path) {
    int32 i;

    for(i = 0; i < db->num; i++) {
        if(strcmp($c db->entries[i].dir, $c path) == 0)
            return false;
    }

    return true;
}

int main(int argc, char *argv[]){
    Database *db;
    Entry e1, e2;
    db = mkdatabase();  
    showdb(db);
    destroydb(db);
    return 0;
}
