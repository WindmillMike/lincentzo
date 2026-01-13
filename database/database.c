#include "database.h"
#include "../antiviRus/antiviRus.h"

int32 hash(Database *db, int8 *dir, int8 *file) {
    int32 h = 5381, c;

    while ((c = *dir++)) {
        h = ((h << 5) + h) + c;
    }

    h = ((h << 5) + h) + '\\';

    while ((c = *file++)) {
        h = ((h << 5) + h) + c;
    }
    
    return h % db->hashsize;
}

int32 hashpath(Database *db, int8 *path) {
    int32 h = 5381, c;

    while ((c = *path++)) {
        h = ((h << 5) + h) + c;
    }

    return h % db->hashsize;
}

Database *mkdatabase() {
    Database *db = (Database *)malloc(sizeof(Database));
    assert(db);   
    memset($1 db, 0, sizeof(Database));  

    db->cap = Blocksize;              // Număr maxim de fișiere inițial
    db->hashsize = Hashsize;       // Dimensiune tabelă hash (număr prim)
    db->poolcap = 1 * 1000 * 1000;   
    db->num = 0;
    db->poolused = 0;

    db->entries = (Entry *)malloc(db->cap * sizeof(Entry));
    assert(db->entries);   
    memset($1 db->entries, 0, sizeof(Entry));  
    db->nodes = (HashNode *)malloc(db->cap * sizeof(HashNode));
    assert(db->nodes);   
    memset($1 db->nodes, 0, sizeof(HashNode));  
    
    db->hashindexes = $4 malloc(db->hashsize * sizeof(int32));
    assert(db->hashindexes);   
    memset($1 db->hashindexes, 0, sizeof(int32)); 

    db->pool = $1 malloc(db->poolcap);
    assert(db->pool);   
    memset($1 db->pool, 0, sizeof(int8)); 

    if (!db->entries || !db->nodes || !db->hashindexes || !db->pool) {
        destroydb(db);
        return NULL;
    }

    for (int32 i = 0; i < db->hashsize; i++) {
        db->hashindexes[i] = -1;
    }

    return db;
}

void destroydb(Database *db) {
    if (!db) return;

    if (db->entries) 
        free(db->entries);
    
    if (db->nodes) 
        free(db->nodes);
    
    if (db->hashindexes) 
        free(db->hashindexes);

    if (db->pool)
        free(db->pool);

    free(db);
    
}

void showdb(Database *db) {
    if (!db || db->num == 0) {
        printf("Empty.\n");
        return;
    }

    printf("--- (%d Files) ---\n", db->num);
    
    for (int32 i = 0; i < db->num; i++) {
        Entry *e = &db->entries[i];

        int8 *dir_path = $1 (db->pool + e->diroffset);
        int8 *file_name = $1 (db->pool + e->fileoffset);

        printf("[%d] %s\\%s (%llu bytes)\n", i, dir_path, file_name);
    }
    
    printf("--- End ---\n");
}

void addtodb(Database *db, int8 *dir, int8 *file) {
    if (db->num >= db->cap) {
        db->cap *= 2; 
        
        db->entries = (Entry *)realloc(db->entries, db->cap * sizeof(Entry));
        assert(db->entries);   
        memset($1 (db->entries + db->num), 0, (db->cap - db->num) * sizeof(Entry));  
        db->nodes = (HashNode *)realloc(db->nodes, db->cap * sizeof(HashNode));
        assert(db->nodes);   
        memset($1 (db->nodes + db->num), 0, (db->cap - db->num) * sizeof(HashNode));  
    }

    if (db->num >= db->hashsize * 0.75) 
        hashresize(db);

    int32 h = hash(db, dir, file);

    int32 i = db->num;
    db->entries[i].diroffset = addtopool(db, dir);    //Trebuie sa implementez o modalitate de a nu duplica folderele printr-un hash table
    db->entries[i].fileoffset = addtopool(db, file);

    db->nodes[i].next = db->hashindexes[h];
    db->hashindexes[h] = i;
    db->num++;
}

int32 addtopool(Database *db, int8 *str) {
    int32 len = strlen($c str) + 1;

    while (db->poolused + len > db->poolcap) {
        db->poolcap *= 2;
        db->pool = $1 realloc(db->pool, db->poolcap);
        assert(db->pool);
        memset($1 (db->pool + db->poolused), 0, (db->poolcap - db->poolused) * sizeof(int8));
    }

    int32 offset = db->poolused;
    memcpy(db->pool + offset, str, len);
    db->poolused += len;

    return offset;
}

void hashresize(Database *db) {
    int32 old_hashsize = db->hashsize;
    db->hashsize *= 2;
    int32 *newindexes = $4 malloc(db->hashsize * sizeof(int32));
    assert(newindexes);
    memset($1 newindexes, 0, sizeof(int32));

    for (int32 i = 0; i < db->hashsize; i++) {
        newindexes[i] = -1;
    }

    for (int32 i = 0; i < db->num; i++) {
        int8 *dir = $1 (db->pool + db->entries[i].diroffset);
        int8 *file = $1 (db->pool + db->entries[i].fileoffset);
        int32 h = hash(db, dir, file);
        db->nodes[i].next = newindexes[h];
        newindexes[h] = i;
    }

    free(db->hashindexes);
    db->hashindexes = newindexes;
}

void findbypathdb(Database *db, int8 *path) {
    int32 h = hashpath(db, path);
    int32 i = db->hashindexes[h];
    while (i != -1) {
        Entry *e = &db->entries[i];
        int8 *dir_path = $1 (db->pool + e->diroffset);
        int8 *file_name = $1 (db->pool + e->fileoffset);

        int32 needed_size = strlen($1 dir_path) + strlen($1 file_name) + 2;
        int8 *full_path = $1 malloc(needed_size);
        assert(full_path);
        memset($1 full_path, 0, needed_size * sizeof(int8));
        snprintf($1 full_path, needed_size, "%s\\%s", $1 dir_path, $1 file_name);

        if (strcmp($1 full_path, $1 path) == 0) {
            printf("Found at index %d: %s\\%s\n", i, $1 dir_path, $1 file_name);
            return;
        }
        free(full_path);
        i = db->nodes[i].next;
    }

    printf("Not Found: %s\n", $c path);
    return;
}

void popfromdb(Database *db, int8 *path) {
    int32 h = hashpath(db, path);
    int32 i = db->hashindexes[h];

    while (i != -1) {
        Entry *e = &db->entries[i];
        int8 *dir_path = $1 (db->pool + e->diroffset);
        int8 *file_name = $1 (db->pool + e->fileoffset);

        int32 needed_size = strlen($1 dir_path) + strlen($1 file_name) + 2;
        int8 *full_path = $1 malloc(needed_size);
        assert(full_path);
        memset($1 full_path, 0, needed_size * sizeof(int8));
        snprintf($1 full_path, needed_size, "%s\\%s", $1 dir_path, $1 file_name);
        if (strcmp($1 full_path, $1 path) == 0) {
            
            return;
        }


}

// Nu e inca ok

// bool adddir(Database *db, int8 *path) {
//     Entry e;
//     HANDLE handle = CreateFileA(
//         path,
//         FILE_LIST_DIRECTORY,
//         FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
//         NULL,
//         OPEN_EXISTING,
//         FILE_FLAG_BACKUP_SEMANTICS, 
//         NULL
//     );

//     if (handle == INVALID_HANDLE_VALUE) return false;

//     BYTE buffer[102400] __attribute__((aligned(8)));

//     if (!GetFileInformationByHandleEx(handle, FileIdBothDirectoryInfo, buffer, sizeof(buffer))) {
//         CloseHandle(handle);
//         return false;
//     }

//     FILE_ID_BOTH_DIR_INFO *info = (FILE_ID_BOTH_DIR_INFO *)buffer;
//     while (1) {
//         memset($c &e, 0, sizeof(Entry));  
//         strncpy($c e.dir, $c path, MAX_PATH - 1);


//         int32 byteswritten = WideCharToMultiByte(
//             CP_UTF8,             
//             0, 
//             info->FileName,     
//             info->FileNameLength / sizeof(WCHAR),          
//             e.file,             
//             MAX_FILE - 1,       
//             NULL, NULL
//         );
        

//         e.file[byteswritten] = '\0';
//         addtodb(db, &e);
        
//         if (info->NextEntryOffset == 0) break;
//         info = (FILE_ID_BOTH_DIR_INFO *)((BYTE *)info + info->NextEntryOffset);
//     }

//     CloseHandle(handle);
//     return true;
// }
