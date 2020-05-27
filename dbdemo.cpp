#define _OPEN_THREADS
#define _LARGE_TIME_API
#include <iostream>
//#pragma managed(push, off)
#include <oratypes.h>
#include "log.h"
#include "rpt.h"
#include "Dbacc.h"
#include "utility.h"
#include "parametre.h"
#include "dbdemo.h"
//#pragma managed(pop)
using namespace oracle::occi;
using namespace std;
pthread_mutex_t lock_res;
pthread_mutex_t lock_db;
int nb_nLimit=200;
__thread int nTotal=0;
__thread int NPages=0;
__thread ResultSet    *rset1=NULL;
__thread id_import=0;
__thread id_ligne=0;
__thread Statement *stmt=NULL;
__thread Statement *stmt2=NULL;
__thread int ncnt=0;
__thread char * chkdir=NULL;
__thread char cpy[PATH_MAX] = "";
__thread char dirpath[PATH_MAX] = "";
__thread char * pathcopy=NULL;
__thread char work[PATH_MAX] = "";
__thread char *pc = NULL;
__thread int fd_to = -1;
__thread int fd_from = -1;
__thread int code_retour = 0;
__thread char errbuf[256] = "";
__thread size_t lcount        = 0;
__thread char lsz_sourceFile[PATH_MAX] = "";
__thread char lsz_bdocfile[PATH_MAX] = "";
__thread char lsz_docsigma[PATH_MAX]= " ";
__thread char lsz_cwd[PATH_MAX] = " ";
__thread int           ret = 0;
__thread nread =0;
__thread nwritten = 0;
__thread char buf[BUFSIZE] = {0};
__thread size_t size =0;
__thread struct stat64 infopath = {0};
__thread char *rest =NULL;
__thread cntr = 0;
/*-----------------------------------------------*/
/* doSomeWork                                    */
/* thread function                               */
/*-----------------------------------------------*/
void *doSomeWork( void * ptr) {
       /*Get the thread details */

         ThreadInfos *thInfos = new ThreadInfos;
         thInfos              =(ThreadInfos *) calloc(1, sizeof(ThreadInfos));

         thInfos              =  (ThreadInfos *)ptr; //get parameters from the main
         PgmInfos *pgminfos   = thInfos->pgmInfos;
         DatabaseInfo *dbt   = thInfos->db;

         thInfos->nTotal      = 0;

         nTotal        = 0;
         // get new connection  and Statement

         Connection *cn;
        //Statement *stmt;

         //---------------------------------//
         // create a Database access classe //
         //---------------------------------//

         Dbacc *dbc = new Dbacc(dbt->user,dbt->passwd,dbt->sid);

         //cn = pgminfos->cxn;

         cn = dbc->GetConnection();        // get connection from the connection pool
         if (!cn) {
             cout <<  "Error geting connection " << endl;
             return NULL;
string alt_stmt = "alter session set current_schema = " ;
         alt_stmt += dbt->schema;
         //cout <<"------------------------------------------------"<<endl;
         //cout << "alter statment <" << alt_stmt << "> " << endl;
         //cout <<"------------------------------------------------"<<endl;

         

       try {
          //pthread_mutex_lock(&lock_db);
          //Statement *stmt2 = cn->createStatement(alt_stmt);
          stmt2 = cn->createStatement(alt_stmt);
          stmt = cn->createStatement("select sysdate from dual;");
          stmt2->execute();
          cn->terminateStatement(stmt2);
          //pthread_mutex_unlock(&lock_db);
          }
          catch (SQLException ex)
                {
                        //cout << "Exception thrown for createStatement" << endl;
                        //cout << "Error number: " << ex.getErrorCode() << endl;
                        //cout << ex.getMessage() << endl;
                        LOG(LOGERR,"Exception thrown for createStatement" );
                        LOG(LOGERR, "Error number:%d ",  ex.getErrorCode());
                        LOG(LOGERR,"Message %s", ex.getMessage()); ;
                        if (dbc) delete dbc;
                        return NULL;
                }
                for (int r=0;r<agwd->ncontrat;r++){
           try {
              //pthread_mutex_lock(&lock_db);
              stmt->setSQL(";");
              stmt->setAutoCommit(FALSE);
              stmt->setSQL("BEGIN someStoredProcedure(:1,:2,:3,:4); END;");
              stmt->executeUpdate();
               stmt->registerOutParam (5, OCCICURSOR);
              cn->commit();
              catch (SQLException ex)
                {
                cout << "Exception thrown for executeUpdate " << r << " thread " << thInfos->idx_thread << endl;
                        cout << "Error number: " << ex.getErrorCode() << endl;
                        cout << ex.getMessage() << endl;
                        LOG(LOGERR,"Exception thrown for executeUpdate %d counter thread  %lu ", r, thInfos->idx_thread );
                        LOG(LOGERR, "Error number:%d ",  ex.getErrorCode());
                        LOG(LOGERR,"Message %s", ex.getMessage()); ;
                        cn->rollback();
                        cn->terminateStatement(stmt);
                        if (cn) dbc->ReleaseConnection(cn);
                        if (dbc) delete dbc;
                        return (NULL);
                }
         catch (exception &excp)
                {
                        cout << excp.what() << endl;
                }
       thInfos->ncnt++;
       } //  for loop end
       stmt->closeResultSet(rset1);
           rset1 = NULL;

//---------------------//
       // clean the statement //
       //---------------------//

       cn->terminateStatement(stmt);
       //--------------------------//
       // release connection handle//
       //--------------------------//
       if (cn) dbc->ReleaseConnection(cn);
       //---------------------//
       // release resource    //
       //---------------------//
       if (dbc) delete dbc;
        return (NULL);
}
docpy()


if(isDirectoryExist(dirpath) != 0){
             //LOG(LOGINFO,"dirpath %s not found for lsz_file %s ", dirpath, lsz_file) ;

                 //strncpy(cpy, ls_bdocfile.c_str(), sizeof(cpy) - 1);

                 snprintf(cpy,sizeof(cpy),"%s",ls_file.c_str());

                 rest = cpy;

                 memset(work,0,sizeof(work));

                 pc = strrchr(cpy, '/');
                 if(pc != NULL)
                    *pc=0;

                 strncpy(work, "/", sizeof(work) - 1);

                 pc = strtok_r(rest, "/",&rest);

                 while(pc != NULL) {
                     strcat(work, pc);
                    /* S_IRWXU = 00700 L'utilisateur (propri▒taire du fichier) a les auns de autho. lecture et ecriture */
                    /* S_IRWXG = 00070 Le groupe a les autorisations de lecture,  ecriture et execution */
                    /* S_IROTH = 00004 Tout le monde a l'autorisation de lecture. */
                    /* S_IXOTH = 00001 Tout le monde a l'autorisation d'ex▒cution. */

                    mkdir(work, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH );

                    strcat( work, "/" );
                    pc = strtok_r ( rest , "/" , &rest);

                   // original pc = strtok( NULL, "/");
  } // end of while pc pointer
               //LOG(LOGINFO,"created following directory  %s ", work);
             }// end if  isDirectoryExist
             

}


int main(int argc, char *argv[]) {

    char lsz_FichierInit[MAX_PATH] = "";
    char lsz_numThreads[2];
    char lsz_numContrat[6];
    char lsz_numNlimit[6];
    char lsz_typeTraitement[4];

    memset(lsz_typeTraitement,0,sizeof(lsz_typeTraitement));
    memset(lsz_numContrat ,0, sizeof(lsz_numContrat));
    memset(lsz_numThreads ,0, sizeof(lsz_numThreads));
    memset(lsz_FichierInit,0, sizeof(lsz_FichierInit));
    memset(lsz_numNlimit  ,0, sizeof(lsz_numNlimit));

    setvbuf(stdout, (char *) NULL, _IONBF, 0);

PgmInfos pgmInfos = {0};
ret=  f_GetParamIni(lsz_FichierInit,"NUM_THREADS" ,lsz_numThreads,1);
    if (ret != 0) {
        LOG(LOGERR, "NUM_THREADS variable it not set int migafer.ini file ");
        cout <<" NUM_THREADS variable is not set so default value 1 " << endl;
        nb_thread = 1;
    }
    else {
    LOG(LOGINFO, "lsz_numThreads %s ", lsz_numThreads);
    nb_thread = atoi(lsz_numThreads);
    }

   DatabaseInfo dbinfo;
    if ( getConnectionInfoagw(&agwd,NULL) == 0 ) {
       strcpy(agwd.sourceDir,sz_sourceDir);
       LOG(LOGINFO," NOM : %s",dbinfo.nom);
       LOG(LOGINFO," SID : %s",dbinfo.sid);
       LOG(LOGINFO," SCHEMA  : %s",dbinfo.schema);
      LOG(LOGINFO," Envivironment : %s",dbinfo.envt);
    }
    
    

   pthread_t *threads;

    threads = new pthread_t[nb_thread];

    long t;

pthread_mutex_init(&lock_res, NULL);
    pthread_mutex_init(&lock_db, NULL);

    for(int t=0;t<nb_thread;t++) {

    ThreadInfos *threadInfos = new ThreadInfos;

    threadInfos =(ThreadInfos *) calloc(1, sizeof(ThreadInfos));

    if ( threadInfos ==NULL ) {
        cout << "error allocation of threadInfos " <<endl;
        closeLogFile();
        return - 1;
    }

     threadInfos->idx_thread =t;
     threadInfos->db = &dbinfo;
     threadInfos->pgmInfos = &pgmInfos;
     threadInfos->lock_resource = lock_res;
     threadInfos->ncnt =0;
     ret = pthread_create(&threads[t], NULL, doSomeWork, (void*)threadInfos);
    if ( ret != 0 ){
         perror("Error creating thread ");
         LOG(LOGERR, "Impossible de lancer le thread");
         closeLogFile(); // close log file before leaving
         return(1);}
      else{
       LOG(LOGINFO, "Thread Number %d created ", t);
     }

   }// for loop no of threads
   void *pstatus;
   for (int t=0;t<nb_thread;t++){
        pthread_join(threads[t],&pstatus);
        cout << "Thread " << t << ": " << (char *)pstatus << endl;
        LOG(LOGINFO,"Joining thread %d ", t );
        delete [] (char *)pstatus;
    }

sleep(5);

   pthread_mutex_destroy(&lock_res);
   pthread_mutex_destroy(&lock_db);


   LOG(LOGINFO, "Fin du programme migafer : code retour = 0");
   closeLogFile();
   delete[] threads;

   return 0;
}

