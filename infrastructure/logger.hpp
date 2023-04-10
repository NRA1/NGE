#ifndef NGE_LOGGER_HPP
#define NGE_LOGGER_HPP
//
#include <string>
#include <utility>
#include <ctime>
#include <iostream>
#include <boost/stacktrace.hpp>
#include "../enums/log_level.hpp"
//
//#define cmm Logger::Action(0, 0)
//#define trace() Logger::Action(1, 0)
//#define fault(code) Logger::Action(2, code)
//#define stdOut Logger::Target(0, "")
////#define file(path) Logger::Target(1, path);
//#define info Logger::Info
//#define debug Logger::Debug
//#define critical Logger::Critical
//#define fatal Logger::Fatal
//
//class Logger
//{
//public:
//
//    enum LogLevel
//    {
//        Info,
//        Debug,
//        Critical,
//        Fatal
//    };
//
//    class Action
//    {
//    public:
//        explicit Action(int code, int ret)
//        {
//            this->code_ = code;
//            this->ret_ = ret;
//        }
//
//    private:
//        int code_;
//        int ret_;
//
//        friend Logger;
//    };
//
//    class Target
//    {
//    public:
//        Target(int code, std::string path) : code_(code), path_(std::move(path)) { }
//
//    private:
//        int code_;
//        std::string path_;
//
//        friend Logger;
//    };
//
//    Logger();
//    Logger(const Logger &logger);
//
//    void setTarget(const Target &target);
//
//    Logger operator<<(const LogLevel &level) const;
//    Logger operator<<(const std::string &string) const;
//    Logger operator<<(const char string[]) const;
//    Logger operator<<(const int &num) const;
//    Logger operator<<(const float &num) const;
//    void operator<<(const Action &action) const;
//    Logger operator<(const std::string &string) const;
//    Logger operator<(const char string[]) const;
//    Logger operator<(const int &num) const;
//    Logger operator<(const float &num) const;
//
//private:
//    void commit() const;
//    std::string levelStr(const LogLevel &level) const;
//
//    struct Message
//    {
//        LogLevel level;
//        std::string message;
//    } message_;
//
//    Target target_;
//};
//
//static Logger log = Logger();
//
////using namespace logger;
//

#define trace Log::Action(1, 0)

class Log
{
public:

    class Action
    {
    public:
        explicit Action(int code, int ret)
        {
            this->code_ = code;
            this->ret_ = ret;
        }

    private:
        int code_;
        int ret_;

        friend Log;
    };

    class Target
    {
    public:
        Target() : code_(1) { }
        Target(std::string path) : code_(2), path_(std::move(path)) { }

    private:
        int code_;
        std::string path_;

        friend Log;
    };

    static void setDefaultTarget(const Target &target);

    Log();


    Log &operator-(LogLevel level);
    Log &operator<(std::string string);
    Log &operator<(const char string[]);
    Log &operator<(int num);
    Log &operator<(float num);
    Log &operator<<=(const Action &action);
    Log &operator>>=(const Target &target);
    Log &operator<=(const std::string &string);
    Log &operator<=(const char string[]);
    Log &operator<=(int num);
    Log &operator<=(float num);
//    Log &operator&&(Action &action);
//    Log &operator>>=(Target &target);

    ~Log();

private:
    static Target default_target_;

    std::string message_;
    LogLevel level_;
    std::vector<Action> actions_;
    Target target_;
};

Log log();

#endif //NGE_LOGGER_HPP