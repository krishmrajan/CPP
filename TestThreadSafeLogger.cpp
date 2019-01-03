// TestThreadSafeLogger.cpp
#include "Logger.h"
#include <vector>
#include <string>
#include <time.h>
#include <sys\utime.h>

using namespace std;

int main()
{
	Logger::instance().openLogFile("test.out");
	Logger::instance().log("test message", Logger::DEBUG);
	vector<string> items = {"item1", "item2"};
	Logger::instance().log(items, Logger::ERROR);

/*	 //http://www.daniweb.com/forums/thread64803.html
  char buffer[BUFSIZ] = { '\0' } ;    
  time_t now = time( &now ) ;
  struct tm local_time; 
  //http://msdn.microsoft.com/en-us/library/a442x3ye(VS.80).aspx
  localtime_s( &local_time, &now ) ;
  cout<<"\nApproach 3"<<endl;
  //http://msdn.microsoft.com/en-us/library/fe06s4ak(VS.71).aspx
  strftime( buffer, BUFSIZ, "%Y-%m-%d  %H:%M:%S", &local_time ) ;    
  cout<<"Current date is: "<< buffer << endl ;
  strftime( buffer, BUFSIZ, "%H:%M:%S.000", &local_time ) ;
  //cout<<"Current time is: "<< buffer << endl ;
	
  buffer[BUFSIZ] = { '\0' };
  */
	
}