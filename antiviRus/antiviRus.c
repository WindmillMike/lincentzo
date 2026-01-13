#include "antiviRus.h"
#include "../database/database.h"
#include "../workqueue/workqueue.h"

int main(int argc, char *argv[]) {
    // 1. Inițializare cu o capacitate mică pentru a forța resize-ul rapid
    // Pornim cu doar 10 locuri și 20 de sertare hash
    Database *db = mkdatabase(); 
    // Dacă mkdatabase are valori fixe mari, le poți lăsa așa, 
    // dar testul de mai jos va funcționa oricum.

    printf("Incepem testarea...\n");

    // 2. Test Inserare Masivă (Forțăm Resize și Rehash)
    // Inserăm 1000 de fișiere simulate
    for (int i = 0; i < 100000; i++) {
        char folder[64];
        char fisier[64];
        sprintf(folder, "C:\\Windows\\System%d", i / 10); // Multe fisiere in acelasi folder
        sprintf(fisier, "file_%d.exe", i);
        
        addtodb(db, (int8 *)folder, (int8 *)fisier);
    }

    printf("Inserare completa. Total fisiere: %d\n", db->num);
    printf("Capacitate curenta: %d, Hash Size: %d\n", db->cap, db->hashsize);

    findbypathdb(db, (int8 *)"C:\\Windows\\System2\\file_7255.exe");
    // 5. Curățenie
    destroydb(db);
    printf("\nTest finalizat cu succes!\n");

    return 0;
}