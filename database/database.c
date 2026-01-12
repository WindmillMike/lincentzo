#include "database.h"
#include "../antiviRus/antiviRus.h"

int32 hash(Database *db, int8 *path) {
    int32 h = 5381, c;

    while ((c = *path++)) {
        h = ((h << 5) + h) + c;
    }
    return h % db->cap;
}

Database *mkdatabase() {
    Database *db;    
    Entry *p;

    db = (Database *)malloc($i sizeof(Database));   //allocates RAM for db
    assert(db);   
    memset($1 db, 0, sizeof(Database));  
    db->num = 0;
    db->cap = Blocksize;
    p = (Entry *)malloc($i (Blocksize * sizeof(Entry)));   //allocates RAM for entries
    assert(p);   //Checks if he allocated the full size, if not throws an error
    memset($1 p, 0, (Blocksize * sizeof(Entry)));   //Clears the zone that it recieved by putting 0's and recycles pointer p
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

void addtodb(Database *db, Entry *e) {
    int32 cap, size, i;

    if(db->cap == db->num) {
        cap = db->cap + Blocksize;
        size = cap * sizeof(Entry) * 2;
        db->entries = realloc(db->entries, size);
        assert(db->entries);
        memset((db->entries + db->cap), 0, (cap - db->cap) * sizeof(Entry));   //Clears the new reallocated zone
        db->cap = cap;
    } 

    i = db->num;
    memcpy(&db->entries[i], e, sizeof(Entry));
    db->num++;

    return;
}

void popfromdb(Database *db, Entry *e) {
    int32 i;

    if(db->num == 0)
        return;

    db->num--;

    memset($1 &db->entries[db->num], 0, sizeof(Entry));

    return;
}

// Nu e inca ok

bool adddir(Database *db, int8 *path) {
    Entry e;
    HANDLE handle = CreateFileA(
        path,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS, 
        NULL
    );

    if (handle == INVALID_HANDLE_VALUE) return false;

    BYTE buffer[102400] __attribute__((aligned(8)));

    if (!GetFileInformationByHandleEx(handle, FileIdBothDirectoryInfo, buffer, sizeof(buffer))) {
        CloseHandle(handle);
        return false;
    }

    FILE_ID_BOTH_DIR_INFO *info = (FILE_ID_BOTH_DIR_INFO *)buffer;
    while (1) {
        memset($c &e, 0, sizeof(Entry));  
        strncpy($c e.dir, $c path, MAX_PATH - 1);


        int32 byteswritten = WideCharToMultiByte(
            CP_UTF8,             
            0, 
            info->FileName,     
            info->FileNameLength / sizeof(WCHAR),          
            e.file,             
            MAX_FILE - 1,       
            NULL, NULL
        );
        

        e.file[byteswritten] = '\0';
        addtodb(db, &e);
        
        if (info->NextEntryOffset == 0) break;
        info = (FILE_ID_BOTH_DIR_INFO *)((BYTE *)info + info->NextEntryOffset);
    }

    CloseHandle(handle);
    return true;
}
