#include "scanfile.h"
#include "../antiviRus/antiviRus.h"
#include "../database/database.h"

int32 scanfile(Entry * e) {
    e->filehandle = CreateFileW(
        e->path, 
        GENERIC_READ, 
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL
    );

    if (e->filehandle == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        if (err == ERROR_ACCESS_DENIED) return 3;
        if (err == ERROR_SHARING_VIOLATION) return 4;
        return 3;
    }

    CloseHandle(e->filehandle);
    return 0;
}