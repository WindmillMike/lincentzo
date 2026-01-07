#include "../antiviRus/antiviRus.h"

typedef int8 Path[MAX_PATH];   
typedef int8 File[MAX_FILE];

struct s_entry {
    Path path;
    File file;
};

typedef struct s_entry Entry;

struct s_database {
    Entry *entries;
    int32 cap;
    int32 num;
};

typedef struct s_database Database;

int32 hash(Database *, int8 *);

Database *mkdatabase();

bool adddir(Database *, int8 *);

void addtodb(Database *, Entry *);

void popfromdb(Database *, Entry *);

void destroydb(Database *);

void showdb(Database *);

typedef struct s_database Database;
