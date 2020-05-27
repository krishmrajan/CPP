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
Laurence.desnoyers@gmail.com

La méthode utilisée pour effectuer une migration des bases Oracle en version 19C consiste à :
•	Installation du moteur Oracle 19C
•	Création de la base cible en 19C (CDB uniquement)
•	Récupérer les DDL (users, tbs, profiles …) de la base source
•	Sauvegarde RMAN de la base source (uniquement si même stockage)
•	Export datapump de la base source
•	Suppression de la base source (uniquement si même stockage)
•	Import datapump de la base source sur la base cible
•	Modification des liens réseaux et profiles
•	Application des spécificités des environnements 
Ces grandes lignes comportent une multitude d’actions et chaque action diffères selon les bases. De ce fait, il a été décidé de construire un fichier Excel afin de faciliter grandement le travail du DBA lors de la migration.
Ce document décrit essentiellement comment remplir le fichier Excel associé pour pouvoir réaliser la migration

PGM=$(basename ${0%%.ksh})
DATEEXEC=$(date "+%Y%m%d_%H%M%S")
LOGFILE=${DIRLOG}/$(basename ${0} | sed -e 's/.ksh$//')_${DATEEXEC}.log

log()
{
        echo ${PGM}'|'$(date "+%Y%m%d_%H%M%S")'|'${*} |  tee -a ${LOGFILE}
}
#main
log "`date`  example log  "
help
demodb.shl
#/bin/shl
OLD_UMASK=`umask`
umask 0027
mkdir -p workdir
mkdir -p workdir/admin/grisR2/adump
mkdir -p workdir/admin/grisR2/dpdump
mkdir -p workdir/admin/grisR2/pfile
mkdir -p workdir/audit
mkdir -p /applics/ora12203/cfgtoollogs/dbca/grisR2
mkdir -p /applics/ora12203/database/dbs
mkdir -p workdir
umask ${OLD_UMASK}
PERL5LIB=$ORACLE_HOME/rdbms/admin:$PERL5LIB; export PERL5LIB
ORACLE_SID=grisR2; export ORACLE_SID
PATH=$ORACLE_HOME/bin:$ORACLE_HOME/perl/bin:$PATH; export PATH
echo You should Add this entry in the /etc/oratab: demoint:/opt/ora12203/database:Y
/applics/ora12203/database/bin/sqlplus /nolog @Templatedir/DemoDb.sql


Demoint.sql
set verify off
ACCEPT sysPassword CHAR PROMPT 'Enter new password for SYS: ' HIDE
ACCEPT systemPassword CHAR PROMPT 'Enter new password for SYSTEM: ' HIDE
ACCEPT pdbAdminPassword CHAR PROMPT 'Enter new password for PDBADMIN: ' HIDE
host /opt/ora12203/database/bin/orapwd file=/opt/ra12203/database/dbs/orapwDbaName force=y format=12
/opt/ora12203/database/bin/sqlplus /nolog @Templatedir/DemoDb.sql


DemoDb.sql
@Templatedir/CreateDB.sql
@Templatedir/CreateDBFiles.sql
@Templatedir/CreateDBCatalog.sql
@Templatedir/lockAccount.sql
@Templatedir/postDBCreation.sql


Create sample createDB.sql 
SET VERIFY OFF
connect "SYS"/"&&sysPassword" as SYSDBA
set echo on
spool Templatedir/CreateDB.log append
startup nomount pfile="Templatedir/init.ora";
CREATE DATABASE "DemoDb"
MAXINSTANCES 8
MAXLOGHISTORY 1
MAXLOGFILES 16
MAXLOGMEMBERS 3
MAXDATAFILES 100
DATAFILE 'workdir/system01.dbf' SIZE 700M REUSE AUTOEXTEND ON NEXT  10240K MAXSIZE UNLIMITED
EXTENT MANAGEMENT LOCAL
SYSAUX DATAFILE 'workdir/sysaux01.dbf' SIZE 550M REUSE AUTOEXTEND ON NEXT  10240K MAXSIZE UNLIMITED
SMALLFILE DEFAULT TEMPORARY TABLESPACE TEMP TEMPFILE 'workdir/temp01.dbf' SIZE 20M REUSE AUTOEXTEND ON NEXT  640K MAXSIZE UNLIMITED
SMALLFILE UNDO TABLESPACE "UNDOTBS1" DATAFILE  'workdir/undotbs01.dbf' SIZE 200M REUSE AUTOEXTEND ON NEXT  5120K MAXSIZE UNLIMITED
CHARACTER SET WE8ISO8859P15
NATIONAL CHARACTER SET AL16UTF16
LOGFILE GROUP 1 ('workdir/redo01.log') SIZE 200M,
GROUP 2 ('workdir/redo02.log') SIZE 200M,
GROUP 3 ('workdir/redo03.log') SIZE 200M
USER SYS IDENTIFIED BY "&&sysPassword" USER SYSTEM IDENTIFIED BY "&&systemPassword";
spool off
================
demodb.init.ora
db_block_size=8192
open_cursors=300
db_name="grisR2"
control_files=("workdir/control01.ctl", "workdir/control02.ctl")
compatible=19.0.0
diagnostic_dest=workdir
nls_language="AMERICAN"
nls_territory="AMERICA"
processes=300
sga_target=1024m
audit_file_dest="workdir/admin/2/adump"
audit_trail=db
remote_login_passwordfile=EXCLUSIVE
dispatchers="(PROTOCOL=TCP) (SERVICE=DemoDbXDB)"
pga_aggregate_target=1024m
undo_tablespace=UNDOTBS1
======================
CreateDB.sql
connect "SYS"/"&&sysPassword" as SYSDBA
set echo on
spool Templatedir/CreateDB.log append
startup nomount pfile="Templatedir/init.ora";
CREATE DATABASE "grisR2"
MAXINSTANCES 8
MAXLOGHISTORY 1
MAXLOGFILES 16
MAXLOGMEMBERS 3
MAXDATAFILES 100
DATAFILE 'workdir/system01.dbf' SIZE 700M REUSE AUTOEXTEND ON NEXT  10240K MAXSIZE UNLIMITED
EXTENT MANAGEMENT LOCAL
SYSAUX DATAFILE 'workdir/sysaux01.dbf' SIZE 550M REUSE AUTOEXTEND ON NEXT  10240K MAXSIZE UNLIMITED
SMALLFILE DEFAULT TEMPORARY TABLESPACE TEMP TEMPFILE 'workdir/temp01.dbf' SIZE 20M REUSE AUTOEXTEND ON NEXT  640K MAXSIZE UNLIMITED
SMALLFILE UNDO TABLESPACE "UNDOTBS1" DATAFILE  'workdir/undotbs01.dbf' SIZE 200M REUSE AUTOEXTEND ON NEXT  5120K MAXSIZE UNLIMITED
CHARACTER SET WE8ISO8859P15
NATIONAL CHARACTER SET AL16UTF16
LOGFILE GROUP 1 ('workdir/redo01.log') SIZE 200M,
GROUP 2 ('workdir/redo02.log') SIZE 200M,
GROUP 3 ('workdir/redo03.log') SIZE 200M
USER SYS IDENTIFIED BY "&&sysPassword" USER SYSTEM IDENTIFIED BY "&&systemPassword";
spool off

CreateDBFiles.sql
SET VERIFY OFF
connect "SYS"/"&&sysPassword" as SYSDBA
set echo on
spool Templatedir/CreateDBFiles.log append
CREATE SMALLFILE TABLESPACE "USERS" LOGGING  DATAFILE  'workdir/users01.dbf' SIZE 5M REUSE AUTOEXTEND ON NEXT  1280K MAXSIZE UNLIMITED  EXTENT MANAGEMENT LOCAL  SEGME
NT SPACE MANAGEMENT  AUTO;
ALTER DATABASE DEFAULT TABLESPACE "USERS";
spool off

CreateDBCatalog.sql
SET VERIFY OFF
connect "SYS"/"&&sysPassword" as SYSDBA
set echo on
spool Templatedir/CreateDBCatalog.log append
@/opt/ora12203/database/rdbms/admin/catalog.sql;
@/opt/ora12203/database/rdbms/admin/catproc.sql;
@/opt/ora12203/database/rdbms/admin/catoctk.sql;
@/opt/ora12203/database/rdbms/admin/owminst.plb;
connect "SYSTEM"/"&&systemPassword"
@/opt/ora12203/database/sqlplus/admin/pupbld.sql;
connect "SYS"/"&&sysPassword" as SYSDBA
@/opt/ora12203/database/sqlplus/admin/pupdel.sql;
connect "SYSTEM"/"&&systemPassword"
set echo on
spool Templatedir/sqlPlusHelp.log append
@/opt/ora12203/database/sqlplus/admin/help/hlpbld.sql helpus.sql;
spool off
spool off
connect "SYS"/"&&sysPassword" as SYSDBA
set echo on
spool Templatedir/postDBCreation.log append

lockAccount.sql
connect "SYS"/"&&sysPassword" as SYSDBA
set echo on
spool Templatedir/lockAccount.log append
BEGIN
 FOR item IN ( SELECT USERNAME, AUTHENTICATION_TYPE FROM DBA_USERS WHERE ACCOUNT_STATUS IN ('OPEN', 'LOCKED', 'EXPIRED') AND USERNAME NOT IN (
'SYS','SYSTEM') )
 LOOP
IF item.AUTHENTICATION_TYPE='PASSWORD' THEN
  dbms_output.put_line('Locking and Expiring: ' || item.USERNAME);
  execute immediate 'alter user ' ||
         sys.dbms_assert.enquote_name(
         sys.dbms_assert.schema_name(
         item.USERNAME),false) || ' password expire account lock' ;
 ELSE
  dbms_output.put_line('Locking: ' || item.USERNAME);
  execute immediate 'alter user ' ||
         sys.dbms_assert.enquote_name(
         sys.dbms_assert.schema_name(
         item.USERNAME),false) || ' account lock' ;
 END IF;
 END LOOP;
END;
/
spool off

postDBCreation.sql

SET VERIFY OFF
spool Templatedir/postDBCreation.log append
host /opt/ora12203/database/OPatch/datapatch -skip_upgrade_check -db demoint;
connect "SYS"/"&&sysPassword" as SYSDBA
set echo on
create spfile='/ora/ora12203/database/dbs/spfiledemoint.ora' FROM pfile='Templatedir/init.ora';
connect "SYS"/"&&sysPassword" as SYSDBA
@/syst/adm/sql/fiab#ora/inst_basic.sql
select 'utlrp_begin: ' || to_char(sysdate, 'HH:MI:SS') from dual;
@/opt/ora12203/database/rdbms/admin/utlrp.sql;
select 'utlrp_end: ' || to_char(sysdate, 'HH:MI:SS') from dual;
select comp_id, status from dba_registry;
shutdown immediate;
connect "SYS"/"&&sysPassword" as SYSDBA
startup ;
spool off
exit;

===========
        inst_basic.sql
 
 ========================
 some help sql
 
 SELECT  p.name  pref_name,
        SUBSTR(DBMS_STATS.GET_PREFS(UPPER(p.name)), 1, 30) pref_value
FROM (
        SELECT 'autostats_target'          name FROM dual UNION ALL
        SELECT 'cascade'                   name FROM dual UNION ALL
        SELECT 'concurrent'                name FROM dual UNION ALL
        SELECT 'degree'                    name FROM dual UNION ALL
        SELECT 'estimate_percent'          name FROM dual UNION ALL
        SELECT 'method_opt'                name FROM dual UNION ALL
        SELECT 'no_invalidate'             name FROM dual UNION ALL
        SELECT 'granularity'               name FROM dual UNION ALL
        SELECT 'publish'                   name FROM dual UNION ALL
        SELECT 'incremental'               name FROM dual UNION ALL
        SELECT'incremental_staleness'      name FROM dual UNION ALL
        SELECT'incremental_level'          name FROM dual UNION ALL
        SELECT'stale_percent'              name FROM dual UNION ALL
        SELECT'global_temp_table_stats'    name FROM dual UNION ALL
        SELECT'options'                    name FROM dual
) p


audit trigger example
audit create session;
audit create user;
REM Audit sur ALTER USER par TRIGGER !!!
audit alter user;
audit drop user;
audit system grant;

declare
     v_count number(1);
begin
select count(1) into v_count from dba_tables where
table_name='AUDIT_EVENT_TABLE' and owner='SYSTEM';
if v_count=0 then
EXECUTE IMMEDIATE  'create table system.audit_event_table
(
 sysevent varchar2(20),
 timestamp date,
 login_user varchar2(30),
 client_terminal varchar2(255),
 instance_num number,
 database_name varchar2(50),
 dict_obj_name varchar2(30),
 dict_obj_type varchar2(20),
 dict_obj_owner varchar2(30),
 sql_text varchar2(2000),
 action number,
 action_name varchar2(27)
)
tablespace sysaux
';
     end if;
end;
/
==========
FUNCTION GET_CLOB_FROM_LONG(
SCHEMA_NAME VARCHAR2,
TABLE_NAME VARCHAR2,
COLUMN_NAME VARCHAR2,
V_ROWID ROWID) RETURN CLOB IS
v_clob clob;
cursor1 INTEGER; -- declare a cursor
ignore INTEGER; -- value is meaningless for a SELECT statement
statement VARCHAR2(2000); -- holds the SQL statement to be executed
out_val VARCHAR2(4000); -- value of the portion of the column returned
out_length INTEGER; -- length of the portion of the column returned
num_bytes INTEGER := 4000; -- length in bytes of the segment of the column
-- value to be selected
offset INTEGER; -- the byte position in the LONG column at which
-- the SELECT is to start
num_fetches INTEGER; -- the number of segments returned
row_count INTEGER := 0; -- the number of rows selected
BEGIN
dbms_lob.createtemporary(v_clob, true,dbms_lob.session);
statement := 'SELECT '|| COLUMN_NAME ||' FROM ' ||SCHEMA_NAME || '.' || TABLE_NAME || '  WHERE rowid = ''' || V_ROWID||''' ';
cursor1 := DBMS_SQL.OPEN_CURSOR;
DBMS_SQL.PARSE(cursor1, statement, DBMS_SQL.NATIVE);
/* Define the LONG column. */
DBMS_SQL.DEFINE_COLUMN_LONG(cursor1, 1);
/* Execute the query. */
ignore := DBMS_SQL.EXECUTE(cursor1);
/* Fetch the rows in a loop. Exit when there are no more rows. */
LOOP
IF DBMS_SQL.FETCH_ROWS(cursor1) > 0 THEN
row_count := row_count + 1;
offset := 0;
num_fetches := 1;
/* Get the value of the LONG column piece by piece. Here a loop
is used to get the entire column. The loop exits when there
is no more data. */
LOOP
DBMS_SQL.COLUMN_VALUE_LONG(cursor1, 1, num_bytes, offset,
out_val, out_length);
IF out_length != 0 THEN
DBMS_LOB.write(v_clob, out_length, offset + 1, out_val);
offset := offset + num_bytes;
num_fetches := num_fetches + 1;
ELSE EXIT;
END IF;
   IF out_length < num_bytes THEN EXIT;
   END IF;
   END LOOP;
     ELSE EXIT;
   END IF;
  END LOOP;
  DBMS_SQL.CLOSE_CURSOR(cursor1);
  return v_clob;
END GET_CLOB_FROM_LONG;


sys.show_hakan ??
create or replace TRIGGER system.after_alter AFTER ALTER on database
DECLARE
   sql_text ora_name_list_t;
   stmt VARCHAR2(2000);
   n pls_integer;
BEGIN
    IF (ora_dict_obj_type='USER') THEN
            n := ora_sql_txt(sql_text);
        IF (n>0) THEN
            FOR i IN 1..n LOOP
                stmt := stmt || sql_text(i);
            END LOOP;
        ELSE
            stmt := to_char(n);
        END IF;
 insert into system.audit_event_table
             (sysevent,
              timestamp,
              login_user,
              client_terminal,
              instance_num,
              database_name,
              dict_obj_name,
              dict_obj_type,
              dict_obj_owner,
              sql_text,
              action,
              action_name
             )
        values (ora_sysevent,
                sysdate,
                ora_login_user,
                ora_client_ip_address,
                ora_instance_num,
                ora_database_name,
                ora_dict_obj_name,
                ora_dict_obj_type,
                ora_dict_obj_owner,
                'text of triggering statement: ' ||stmt,
                43,
                'ALTER USER'
       );
    END IF;
END;



