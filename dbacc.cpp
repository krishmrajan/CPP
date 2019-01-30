#include "dbacc.h"
#include <SI_serr.h>
#include <SI_ost.h>
#include <SI_ost2cpte.h>
#include <SI_ost2vale.h>
#include <SI_site.h>
#include <SI_ost2tycp.h>
#include <SI_etai.h>
#include <SI_etao.h>
//#include <iostream>
//#include <occi.h>
//using namespace oracle::occi;
//using namespace std;
/*************************************************************/
/** Classe Dbacc : destructor                               **/
/**                                                         **/
/** In  :                                                   **/
/** Out :                                                   **/
/**                                                         **/
/*************************************************************/
//oracle::occi::Environment* Dbacc::con;

Dbacc ::~Dbacc ()
{
    environment->terminateConnection (con);
    oracle::occi::Environment::terminateEnvironment (environment);
}
Dbacc :: Dbacc(string user, string passwd, string db)
{
  try{  
         environment = oracle::occi::Environment::createEnvironment(oracle::occi::Environment::DEFAULT);
         con = environment->createConnection(user, passwd,db); 

     }catch(oracle::occi::SQLException &e){
         std::cout<<e.what();
     }
  	 
}
void Dbacc :: ShowDate(string ddt)
{

Date to_day = oracle::occi::Date::getSystemDate(environment);
string tsstr=to_day.toText("dd/mm/yyyy hh:mi:ss [tzh:tzm]",0);
cout<<tsstr<<endl;
}



oracle::occi::Environment* Dbacc ::  GetEnvironment ()
{
return environment;
} 
oracle::occi::Connection* Dbacc ::  GetConnection  ()
{
return con;
}

void Dbacc :: ExecuteQry (string sql )
{
oracle::occi::Statement *stmt = NULL; 
oracle::occi::ResultSet *rs =NULL;
  try 
  { 
    
	
    stmt = con->createStatement(sql); 
  } 
  catch (oracle::occi::SQLException &ex) 
  { 
    cout << ex.getMessage(); 
  }
  if (stmt) 
  { 
    try 
    { 
      //stmt->setPrefetchRowCount(32);
      
      rs = stmt->executeQuery(); 
    } 
    catch (oracle::occi::SQLException &ex) 
    { 
      cout << ex.getMessage(); 
    }
	if (rs)
	{
	while (rs->next()) { 
	    cout << rs->getString(1) << endl;
        
      }
	stmt->closeResultSet(rs); 
   }
   con->terminateStatement(stmt); 
   }
}
// end of classe 
 
  


