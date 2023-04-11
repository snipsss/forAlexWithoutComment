#pragma once

#include <stdio.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <chrono>
#include "ILS_Logger.h"


class BaseLogger : public ILogger {
	
protected: 
	mutable unsigned int errors = 0;
	
	mutable unsigned int warnings = 0;
	
public: 
	BaseLogger();
	
public: 
	mutable unsigned int show_info;
	
	mutable bool bStarted;
	
	mutable bool bLogToConsole;
	
	mutable std::chrono::steady_clock::time_point start_time;
protected: 
	virtual void infOut(const Msg& msg, const LogId& id) const;
	
	virtual void logOut(const Msg& msg, const LogId& id) const;
	
	virtual void wrnOut(const Msg& msg, const LogId& id) const;
	
	virtual void errOut(const Msg& msg, const LogId& id) const;
protected: 	virtual std::string title() const;
	
	virtual std::string lTitle() const;
	
	virtual std::string iTitle() const;
	
	virtual std::string wTitle() const;
	
	virtual std::string eTitle() const;
	
protected: 
	virtual void lOut(const std::string& msg) const = 0;
	
	virtual void wOut(const std::string& msg) const = 0;
	
	virtual void eOut(const std::string& msg) const = 0;
}; 


class StdLogger : public BaseLogger {
protected:
	using BaseLogger::bLogToConsole;
	
	mutable std::ostream *log_out, *wrn_out, *err_out;
	bool l_del,w_del,e_del;
	
public: 
	StdLogger(std::ostream& l_out=std::cout,
	          std::ostream& w_out=std::cerr,
	          std::ostream& e_out=std::cerr) {
		log_out = &l_out; l_del=false;
		wrn_out = &w_out; w_del=false;
		err_out = &e_out; e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}

	
	StdLogger(std::string l_out_file,
	          std::ostream& w_out=std::cerr,
	          std::ostream& e_out=std::cerr,
			  std::ios_base::openmode mode = std::ios_base::out
			  ) {
		using namespace std;
		log_out = NULL;l_del=false;;
		if(l_out_file!="") {
			std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(),mode|std::ios_base::out);
			if(*log_stream) {log_out = log_stream;l_del=true;}
			else delete log_stream;
		}
		wrn_out = &w_out; w_del=false;
		err_out = &e_out; e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	
	StdLogger(std::string l_out_file,
	          std::string w_out_file,
	          std::ostream& e_out=std::cerr,
			  std::ios_base::openmode mode = std::ios_base::out
			  ) {
		using namespace std;
		log_out = NULL;l_del=false;;
		if(l_out_file!="") {
			std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(),mode|std::ios_base::out);
			if(*log_stream) {log_out = log_stream;l_del=true;}
			else delete log_stream;
		}
		wrn_out = NULL;w_del=false;;
		if(w_out_file!="") {
			std::ofstream* wrn_stream = new std::ofstream(w_out_file.c_str(),mode|std::ios_base::out);
			if(*wrn_stream) {wrn_out = wrn_stream;w_del=true;}
			else delete wrn_stream;
		}
		err_out = &e_out; e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	
	StdLogger(std::string l_out_file,
	          std::string w_out_file,
	          std::string e_out_file,
			  std::ios_base::openmode mode = std::ios_base::out
			  ) {
		using namespace std;
		log_out = NULL;l_del=false;;
		if(l_out_file!="") {
			std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(),mode|std::ios_base::out);
			if(*log_stream) {log_out = log_stream;l_del=true;}
			else delete log_stream;
		}
		wrn_out = NULL;w_del=false;;
		if(w_out_file==l_out_file && w_out_file!="") {
			wrn_out = log_out;
			w_del = false;
		} else if(w_out_file!="") {
			std::ofstream* wrn_stream = new std::ofstream(w_out_file.c_str(),mode|std::ios_base::out);
			if(*wrn_stream) {wrn_out = wrn_stream;w_del=true;}
			else delete wrn_stream;
		}
		err_out = NULL;e_del=false;;
		if(e_out_file==l_out_file && e_out_file!="") {
			err_out = log_out;
			e_del = false;
		} else if(e_out_file==w_out_file && e_out_file!="") {
			err_out = wrn_out;
			e_del = false;
		} else if(e_out_file!="") {
			std::ofstream* err_stream = new std::ofstream(e_out_file.c_str(),mode|std::ios_base::out);
			if(*err_stream) {err_out = err_stream;e_del=true;}
			else delete err_stream;
		}
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	
	StdLogger(const StdLogger& src) {
		if(src.l_del) log_out=NULL; 
		else log_out = src.log_out;
		if(src.w_del) wrn_out=NULL; 
		else wrn_out = src.wrn_out;
		if(src.e_del) err_out=NULL; 
		else err_out = src.err_out;
		l_del=false;
		w_del=false;
		e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	
	virtual ~StdLogger() {
		BaseLogger::onLogFinish(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
		if(l_del && log_out) delete log_out;
		if(w_del && wrn_out) delete wrn_out;
		if(e_del && err_out) delete err_out;
	}
	
protected: 
	virtual void ConsoleOut(const std::string& msg) const {
		if (bLogToConsole)
			std::cout << msg << std::endl;
	}
	virtual void lOut(const std::string& msg) const {
		if (log_out) (*log_out) << msg << std::endl;
		ConsoleOut(msg);
	}
	virtual void wOut(const std::string& msg) const {
		if (wrn_out) (*wrn_out) << msg << std::endl;
		ConsoleOut(msg);
	}
	virtual void eOut(const std::string& msg) const {
		if (err_out) (*err_out) << msg << std::endl;
		ConsoleOut(msg);
	}
}; //struct StdLogger
