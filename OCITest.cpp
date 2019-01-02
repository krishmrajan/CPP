#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;

class  occidml
{
  private:

  Environment *env;
  Connection *conn;
  Statement *stmt;
  public:

  occidml (string user, string passwd, string db)
  {
    env = Environment::createEnvironment (Environment::DEFAULT);
    conn = env->createConnection (user, passwd, db);
  }

  ~occidml ()
  {
    env->terminateConnection (conn);
    Environment::terminateEnvironment (env);
  }
};

int main()
{
    string user = "scott";
  string passwd = "tiger";
  string db = "localhost:1521/FAFWDEV01";

  try
  {
    cout << "occidml - Exhibiting simple insert, delete & update operations"
      << endl;
    occidml *demo = new occidml (user, passwd, db);

    delete (demo);
    cout << "occidml - done" << endl;
  }
  catch (SQLException ea)
  {
    cerr << "Error running the demo: " << ea.getMessage () << endl;
  }
}
