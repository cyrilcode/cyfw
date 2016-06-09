
#ifndef CYFW_LOGGER_H
#define CYFW_LOGGER_H

#include <fstream>

namespace cy
{
    namespace logger
    {
        enum class level
        {
            VERBOSE = 2,
            WARNING = 1,
            ERROR = 0,
        };

    }

    extern std::string default_log_filename;

    template <typename LOG>
    class Logger {
        logger::level level;
        LOG log;
    public:
        Logger(logger::level l = logger::level::ERROR, std::string n = "output.log") : level(l), log(n) { }
        inline void verbose(std::string s) { if (level == logger::level::VERBOSE) log(logger::level::VERBOSE, s); }
        inline void warning(std::string s) { if (level >= logger::level::WARNING) log(logger::level::WARNING, s); }
        inline void error(std::string s) { if (level >= logger::level::ERROR) log(logger::level::ERROR, s); }
    };

    class NullLogger {
    public:
        NullLogger(std::string) {}
        inline void operator()(logger::level level, std::string s) { }
    };

    class ConsoleLogger {
    public:
        ConsoleLogger(std::string) {}
        inline void operator()(logger::level level, std::string s) { std::cout << s << std::endl; }
    };

    class FileLogger {
        std::ofstream ofs;
    public:
        FileLogger(std::string n) {
            ofs.open(n, std::ios::out | std::ios::app);
            if (!ofs.is_open()) throw "Error opening log file";
        }
        inline void operator()(logger::level level, std::string s) {
            if (level == logger::level::VERBOSE) {
                ofs << std::string("Verbose: ");
            }
            else if (level == logger::level::WARNING) {
                ofs << std::string("Warning: ");
            }
            else if (level == logger::level::ERROR) {
                ofs << std::string("!!!ERROR!!! ");
            }
            ofs << s << "\n";
            ofs.flush();
        }
        ~FileLogger() { if (ofs.is_open()) ofs.close(); }
    };
}


#endif
