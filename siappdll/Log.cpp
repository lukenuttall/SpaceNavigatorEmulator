#include "Log.h"

INITIALIZE_EASYLOGGINGPP

#include <shlobj.h>
#include <Shlwapi.h>

Log::Log()
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
	{
		std::string path = std::string(szPath) + "\\SpaceNavigatorEmulator";

		if (::PathIsDirectory(path.c_str()) || 
			SUCCEEDED(::CreateDirectory(path.c_str(), NULL)))
		{
			auto configPath = path + "\\log.conf";
			el::Configurations conf;
			if (conf.parseFromFile(configPath))
			{
				el::Loggers::reconfigureAllLoggers(conf);
				LOG(INFO) << "Logging using configuration in " << configPath;
			}
			else
			{
				std::string filePath = path + "\\siappdll.log";
				conf.setToDefault();
				conf.setGlobally(el::ConfigurationType::Filename, filePath);
				el::Loggers::reconfigureLogger("default", conf);
				LOG(INFO) << "Logging with default settings to " << path;
			}
			LOG(TRACE) << "Debug logging on";
		}
		else
		{
			throw "Cannot log to Common AppData directory";
		}
	}
}