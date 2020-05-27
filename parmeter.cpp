#include <signal.h>
#include <malloc.h>
#include <libgen.h>
#include "parameter.h"
long f_GetParamIni(const char *sz_FichierInit, const char *sz_VariableName, char *sz_Buffer, long b_Instrumentation)
{
        FILE * p_File = NULL;
        long b_Trouve = 0;
        char sz_Donnees[500] = "";
        char *pc = NULL;
        char *pc_Out = sz_Buffer;

        sz_Buffer[0]=0;

        p_File = fopen( sz_FichierInit, "r" );

        if(p_File == NULL)
        {
                return 1;
        }

        memset(sz_Donnees,0,sizeof(sz_Donnees));

       while ( fgets(sz_Donnees, sizeof(sz_Donnees)-1, p_File) != NULL )
        {

                if(memcmp(sz_Donnees,sz_VariableName,strlen(sz_VariableName))==0)
                {
                        pc = sz_Donnees + strlen(sz_VariableName);
                       // ---------------------------------------------------
                        // skip white spaces
                        // ---------------------------------------------------

                        while(isspace(*pc))
                        {
                                pc++;
                        }
                        // -----------------------------------------
                        // if this equal or new line 
                        // -----------------------------------------
                        if((*pc) == '=')
                        {
                                char sz_NameVarEnv[MAX_VAR_ENV] = "";
                                char *pCur_NameVarEnv = NULL;
                                char *psz_ValueVarEnv = NULL;
                                b_Trouve = 1;

                                // ---------------------------------------------------
                                // remove white space afer = sign
                                // ---------------------------------------------------
                                pc++;
                                while(isspace(*pc))
                                {
                                        pc++;
                                }
                                // -------------------------------------------------------------
                                // we copy to return varaible
                                //-----------------------------------------
                                                                while(*pc)
                                {
                                        if(*pc=='$')
                                        {
                                                pc++;

                                                pCur_NameVarEnv=sz_NameVarEnv;

                                                while(isalnum(*pc) || (*pc)=='_')
                                                {
                                                        *(pCur_NameVarEnv++)=*(pc++);
                                                }

                                                *(pCur_NameVarEnv++)=0;

                                                // ----------------------------------------
                                                // on recherche la variable d'environnement
                                                // ----------------------------------------

                                                psz_ValueVarEnv = getenv(sz_NameVarEnv);

                                                if(psz_ValueVarEnv)
                                                {
                                                        strcpy(pc_Out, psz_ValueVarEnv);
                                                        pc_Out+=strlen(psz_ValueVarEnv);
                                                }
                                        }
                                        else
                                        {
                                                *(pc_Out++)=*(pc++);
                                        }
                                }
                                *(pc_Out++)=0;
                        }
                }
                 }

        fclose(p_File);

        if(b_Trouve != 0)
        {
                f_KillSpacesAtEnd(sz_Buffer);
                return(0);
        }

        // ------------------------------------------------------------
        // key value not found  1
        // ------------------------------------------------------------
        return(1);
}


void f_KillSpacesAtEnd( char * psz_Buffer )
{
        int i = 0;

        for ( i = strlen(psz_Buffer) - 1; (i >= 0) && (isspace(psz_Buffer[i])); i--) psz_Buffer[i] = '\0';
}
  
                                
/*int main(int argc, char *argv[]) {

char lsz_FichierInit[MAX_PATH] = "";
memset(lsz_FichierInit, 0, sizeof(lsz_FichierInit));
 if (argc == 2) {
                strncpy(lsz_FichierInit, argv[1], sizeof(lsz_FichierInit) - 1);
        }
        else
        {
                // ------------------------------------------------------
                // OU bien recuperation des parametres du fichier de parametrage
                // ------------------------------------------------------
                const char *bibref = getenv(TOTO);
                if(bibref != NULL) {
                        snprintf(lsz_FichierInit, sizeof(lsz_FichierInit), "%s%s", bibref, FILE_INIT);
                }
        }

                char            sz_HostName             [MAX_COMPUTERNAME_LENGTH+1];
                 f_GetParamIni(lsz_FichierInit,"TITI" ,sz_HostName,1);
     printf("sz_HostName  TITI  %s\n",sz_HostName );


return 0;
}
*/
parameter.h file
/* Includes standards C */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/errno.h>
#define MAX_PATH                        256
#define MAX_COMPUTERNAME_LENGTH         15
#define FILE_INIT "/toto.ini"
#define BIBREF    "totref"
#define MAX_VAR_ENV             4096
long f_GetParamIni(const char *sz_FichierInit, const char *sz_VariableName, char *sz_Buffer, long b_Instrumentation);
void f_KillSpacesAtEnd( char * psz_Buffer );


