#include "../antiviRus/antiviRus.h"
#include "../database/database.h"

#define SCAN_CLEAN          0
#define SCAN_INFECTED       1
#define SCAN_SUSPICIOUS     2
#define SCAN_ERR_ACCESS     3
#define SCAN_ERR_LOCKED     4
#define SCAN_SKIPPED        5

int32 scanfile(Entry *);

int8 * calculatesha256();