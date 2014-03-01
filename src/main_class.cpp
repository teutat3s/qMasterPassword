/*
 * Copyright (C) 2010-2011 Beat Küng <beat-kueng@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "main_class.h"
#include "version.h"

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>

/*********************************************************************//*
 * class CMain
 *//*********************************************************************/


CMain::CMain() : m_parameters(NULL), m_cl_parse_result(Parse_none_found)
{

}

CMain::~CMain()
{
	SAVE_DEL(m_parameters);
}



void CMain::init(int argc, char* argv[])
{
	parseCommandLine(argc, argv);
	
}

void CMain::parseCommandLine(int argc, char* argv[])
{

	SAVE_DEL(m_parameters);
	m_parameters = new CCommandLineParser(argc, argv);
	
	//init known arguments
	m_parameters->addSwitch("help", 'h');
	m_parameters->addSwitch("version");
	m_parameters->addSwitch("verbose", 'v');
	//control the logging
	m_parameters->addParam("log");
	m_parameters->addSwitch("no-log");
	m_parameters->addParam("file-log");
	m_parameters->addSwitch("no-file-log");
	
	
	//m_parameters->addTask("main task", 't');
	//m_parameters->addParam("output", 'o', "", "main task");
	//m_parameters->addSwitch("new-only", 'n', "main task");
	
	
	m_cl_parse_result = m_parameters->parse();
	
}

void CMain::printHelp()
{
	printf("Usage:\n"
		   " " APP_NAME " [-v] \n"
		   " " APP_NAME " --version\n"
		   "  -v, --verbose                   print debug messages\n"
		   "                                  (same as --log debug)\n"
		   "  -h, --help                      print this message\n"
		   "  --version                       print the version\n"
		   "\n"
		   " logging\n"
		   "  --log <level>                   set console log level\n"
		   "  --file-log <level>              set file log level\n"
		   "   <level>                        none, error, warn, info, debug\n"
		   "  --no-log                        no console logging (--log none)\n"
		   "  --no-file-log                   no file logging (--file-log none)\n"
		  );
}


void CMain::exec()
{

	ASSERT_THROW(m_parameters, ENOT_INITIALIZED);
	
	switch (m_cl_parse_result) {
	case Parse_none_found:
		printHelp();
		break;
	case Parse_unknown_command:
		wrongUsage("Unknown command: %s",
				   m_parameters->getUnknownCommand().c_str());
		break;
	case Parse_success:
		if (m_parameters->getSwitch("help")) {
			printHelp();
		} else if (m_parameters->getSwitch("version")) {
			printVersion();
		} else {
			processArgs();
		}
		break;
	}
}

void CMain::printVersion()
{
	printf("%s\n", getAppVersion().toStr().c_str());
}

void CMain::wrongUsage(const char* fmt, ...)
{

	printHelp();
	
	printf("\n ");
	
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	
	printf("\n");
	
}


void CMain::processArgs()
{

	//set console log level
	string level;
	if (m_parameters->getSwitch("verbose"))
		CLog::getInstance().setConsoleLevel(DEBUG);
	else if (m_parameters->getSwitch("no-log"))
		CLog::getInstance().setConsoleLevel(NONE);
	else if (m_parameters->getParam("log", level)) {
		ELOG log_level;
		if (CLog::parseLevel(level, log_level))
			CLog::getInstance().setConsoleLevel(log_level);
	}
	//set file log level
	if (m_parameters->getSwitch("no-file-log"))
		CLog::getInstance().setFileLevel(NONE);
	else if (m_parameters->getParam("file-log", level)) {
		ELOG log_level;
		if (CLog::parseLevel(level, log_level))
			CLog::getInstance().setFileLevel(log_level);
	}
	
	
	string device;
	if (m_parameters->getParam("device", device)) {
		//device set
	} else {
		//device is default value
	}
	
	if (m_parameters->setTask("main task")->bGiven) {
	
	}
	
}










