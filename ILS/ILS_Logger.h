#pragma once

#include <cstdio>
#include <memory>
#include <string>
#include <stdarg.h>


struct ILogger {
public:
	
	typedef std::string LogId;
	
	typedef std::string Msg;
	
	static const unsigned long max_msg_size = 1024;
	
	virtual void infOut(const Msg& msg, const LogId& id) const = 0;
	
	virtual void logOut(const Msg& msg, const LogId& id) const = 0;
	
	virtual void wrnOut(const Msg& msg, const LogId& id) const = 0;
	
	virtual void errOut(const Msg& msg, const LogId& id) const = 0;
protected: 
	friend struct Logger;
	
	virtual Msg msgTranslate(const LogId& id, const char* msg) const {
		
		Msg str = msg;
		size_t start_pos = 0;
		while ((start_pos = str.find("%t")) != std::string::npos) {
			str[start_pos + 1] = 'f';
		}
		return str;
	}
	
	virtual void onLogStart(bool l, bool w, bool e) {}
	
	virtual void onLogFinish(bool l, bool w, bool e) {}
public:
	
	void inf(const LogId& id, const char* msg, ...) const {
		char* str = new char[max_msg_size];
		try {
			va_list marker;
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, msgTranslate(id, msg).c_str(), marker);
			va_end(marker);
			infOut(str, id);
		}
		catch (...) {}
		delete[] str;
	}
	
	void log(const LogId& id, const char* msg, ...) const {
		char* str = new char[max_msg_size];
		try {
			va_list marker;
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, msgTranslate(id, msg).c_str(), marker);
			va_end(marker);
			logOut(str, id);
		}
		catch (...) {}
		delete[] str;
	}
	
	void wrn(const LogId& id, const char* msg, ...) const {
		char* str = new char[max_msg_size];
		try {
			va_list marker;
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, msgTranslate(id, msg).c_str(), marker);
			va_end(marker);
			wrnOut(str, id);
		}
		catch (...) {}
		delete[] str;
	}
	
	void err(const LogId& id, const char* msg, ...) const {
		char* str = new char[max_msg_size];
		try {
			va_list marker;
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, msgTranslate(id, msg).c_str(), marker);
			va_end(marker);
			errOut(str, id);
		}
		catch (...) {}
		delete[] str;
	}
	
	inline void dbg(const char* msg, ...) const {
#ifdef _DEBUG
		char* str = new char[max_msg_size + 6];
		strcpy(str, "DEBUG:");
		try {
			va_list marker;
			va_start(marker, msg);
			vsnprintf(str + 6, max_msg_size, msg, marker);
			va_end(marker);
			logOut(str, "dbg");
		}
		catch (...) {}
		delete[] str;
#endif //#ifdef _DEBUG
	}
	
	virtual double logParam(int param) const { return 0.; };
}; //struct Logger


struct Logger : public ILogger {
private: 
	mutable std::shared_ptr<ILogger> personal_logger;   
	mutable std::shared_ptr<ILogger> parent_logger;     
	
	std::shared_ptr<ILogger> logger() const {
		if (personal_logger != NULL) return personal_logger;
		else if (parent_logger != NULL) return parent_logger;
		else return NULL;
	}
public:
	
	virtual std::shared_ptr<ILogger> getPersonalLogger() const { return personal_logger; }
	
	virtual void setPersonalLogger(std::shared_ptr<ILogger> l = NULL) const { personal_logger = l; }
private:
	
	std::shared_ptr<ILogger> getParentLogger() const { return personal_logger; }
	
	void setParentLogger(std::shared_ptr<ILogger> l = NULL) const { personal_logger = l; }
public:  
	virtual Msg msgTranslate(const LogId& id, const char* msg) const {
		if (logger()) return logger()->msgTranslate(id, msg);
		else return msg;
	}
	virtual void infOut(const Msg& msg, const LogId& id) const { if (logger()) logger()->infOut(msg, id); }
	virtual void logOut(const Msg& msg, const LogId& id) const { if (logger()) logger()->logOut(msg, id); }
	virtual void wrnOut(const Msg& msg, const LogId& id) const { if (logger()) logger()->wrnOut(msg, id); }
	virtual void errOut(const Msg& msg, const LogId& id) const { if (logger()) logger()->errOut(msg, id); }
public:
	
	virtual double logParam(int param) const {
		if (logger()) return logger()->logParam(param);
		else return 0.;
	};
}; //struct Logger
