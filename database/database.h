#include "../antiviRus/antiviRus.h"


struct s_entry {
    int32 fileoffset;
    int32 diroffset;
};

typedef struct s_entry Entry;

struct s_hashnode {
    int32 next;
};

typedef struct s_hashnode HashNode;

struct s_database {
    Entry *entries;
    HashNode *nodes;
    int32 *hashindexes;

    int32 *folderhashes;   // Aici avem offseturile folderelor in functie de hashuri
    int32 foldercap;   // Capacitatea folderhashes
    int32 foldernum;  // Numarul de foldere unice

    int8 *pool;         // Rezervorul de memorie (String Pool) pentru nume
    int32 poolcap;    // Capacitatea pool-ului
    int32 poolused;   // Cat din pool am folosit deja

    int32 cap;          // Capacitatea curenta (numar de entries)
    int32 num;          // Numarul actual de fisiere salvate
    int32 hashsize;
};

typedef struct s_database Database;

int32 hash(Database *, int8 *, int8 *, int8);

int32 hashpath(Database *, int8 *, int8);

Database *mkdatabase();

bool adddir(Database *, int8 *);

void addtodb(Database *, int8 *, int8 *);

int32 adddirpool(Database *, int8 *);

int32 addfilepool(Database *, int8 *);

void dirhashresize(Database *);

void hashresize(Database *);

void findbypathdb(Database *, int8 *);

void popfromdb(Database *, int8 *);

void destroydb(Database *);

void showdb(Database *);

typedef struct s_database Database;
