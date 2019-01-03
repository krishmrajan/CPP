// Logger.cpp
// Implementation of a multithread safe singleton logger class
#include <stdexcept>
#include "Logger.h"

using namespace std;

const string Logger::DEBUG = "[DEBUG]";
const string Logger::INFO = "[INFO]";
const string Logger::ERROR = "[ERROR]";
const string Logger::WARN = "[Warning]";

//const char* const Logger::kLogFileName = "log.out";
 char* Logger::kLogFileName = "log.out";

Logger* Logger::pInstance = nullptr;

mutex Logger::sMutex;

Logger& Logger::instance()
{
	static Cleanup cleanup;

	lock_guard<mutex> guard(sMutex);
	if (pInstance == nullptr)
		pInstance = new Logger();
	return *pInstance;
}

Logger::Cleanup::~Cleanup()
{
	lock_guard<mutex> guard(Logger::sMutex);
	delete Logger::pInstance;
	Logger::pInstance = nullptr;
}

Logger::~Logger()
{
	mOutputStream.close();
}

Logger::Logger()
{
	/*mOutputStream.open(kLogFileName, ios_base::app);
	if (!mOutputStream.good()) {
		throw runtime_error("Unable to initialize the Logger!");
	} */
}

void Logger::log(const string& inMessage, const string& inLogLevel)
{
	lock_guard<mutex> guard(sMutex);
	logHelper(inMessage, inLogLevel);
}

void Logger::log(const vector<string>& inMessages, const string& inLogLevel)
{
	lock_guard<mutex> guard(sMutex);
	for (size_t i = 0; i < inMessages.size(); i++) {
		logHelper(inMessages[i], inLogLevel);
	}
}

bool Logger::openLogFile(std::string _logFile)
{
	mOutputStream.open(_logFile, ios_base::app);
	if (!mOutputStream.good()) {
		throw runtime_error("Unable to initialize the Logger!");
		return false;
	}
	return true;
}

void Logger::logHelper(const std::string& inMessage, const std::string& inLogLevel)
{
	now = time(&now);
	localtime_s(&local_time, &now);
	strftime(buffer, BUFSIZ, "%Y-%m-%d %H:%M:%S", &local_time);
	mOutputStream <<  buffer <<  ":" <<  inLogLevel << ": " << inMessage << endl;
	buffer[BUFSIZ] = { '\0' };
}

