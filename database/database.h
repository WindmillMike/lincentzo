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

    int8 *pool;         // Rezervorul de memorie (String Pool) pentru nume
    int32 poolcap;    // Capacitatea pool-ului
    int32 poolused;   // Cat din pool am folosit deja

    int32 cap;          // Capacitatea curenta (numar de entries)
    int32 num;          // Numarul actual de fisiere salvate
    int32 hashsize;
};

typedef struct s_database Database;

int32 hash(Database *, int8 *, int8 *);

int32 hashpath(Database *, int8 *);

Database *mkdatabase();

bool adddir(Database *, int8 *);

void addtodb(Database *, int8 *, int8 *);

int32 addtopool(Database *, int8 *);

void hashresize(Database *);

void findbypathdb(Database *, int8 *);

void popfromdb(Database *, int8 *);

void destroydb(Database *);

void showdb(Database *);

typedef struct s_database Database;
