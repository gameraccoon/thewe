#ifndef LOG_H
#define LOG_H

#include <string>

/**
 * Class that realised writing log on several platforms
 * Realised as Phoenix singlton
 */
class Log
{
public:
	static Log& Instance();

	void writeError(const std::string& text);
	void writeWarning(const std::string& text);
	void writeLog(const std::string& text);
	void writeInit(const std::string& text);

private:
	/** Write line with timestamp */
	void writeLine(const std::string& text);

	std::ofstream *logFileStream;

	/** Construct information about singlton */
	static void create();

	/** Function that run when smd trying to use destroyed singlton */
	static void onDeadReference();

	/** Destruct singlton with ability to rebuild it */
	static void killPhoenixSingletone();

	static Log* singleInstance;

	static bool isDestroyed;
	static bool isFirstLife;

	Log();
	~Log();
	Log(const Log&);
	void operator=(const Log&);
};

#endif