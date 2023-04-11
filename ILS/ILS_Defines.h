#ifndef ILS_DefinesH
#define ILS_DefinesH

#include "ILS_LoggerStream.h"


#define ILS_LOG(LOG_ARG) {TLoggerStream(this,&ILogger::logOut)LOG_ARG;}
#define ILS_LOG_(PTR, LOG_ARG) {TLoggerStream(PTR,&ILogger::logOut)LOG_ARG;}


#define ILS_WRN(LOG_ARG)  {TLoggerStream(this,&ILogger::wrnOut)LOG_ARG;}
#define ILS_WRN_(PTR, LOG_ARG)  {TLoggerStream(PTR,&ILogger::wrnOut)LOG_ARG;}


#define ILS_SECTB(SECTID, LOG_ARG) {\
	TLoggerStream oSection##SECTID(this,&ILogger::infOut,#SECTID); \
	oSection##SECTID.SectBegin LOG_ARG; \
	oSection##SECTID.Flush();\
	try


#define ILS_SECTBI(SECTID, INDEX, LOG_ARG) {\
	TLoggerStream oSection##SECTID(this,&ILogger::infOut,#SECTID,INDEX); \
	oSection##SECTID.SectBegin LOG_ARG; \
	oSection##SECTID.Flush();\
	try


#define ILS_SECTE(SECTID, LOG_ARG) \
	catch(const std::exception& e)  {wrn("SectException", "Секция %s не завершена из-за: %s", oSection##SECTID.SectId(), e.what());throw;}\
	catch(...) {wrn("SectException", "Секция %s не завершена из-за: %s", oSection##SECTID.SectId(), "unknown");throw;}\
	oSection##SECTID.SectEnd LOG_ARG;\
	}

#define ILS_SECTEI(SECTID, INDEX, LOG_ARG) \
	catch(const std::exception& e)  {wrn("SectException", "Секция %s не завершена из-за: %s", oSection##SECTID.SectId(), e.what());throw;}\
	catch(...) {wrn("SectException", "Секция %s не завершена из-за: %s", oSection##SECTID.SectId(), "unknown");throw;}\
	oSection##SECTID.SectCheck(#SECTID, INDEX);\
	oSection##SECTID.SectEnd LOG_ARG;\
	}

#endif  // ILS_DefinesH
