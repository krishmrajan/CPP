// Logger.h
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>
#include <time.h>

// Definition of a multithread safe singleton logger class
class Logger
{
public:
	static const std::string DEBUG;
	static const std::string INFO;
	static const std::string ERROR;
	static const std::string WARN;
	// Returns a reference to the singleton Logger object
	static Logger& instance();

	// Logs a single message at the given log level
	void log(const std::string& inMessage, 
		const std::string& inLogLevel);

	// Logs a vector of messages at the given log level
	void log(const std::vector<std::string>& inMessages, 
		const std::string& inLogLevel);
	bool Logger::openLogFile(std::string _logFile);
	

protected:
	// Static variable for the one-and-only instance  
	static Logger* pInstance;

	// Constant for the filename
	//static const char* const kLogFileName;
	static char* kLogFileName;
	// Data member for the output stream
	std::ofstream mOutputStream;

	char buffer[BUFSIZ] = { '\0' };
	time_t now;
	struct tm local_time;

	// Embedded class to make sure the single Logger
	// instance gets deleted on program shutdown.
	friend class Cleanup;
	class Cleanup
	{
	public:
		~Cleanup();
	};

	// Logs message. The thread should own a lock on sMutex
	// before calling this function.
	void logHelper(const std::string& inMessage, 
		const std::string& inLogLevel);

private:
	Logger();
	virtual ~Logger();
	Logger(const Logger&);
	Logger& operator=(const Logger&);
	static std::mutex sMutex;
};
