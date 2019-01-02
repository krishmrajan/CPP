/****************************************************************************
storedproc.cpp - OCCI sample demonstrating calling PL/SQL stored procedures
and functions from OCCI.

Check attached README.txt for more details.

Report any errors and suggestions in OCCI Discussion forum in OTN.
*****************************************************************************/

#include <iostream>
#include <string>
#include <vector>

#include <occi.h>

using namespace std;
using namespace oracle::occi;

main()
{
  try
  {
   //initialize in object mode since we are accessing VARRAY types
   Environment *env = Environment::createEnvironment(Environment::OBJECT);
   Connection *conn = env->createConnection("scott","tiger");
   Statement *stmt = conn->createStatement();

   try
   {//different scope from Environment object for application logic 

    cout << "******Calling GetBookDetails******" << endl;

   //Stored Procedure 1 - shows basic datatypes and VARRAY
   // procedure GetBookDetails(pId in number, pTitle out varchar2,
   // pPrice out number, pPubDate out date, pKeywords out KeywordsList)

    //create anonymous block for executing stored proc/function
    stmt->setSQL("begin OCCIDEMO1.GetBookDetails(:1, :2, :3, :4, :5); end;");
    int id = 1;//book to retrieve

    //pass IN parameters by setXXX(), indicate OUT parameters 
    //by registerOutParam
    stmt->setNumber(1, id);//1 - IN - Id
    stmt->registerOutParam(2, OCCISTRING, 100); //2 - OUT - title,max=100
    stmt->registerOutParam(3, OCCIDOUBLE); //3 - OUT - price
    stmt->registerOutParam(4, OCCIDATE); //4 - OUT - pubdate
    stmt->registerOutParam(5, OCCIVECTOR, 0,
         "SCOTT.KEYWORDSLIST"); //5 - OUT - keywords(varray)

    stmt->execute(); //execute procedure

    //get the OUT parameters
    string title = stmt->getString(2);
    double price = stmt->getDouble(3);
    Date pubdate = stmt->getDate(4); //OCCI Date class
    vector<string> keywords;
    //there are overloaded getVector methods for each vector<T>
    getVector(stmt, 5, keywords);

    cout << "Book Details : " <<  title << " " << price << " " << 
         pubdate.toText() << endl;
    cout << "Keywords : ";
    for (int i = 0 ; i < keywords.size(); i++)
       cout << keywords[i] << ",";
    cout << endl;

    cout << "******Calling GetBookList******" << endl;

   //Stored Function 2 - shows PL/SQL tables and function returning value
   // function  GetBookList(pIds in IdList, pBooks out BookList) 
   // return number

    stmt->setSQL("begin :1 := OCCIDEMO1.GetBookList(:2, :3); end;");
    int idlist[10];
    char booklist[10][200];
    ub4 inelems=3,outelems=0;
    ub2 outelemlens[10], inelemlens[10];
    idlist[0] = 1;idlist[1] = 2;idlist[2] = 3;//book Ids 1,2,3
    inelemlens[0] = inelemlens[1] = inelemlens[2] =  sizeof(int);

/* To pass/retrieve PL/SQL tables use setDataBufferArray method :-
     void setDataBufferArray(unsigned int paramIndex,
        void *array, Type type, ub4 arraySize, ub4 *arrayElements,
        sb4 elementSize, ub2 *elementLengths, sb2 *ind = NULL, ub2 *rc = NULL)

   After the execute, the buffer will contain OUT values
*/
    stmt->setDataBufferArray(2, idlist, OCCIINT, 10, &inelems,
          sizeof(int), inelemlens);//2 - IN - TABLE OF NUMBER
    stmt->setDataBufferArray(3, booklist, OCCI_SQLT_STR, 10, &outelems,
          200, outelemlens);//3 - OUT - TABLE OF VARCHAR2
    stmt->registerOutParam(1, OCCIINT); //1 - Function return value

    stmt->execute(); //execute the function

    int ret = stmt->getInt(1);//Function return value
    cout << "Function return : " << ret << endl;

    cout << "Elements returned : " << outelems << endl;
    for (int j = 0; j < outelems; j++)
      cout << booklist[j] << "[" << outelemlens[j] << "]" << endl;

    cout << "******Calling SearchBooks******" << endl;

    //Stored Proc 3 - returns a REF cursor for a query
    // procedure SearchBooks(pKeyword in out varchar2, pBookCur out BookCur) 

    stmt->setSQL("begin OCCIDEMO1.SearchBooks(:1, :2); end;");
    //for IN/OUT, input with setXXX, do not call registerOutParam
    stmt->setString(1, "Databases"); //1 - IN/OUT - varchar
    stmt->setMaxParamSize(1, 100);//max param size for string parameter
    stmt->registerOutParam(2, OCCICURSOR);//2 - OUT - Ref Cursor
  
    stmt->execute();//execute stored procedure

    cout << "Searched using keyword: " << stmt->getString(1) << endl;
    ResultSet *rs = stmt->getCursor(2);//use ResultSet to fetch rows
    //we know cursor has 1 varchar column in Select, use getString()
    while (rs->next())
      cout << "Book : " << rs->getString(1) << endl;

    stmt->closeResultSet(rs);
   }
   catch (SQLException &ex)
   {//cleanup
    cout << "Error, cleaning up..." << endl;
    conn->terminateStatement(stmt);
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);
    throw;//will be caught by outer handler
   }

   conn->terminateStatement(stmt);

   env->terminateConnection(conn);
   Environment::terminateEnvironment(env);

   cout << "Demo completed" << endl;
  } catch (SQLException &ex)
  {
      cout << "Error running demo : " << ex.getMessage() << endl;
  }
}
