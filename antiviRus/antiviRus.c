#include "antiviRus.h"
#include "../database/database.h"
#include "../workqueue/workqueue.h"

int main(int argc, char *argv[]){
    Database *db;
    Entry e1, e2;
    strcpy($c e1.path, "C:/Users/Sn0ke/Desktop");
    strcpy($c e1.file, "file1.txt");    
    strcpy($c e2.path, "C:/Users/Sn0ke/Documents");
    strcpy($c e2.file, "file2.txt");
    db = mkdatabase();  
    addtodb(db, &e1);
    addtodb(db, &e2);
    showdb(db);
    printf("%d\n\n", hash(db, "C"));
    printf("%d\n\n", hash(db, "D"));
    printf("%d\n\n", hash(db, "C:/Users"));
    destroydb(db);
    return 0;
}