#include "antiviRus.h"
#include "../database/database.h"
#include "../workqueue/workqueue.h"

int main(int argc, char *argv[]){
    Database *db;
    db = mkdatabase();  
    adddir(db, $1 "C:\\Windows\\System32");
    showdb(db);
    destroydb(db);

    return 0;
}