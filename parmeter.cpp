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


Dbacc.h
/************************************************************************
Projet       : Dbacc
Fichier      : Dbacc.h
Description  : Header file description for OCCI interface to ORACLE

Auteur       : KRA
Date creation: 14/11/2011
Histo modifs :
Remarque :
******************************************************************************/

#ifndef __DBACC_H__
#define __DBACC_H__


#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;



/****************************************************************/
/** Class Dbacc : service class for a connection               **/
/** connection to OCCI interface                               **/
/**                                                            **/
/****************************************************************/
/****************************************************************/
/** Class Dbacc : service class for a connection               **/
/** connection to OCCI interface                               **/
/**                                                            **/
/****************************************************************/

class Dbacc
 {

    private:
        oracle::occi::Environment *env;
        oracle::occi::StatelessConnectionPool *scPool;

        public:

     // oracle::occi::Environment* environment;  //environement handle
      oracle::occi::Connection *con;           //connection handle
      oracle::occi::Statement* stmt;           //oracle statement


    public :
           void ShowDate(string ddt);
       Dbacc(string user, string passwd, string db);
           //void ExecuteQry(string sql );
            oracle::occi::Connection*  GetConnection();
            void ReleaseConnection(oracle::occi::Connection* cnx);
            //oracle::occi::Environment* GetEnvironment ();
      ~Dbacc();

 };


#endif // __DBACC_H__



Dbacc.cpp
#include "Dbacc.h"
//#include <iostream>
//#include <occi.h>
//using namespace oracle::occi;
//using namespace std;
//#define VERBOSE;
/*************************************************************/
/** Classe Dbacc : destructor                               **/
/**                                                         **/
/** In  :                                                   **/
/** Out :                                                   **/
/**                                                         **/
/*************************************************************/
//oracle::occi::Environment* Dbacc::con;

const string getNextContratTag = "getNextContratTag";

// TAF callback function
int taf_callback(Environment *env, Connection *conn, void *ctx,  \
Connection::FailOverType foType, Connection::FailOverEventType foEvent)
{

        #ifdef VERBOSE
        cout << "TAF [Callback] Called " << endl;
        #endif

        cout << "Failover Event : " ;
        switch(foEvent)
        {
                case Connection::FO_END   : cout << "FO_END" << endl;
                                         break;
                case Connection::FO_ABORT : cout << "FO_ABORT" << endl;
                                         break;
                case Connection::FO_REAUTH: cout << "FO_REAUTH" << endl;
                                         break;
                case Connection::FO_BEGIN : cout << "FO_BEGIN" << endl;
                                         break;
                case Connection::FO_ERROR : cout << "FO_ERROR" << endl;
                                         break;
                default                   : cout << "Default " << endl;
                                         break;
        }
        cout << "Failover Type : " ;
        switch(foType)
        {
                case Connection::FO_NONE   : cout << "FO_NONE" << endl;
                                          break;
                case Connection::FO_SESSION: cout << "FO_SESSION" << endl;
                                          break;
                case Connection::FO_SELECT : cout << "FO_SELECT" << endl;
                                          break;
                default                    : cout << "Default " << endl;
                                          break;
        }
        if (foEvent == Connection::FO_ERROR)
                return FO_RETRY;
        if (foEvent == Connection::FO_END)
        {
                cout << "TAF-FO: Failover complete" << endl;
                return 0;
        }
        }
        return 0;
}
Dbacc ::~Dbacc ()
{
        try {
              #ifdef VERBOSE
              cout << "Terminating connection pool" << endl;
              #endif
                // Destroy the connection pool
                if (scPool) env->terminateStatelessConnectionPool (scPool);
        }
        catch(exception& ex) {}
        try {
              #ifdef VERBOSE
               cout << "Terminating environment " << endl;
              #endif
                Environment::terminateEnvironment (env);
        }
        catch(exception& ex) {}
    //environment->terminateConnection (con);
    //oracle::occi::Environment::terminateEnvironment (environment);
}

Dbacc :: Dbacc(string user, string passwd, string connectStr)
{

        unsigned int maxConn = 10;
        unsigned int minConn = 1;
        unsigned int incrConn = 1;
        unsigned int stmtCacheSize = 3;
        try
        {
                // env = Environment::createEnvironment (Environment::DEFAULT);
                // or, to ensure a multi-threaded safe Environment (env does mutexing):
                 env = Environment::createEnvironment (Environment::THREADED_MUTEXED);
                 //env = Environment::createEnvironment (Environment::OBJECT);

                try
                {
                        // Create a homogenous connection pool
                        scPool = env->createStatelessConnectionPool
                                          (user, passwd, connectStr,
                                          maxConn, minConn, incrConn,
                                          StatelessConnectionPool::HOMOGENEOUS);
                }
                catch(SQLException& ex)
                {
                        cout<<"ERROR: SQL Exception thrown."<<endl;
                        cout<<"Error number: "<<  ex.getErrorCode()<<endl;
                        cout<<ex.getMessage() << endl;

                        // this shouldn't really be necessary, but if connection pool
                        // creation has failed due to db unavailable, scPool is
                        // still defined, yet any attempt to call a method on
                        // scPool generates a segmentation fault.
                        // By nullifying the scPool handle we avoid that problem
                        scPool = 0;
                }
                if (scPool) {
                        #ifdef VERBOSE
                        cout<<"SUCCESS - createStatelessConnectionPool"<<endl;
                        // Show the connection pool parameters
                        cout<<"BusyConn="<<scPool->getBusyConnections()<<endl;
                        cout<<"OpenConn="<<scPool->getOpenConnections()<<endl;
                        cout<<"MaxConn="<<scPool->getMaxConnections()<<endl;
                        cout<<"MinConn="<<scPool->getMinConnections()<<endl;
                        cout<<"IncrConn="<<scPool->getIncrConnections()<<endl;
                        #endif

                        //Enabling the cache on the pool
                        scPool->setStmtCacheSize(stmtCacheSize);
                        #ifdef VERBOSE
                        unsigned size = scPool->getStmtCacheSize();
                        cout<<"The size of cache for pool is "<< size<<endl;
                        #endif

                } else {
                        throw runtime_error("connection pool not available");
                }

        }
        catch(SQLException& ex)
        {
                cout<<"ERROR: SQL Exception thrown."<<endl;
                cout<<"Error number: "<<  ex.getErrorCode()<<endl;
                cout<<ex.getMessage() << endl;
        }
        catch(exception& ex)
        {
                cout<<"ERROR: General exception thrown."<<endl;
                cout<<ex.what() << endl;
        }




  /*try{
         environment = oracle::occi::Environment::createEnvironment(oracle::occi::Environment::DEFAULT);
         con = environment->createConnection(user, passwd,db);

     }catch(oracle::occi::SQLException &e){
         std::cout<<e.what();
     }*/

}
void Dbacc :: ShowDate(string ddt)
{

Date to_day = oracle::occi::Date::getSystemDate(env);
string tsstr=to_day.toText("dd/mm/yyyy hh:mi:ss [tzh:tzm]",0);
cout<<"tsstr date format " << tsstr<<endl;
}


//oracle::occi::Environment* Dbacc ::  GetEnvironment ()
//{
//return Dbacc::env;
//}
void Dbacc :: ReleaseConnection(oracle::occi::Connection* cnx)
{

scPool->releaseConnection(cnx);

}
oracle::occi::Connection* Dbacc ::  GetConnection  ()
{
Connection *conn ;
        try
        {
                if (!scPool) throw runtime_error("connection pool not available");
                conn = scPool->getConnection();
                if (!conn) throw runtime_error("connection not available");

                //Registering the Connection for TAF Callback.
                #ifdef VERBOSE
                cout << "Registering the Connection with the TAF Callback" << endl;
                #endif
                conn->setTAFNotify(taf_callback,NULL);
         }
          catch(SQLException& ex)
                {
                        cout<<"ERROR: SQL Exception thrown "<<endl;
                        cout<<"Error number: "<<  ex.getErrorCode()<<endl;
                        cout<<ex.getMessage() << endl;
                }
        return conn;
}
//void Dbacc :: ExecuteQry (string sql )
// {
// oracle::occi::Statement *stmt = NULL;
// oracle::occi::ResultSet *rs =NULL;
  // try
  // {

    // stmt = con->createStatement(sql);
  // }
  // catch (oracle::occi::SQLException &ex)
  // {
    // cout << ex.getMessage();
  // }
  // if (stmt)
  // {
    // try
    // {
      // //stmt->setPrefetchRowCount(32);

      // rs = stmt->executeQuery();
    // }
    // catch (oracle::occi::SQLException &ex)
    // {
      // cout << ex.getMessage();
    // }
        // if (rs)
        // {
        // while (rs->next()) {
            // cout << rs->getString(1) << endl;


      // }
        // stmt->closeResultSet(rs);
   // }
   // con->terminateStatement(stmt);
   // }
// }
// end of classe

compile command
oraxlc -o toto loger.cpp Dbacc.cpp toto.cpp.cpp parameter.cpp  -qtls=local-exec  -O3 -q32 -DSS_32BIT_SERVER -qwarn32 -qinfo=uni -DAIXRIOS -
qflag=s:s -I/xxxx/ora11204/11.2.0/rdbms/public -L/xxx/ora11204/11.2.0/lib32
 -L/xxx/ora11204/11.2.0/lib32/stubs -lc /xxx/ora11204/11.2.0/lib32/libclntsh.a  -locci -lclntsh -lpthread -lm -lpdf -lld

