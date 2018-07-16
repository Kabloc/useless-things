#ifndef __LOGGER_H__
#define __LOGGER_H__

// To use multithread use this definition in preprocessor. MULTITHREAD_LOGGER

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <string>
#include <ctime>
#include <sys/stat.h>
#ifdef MULTITHREAD_LOGGER
#include <mutex>
#endif

/**************
** How to use **
 **************
logger output;
Nothing happens just a object will be created but will not create any log files neither verbose

To start logging call this member function to setup and start...

void start_logging(const std::string& log_file_name,       // Name of log file, can be "" if you just want verbose 
                   const uint32_t& log_max_file_size,      // Max size of each log file
                   const uint32_t& log_number_of_files,    // How many files can be created
				   LOGGER_LEVEL reporting_level = logINFO, // Level of report
                   bool verbose = true,                    // Verbosing to stderr
				   bool obfuscate = false,                 // Obfuscate the informations (is not a cryptografy)  ** just log files will affected **
				   const std::string& label = ""           // Label that will write on each line
				   const std::string& log_dir = ""         // Directory of logs
				   )

 *****************
***** CAUTION *****
 *****************
* This class will not verify any parameters, so make sure if:

If "log_file_name" is a valid name for file
If "log_max_file_size" and/or "log_number_of_files" are diferent from zero
If "log_dir" is valid, accessible and existing
 
If "log_dir" is empty, the log file will be created in the actual work dir.

***** ONLY VERBOSE *****
output.start_logging("", 0, 0, logDEBUG4);
To just verbose [stderr]... (no log files will be created)

***** ONLY LOG FILES OBFUCATED *****
output.start_logging("logger_test.log", 1024*500, 5, logDEBUG4, false, true);
Five log files will be created with 500 KB each, with names below

0.logger_test.log
1.logger_test.log
2.logger_test.log
3.logger_test.log
4.logger_test.log

When the 4th file reaches the size of 500 KB the first one will be overwritten

***** VERBOSE AND LOG FILES NOT OBFUSCATED *****
output.start_logging("logger_test.log", 1024*500, 5, logDEBUG4, true, false, "logger_test");

***** CODE SAMPLE OF LOGGING *****
LOGGER_OUT(output, logERROR)   << "This is a error message";          
LOGGER_OUT(output, logWARNING) << "This is a warning message";
LOGGER_OUT(output, logINFO)    << "This is a information message";
LOGGER_OUT(output, logDEBUG1)  << "This is a debug message level 1";
LOGGER_OUT(output, logDEBUG2)  << "This is a debug message level 2";
LOGGER_OUT(output, logDEBUG3)  << "This is a debug message level 3";
LOGGER_OUT(output, logDEBUG4)  << "This is a debug message level 4";

***** EXAMPLE OF LOG FILE LINES *****
12-07-2018 20:28:48.9356710|ERR|logger_test|This is a error message
12-07-2018 20:28:48.9383979|WRN|logger_test|This is a warning message
12-07-2018 20:28:48.9400073|INF|logger_test|This is a information message
12-07-2018 20:28:48.9453507|DB1|logger_test|This is a debug message level 1
12-07-2018 20:28:48.9474207|DB2|logger_test|This is a debug message level 2
12-07-2018 20:28:48.9496478|DB3|logger_test|This is a debug message level 3
12-07-2018 20:28:48.9518782|DB4|logger_test|This is a debug message level 4

*/

enum LOGGER_LEVEL { logNONE = -3,  logERROR, logWARNING, logINFO, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4 };

#define LOGGER_MIN_LEVEL logERROR
#define LOGGER_MAX_LEVEL logDEBUG4

#ifdef WIN32
#pragma pack (push)
#pragma warning (disable : 4996) // for std::localtime use.
#endif

template <typename T>
class Log {
public:
	Log(T& t) : t_(t) {}
	virtual ~Log() {
		os_ << std::endl;
		t_.output(os_.str(), t_.verbose());
	}
	std::ostringstream& get(LOGGER_LEVEL level) {
		os_ << t_.now_time() << "|" << to_string(level) << "|";
		if (t_.label().size()) os_ << t_.label() << "|";
		return os_;
	}
	std::string to_string(LOGGER_LEVEL level) {
		static const char* const buffer[] = { "ERR", "WRN", "INF", "DB1", "DB2", "DB3", "DB4" };
		return buffer[level + 2];
	}
protected:
	std::ostringstream os_;
private:
	T & t_;
	Log(const Log&) {}
	Log& operator=(const Log&) {}
};

class logger {
	friend class Log<logger>;
public:
	logger(const std::string& log_file_name, const uint32_t& log_max_file_size, const uint32_t& log_number_of_files,
		LOGGER_LEVEL reporting_level = logNONE, bool verbose = false, bool obfuscate = false, const std::string& label = "",
		const std::string& log_dir = "") {
		start_logging(log_file_name, log_max_file_size, log_number_of_files, reporting_level, verbose, obfuscate, label, log_dir);
	}

	void start_logging(const std::string& log_file_name, const uint32_t& log_max_file_size, const uint32_t& log_number_of_files,
		LOGGER_LEVEL reporting_level = logINFO, bool verbose = true, bool obfuscate = false, const std::string& label = "",
		const std::string& log_dir = "") {
#ifdef MULTITHREAD_LOGGER
		std::lock_guard<std::mutex> lock(mtx_);
#endif
		reporting_level_ = reporting_level;
		verbose_ = verbose;
		label_ = label;

		if (log_file_name == "" || !log_max_file_size || !log_number_of_files) return;
		file_name_ = log_file_name;
		max_file_size_ = log_max_file_size;
		number_of_files_ = log_number_of_files;
		obfuscate_ = obfuscate;
		log_dir_ = log_dir;
		next_file(); open();
	}

	inline logger() { }
	inline ~logger() { out_file_.close(); }
	inline const LOGGER_LEVEL& repoting_level() const { return reporting_level_; }

private:
	void output(const std::string& msg, bool verbose) {
#ifdef MULTITHREAD_LOGGER
		std::lock_guard<std::mutex> lock(mtx_);
#endif
		if (verbose) std::cerr << msg;
		if (!out_file_) return;
		if (obfuscate_) {
			std::string c(msg);
			out_file_ << obfuscate(c);
		}
		else out_file_ << msg;
		out_file_.flush();

		if (out_file_.tellp() >= max_file_size_) {
			out_file_.close();
			++actual_file_number_;
			actual_file_number_ %= number_of_files_;
			next_file(); open();
		}
	}

	inline std::string& obfuscate(std::string& s) {
		static char k[] = "W~IZV@Bf4|ipC1#nEP^:r*FAtu5&Lwdk`\"K>oXacb7g+%(j?93U][!$S,<eml_MHRO2T/h0 Q}GY6Jy8vN-D)=qx{;sz\\'.";
		std::for_each(s.begin(), s.end(), [&](char& c) { if (c >= 32 && c <= 126) c = k[c - 32]; });
		return s;
	}

	inline void open() {
		if (!max_file_size_) return;
		struct stat buf;
		if (!stat(current_file_name_.c_str(), &buf) && buf.st_size < max_file_size_)
			out_file_.open(current_file_name_, std::ios::app);
		else
			out_file_.open(current_file_name_, std::ios::trunc);
	}

	inline void next_file() {
		if (!max_file_size_) return;
		struct stat buf;
		std::ostringstream filename;
		for (uint32_t i = actual_file_number_; i < number_of_files_; ++i) {
			filename << log_dir_ << i << "." << file_name_;
			if (stat(filename.str().c_str(), &buf) || buf.st_size < max_file_size_) {
				current_file_name_ = filename.str();
				actual_file_number_ = i;
				return;
			}
			filename.str("");
		}
		filename << log_dir_ << actual_file_number_ << "." << file_name_;
		current_file_name_ = filename.str();
	}

	inline std::string now_time() {
#ifdef MULTITHREAD_LOGGER
		std::lock_guard<std::mutex> lock(mtx_);
#endif
		using namespace std::chrono;
		char buf[21]; // "DD-MM-YYYY HH:MM:SS"
		std::stringstream ss;
		system_clock::time_point now = system_clock::now();
		system_clock::duration tp = now.time_since_epoch();
		time_t t = system_clock::to_time_t(now);
		tp -= duration_cast<seconds>(tp);
		strftime(buf, sizeof(buf), "%d-%m-%Y %T", std::localtime(&t));
		ss << buf << "." << std::setfill('0') << std::setw(6) << duration_cast<microseconds>(tp).count();
		return ss.str();
	}

	inline const std::string& label() const { return label_; }
	inline const bool& verbose() const { return verbose_; }

	std::ofstream out_file_;
	std::string log_dir_;
	std::string file_name_;
	std::string current_file_name_;
	std::string label_;
	LOGGER_LEVEL reporting_level_ = { logNONE };
	uint32_t actual_file_number_ = { 0 };
	uint32_t number_of_files_ = { 0 };
	int32_t max_file_size_ = { 0 };
	bool obfuscate_ = { false };
	bool verbose_ = { false };
#ifdef MULTITHREAD_LOGGER
	std::mutex mtx_;
#endif
};

#ifdef WIN32
#pragma pack (pop)
#endif

#define LOGGER_OUT(output, level) \
	if (level > LOGGER_MAX_LEVEL || level < LOGGER_MIN_LEVEL);\
	else if (level > output.repoting_level()); \
	else Log<logger>(output).get(level)

#endif //__LOGGER_H__
