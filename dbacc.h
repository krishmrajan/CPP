/************************************************************************
Projet       : dbacc
Fichier      : dbacc.h

Description  : Header file description for OCCI interface to ORACLE 
               

Auteur       : KRA
Date creation: 14/11/2011

Histo modifs : 

Remarque : 
************************************************************************/

// 
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

class Dbacc
 { 

	public:

      oracle::occi::Environment* environment;  //environement handle 
      oracle::occi::Connection *con;           //connection handle 
      oracle::occi::Statement* stmt;           //oracle statement 
	
         
    public :
	   void ShowDate(string ddt);
       Dbacc(string user, string passwd, string db);
	   void ExecuteQry(string sql );
	    oracle::occi::Connection*  GetConnection  ();
		oracle::occi::Environment* GetEnvironment ();
      ~Dbacc();
    
 };


#endif // __DBACC_H__

