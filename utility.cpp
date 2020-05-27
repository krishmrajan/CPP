#include "utility.h"
#include "loger.h"

/*------------------------------------*/
/* Function :isDirectoryExist         */
/* parameter : directory Name to check*/
/* return    : 0 exist                */
/*             1 not exist            */
/*-------------------------------------*/
int isDirectoryExist(const char * dirpath)
{
        struct stat64 infopath = {0};

        if(stat64(dirpath , &infopath) != 0 || ((infopath.st_mode & S_IFDIR) != S_IFDIR))
        {
                return 1;
        }
        return 0;
}
/*------------------------------------*/
/* Function :isFileExist              */
/* parameter : fileName with path     */
/* return    : 0 exist                */
/*             1 not exist            */
/*-------------------------------------*/
int isFileExist(const char * filepath)
{

        struct stat64 infopath = {0};
        if(stat64(filepath, &infopath) != 0 || ((infopath.st_mode & S_IFREG) != S_IFREG))
        {
                return 1;
        }
        return 0;
}

int getConnectionInfoDb(DatabaseInfo *pdb, const char *nom)
{
        const char *varprefix = NULL;
        const char *varenv = NULL;
        const char *defaultprefix = "ORA_";
        if(nom == NULL) {
                nom = "XXX";
        }

        if(pdb == NULL) {
                LOG(LOGERR, "pdb est NULL");
                return 1;
        }
        memset(pdb, 0, sizeof(DatabaseInfo));

        /* connexion via les variables d'environnement */
        strncpy(pdb->nom, nom, sizeof(pdb->nom) - 1);
        varprefix = getenv("PREFIX_BDD");
        if(varprefix == NULL || strcmp(varprefix, "") == 0) {
                varprefix = defaultprefix;
        }
        varenv = getenv(getVarname(varprefix, "SID"));
        if(varenv == NULL) {
                LOG(LOGERR, "la variable ORA_SID n'est pas défini");
strncpy(pdb->sid, varenv, sizeof(pdb->sid) - 1);

        varenv = getenv(getVarname(varprefix, "AGW_USER"));
        if(varenv == NULL) {
                LOG(LOGERR, "la variable ORA_AGW_USER n'est pas défini");
                return 1;
        }
        strncpy(pdb->user, varenv, sizeof(pdb->user) - 1);

        varenv = getenv(getVarname(varprefix, "PASSWD"));
        if(varenv == NULL) {
                LOG(LOGERR, "la variable ORA_PASSWD n'est pas défini");
                return 1;
        }
        strncpy(pdb->passwd, varenv, sizeof(pdb->passwd) - 1);

        varenv = getenv(getVarname(varprefix, "SCH"));
        if(varenv == NULL) {
                LOG(LOGERR, "la variable ORA_SCH n'est pas défini");
                return 1;
        }
        strncpy(pdb->schema, varenv, sizeof(pdb->schema) - 1);
        varenv = getenv("ENVT");
        if(varenv == NULL) {
                LOG(LOGERR, "la variable ENVT n'est pas défini");
                return 1;
        }
        strncpy(pdb->envt, varenv, sizeof(pdb->envt) - 1);

        return 0; //Connect_Oracle(pdb);
}
