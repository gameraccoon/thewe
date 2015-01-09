#include "Log.h"

#include <fstream>
#include <ctime>
#include <cocos2d.h>
#include <mutex>

#include "MiscUtils.h"

Log* Log::singleInstance = nullptr;
bool Log::isDestroyed = false;
bool Log::isFirstLife = true;

static std::mutex InstanceMutex;
static std::mutex FilestreamMutex;

Log::Log()
{
	const std::string LOG_FILE = Utils::GetWritablePath() + "log.htm";

	if (this->isFirstLife)
	{
		this->logFileStream = new std::ofstream(LOG_FILE, std::ios_base::trunc);
		this->writeInit("Log file created");
	}
	else
	{
		this->logFileStream = new std::ofstream(LOG_FILE, std::ios_base::app);
	}
}

Log::~Log()
{
	this->logFileStream->close();
	delete this->logFileStream;

	Log::singleInstance = nullptr;
	Log::isDestroyed = true;
	Log::isFirstLife = false;
}

Log& Log::Instance()
{
	std::lock_guard<std::mutex> lock(::InstanceMutex);

	if (Log::singleInstance == nullptr)
	{
		if (isDestroyed)
		{
			Log::onDeadReference();
		}
		else
		{
			Log::create();
		}
	}

	return *Log::singleInstance;
}

void Log::create()
{
	static Log theInstance;
	Log::singleInstance = &theInstance;
}

void Log::onDeadReference()
{
	// Get the ash (old location of the singlton)
	Log::create();
	// Create new singlton at this location
	new (Log::singleInstance) Log;
	// Add them to the queue to removing
	atexit(killPhoenixSingletone);
	// Change destruction flag
	Log::isDestroyed = false;
}

void Log::killPhoenixSingletone()
{
	Log::singleInstance->~Log();
	operator delete(Log::singleInstance);
}

void Log::writeError(const std::string& text)
{
	this->writeLine(std::string("<font color=\"red\"><b>Error</b>: ").append(text).append("</font><br/>"));
	cocos2d::log("Error: %s", text.c_str());
}

void Log::writeWarning(const std::string& text)
{
	this->writeLine(std::string("<font color=\"orange\"><b>Warning</b>: ").append(text).append("</font><br/>"));
	cocos2d::log("Warning: %s", text.c_str());
}

void Log::writeLog(const std::string& text)
{
	this->writeLine(std::string("<b>Log</b>: ").append(text).append("<br/>"));
	cocos2d::log("Log: %s", text.c_str());
}

void Log::writeInit(const std::string& text)
{
	this->writeLine(std::string("<font color=\"green\"><b>Init</b>: ").append(text).append("</font><br/>"));
}

void Log::writeLine(const std::string& text)
{
	std::lock_guard<std::mutex> lock(::FilestreamMutex);

	if (this->logFileStream->is_open())
	{
		*this->logFileStream << text << std::endl;
	}
}
