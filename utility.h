#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/mode.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <occi.h>
#include <time.h>
#include <ctype.h>
#include <pdflib.h>
#include <libgen.h>
#define NOMLEN 20
#define SIDLEN 20
#define USERLEN 50
#define PASSWDLEN 50
#define ENVTLEN 20
#define SCHEMALEN 20
#define MAX_LEN 255
#define FC_PARAM_LEN 4

typedef struct {
        char nom[NOMLEN];
        char sid[SIDLEN];
        char user[USERLEN];
        char passwd[PASSWDLEN];
        char envt[ENVTLEN];
        char schema[SCHEMALEN];
        char sourceDir[PATH_MAX];
        int  isconnected;
        int  ncontrat;
        char fcpy[FC_PARAM_LEN];
} DatabaseInfo;


typedef struct stPgmInfos {
        char sourceDir[PATH_MAX];
        oracle::occi::Connection * cxn;
        char date_debut[20+1]; /* %Y%m%d_%H%M%S */
} PgmInfos;
typedef struct stThreadInfos  {
        PgmInfos *pgmInfos;
        DatabaseInfo *db;
        int idx_thread;
        pthread_mutex_t lock_resource;
        size_t nTotal;
        int  ncnt;
        int statut;
} ThreadInfos;


int isDirectoryExist(const char * dirpath);
int isFileExist(const char * filepath);
int getConnectionInfoDb(DatabaseInfo *pdb, const char *nom);
char *getVarname(const char *prefix, const char*name);
int copyFile(const char *from, const char *to, pthread_mutex_t *mutexCreateFile);
void  CheckDirectory(const char *fichier);
int getFileExtension(const char * fileName , char * extension , size_t len_extension )

