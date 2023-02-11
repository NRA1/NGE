//#include "logger.hpp"
//
//Logger::Logger() : target_(stdOut) { }
//
//Logger::Logger(const Logger &logger) : target_(logger.target_), message_(logger.message_)
//{
//}
//
//Logger Logger::operator<<(const Logger::LogLevel &level) const
//{
//    Logger log(*this);
//    log.message_.level = level;
//    return std::move(log);
//}
//
//Logger Logger::operator<<(const std::string &string) const
//{
//    Logger log(*this);
//    if(!log.message_.message.ends_with(' ') && !log.message_.message.empty())
//        log.message_.message.append(" ");
//    log.message_.message.append(string);
//    return log;
//}
//
//Logger Logger::operator<<(const char string[]) const
//{
//    return this->operator<<(std::string(string));
//}
//
//Logger Logger::operator<<(const int &num) const
//{
//    return this->operator<<(std::to_string(num));
//}
//
//Logger Logger::operator<<(const float &num) const
//{
//    return this->operator<<(std::to_string(num));
//}
//
//void Logger::operator<<(const Logger::Action &action) const
//{
//    commit();
//    if(action.code_ == 1 || action.code_ == 2)
//    {
//#ifdef DEBUG
//        std::cout << boost::stacktrace::stacktrace();
//#endif
//    }
//    if(action.code_ == 2)
//    {
//        exit(action.ret_);
//    }
//}
//
//void Logger::setTarget(const Logger::Target &target)
//{
//    target_ = target;
//}
//
//
//void Logger::commit() const
//{
//    time_t timer = time(nullptr);
//    struct tm *tm = localtime(&timer);
//    char buff[26];
//    strftime(buff, 26, "%D %T ", tm);
//    std::string message = std::string(buff);
//    message.append(levelStr(this->message_.level));
//    message.append(" ");
//    message.append(this->message_.message);
//
//    if(target_.code_ == 0)
//    {
//        std::cout << message << std::endl;
//    }
//}
//
//std::string Logger::levelStr(const Logger::LogLevel &level) const
//{
//    if(level == LogLevel::Info)
//        return "INFO";
//    if(level == LogLevel::Debug)
//        return "DEBUG";
//    if(level == LogLevel::Critical)
//        return "CRITICAL";
//    if(level == LogLevel::Fatal)
//        return "FATAL";
//    return "";
//}
//
//Logger Logger::operator<(const std::string &string) const
//{
//    Logger log(*this);
//    log.message_.message.append(string);
//    return log;
//}
//
//Logger Logger::operator<(const char *string) const
//{
//    return operator<(std::string(string));
//}
//
//Logger Logger::operator<(const int &num) const
//{
//    return operator<(std::to_string(num));
//}
//
//Logger Logger::operator<(const float &num) const
//{
//    return operator<(std::to_string(num));
//}
//
//
//

#include "logger.hpp"
#include "glad/gl.h"

Log::Target Log::default_target = Target();

Log log()
{
    return { };
}

Log::Log() : level_(LogLevel::Info), target_(default_target)
{

}

Log &Log::operator-(LogLevel level)
{
    level_ = level;
    return *this;
}

Log &Log::operator<(std::string string)
{
    if(!message_.empty() && (!message_.ends_with(' ') || string.starts_with(' '))) string.insert(0, " ");
    return operator<=(string);
}

Log &Log::operator<(const char string[])
{
    return operator<(std::string(string));
}

Log &Log::operator<(int num)
{
    return operator<(std::to_string(num));
}

Log &Log::operator<(float num)
{
    return operator<(std::to_string(num));
}

Log &Log::operator<=(const std::string &string)
{
    message_.append(string);
    return *this;
}

Log &Log::operator<=(const char string[])
{
    return operator<=(std::string(string));
}

Log &Log::operator<=(int num)
{
    return operator<=(std::to_string(num));
}

Log &Log::operator<=(float num)
{
    return operator<=(std::to_string(num));
}

//Log &Log::operator&&(Log::Action &action)
//{
//    actions_.push_back(action);
//    return *this;
//}

Log &Log::operator<<=(const Log::Action &action)
{
    actions_.push_back(action);
    return *this;
}

Log &Log::operator>>=(const Log::Target &target)
{
    target_ = target;
    return *this;
}

Log::~Log()
{
    time_t timer = time(nullptr);
    struct tm *tm = localtime(&timer);
    char buff[26];
    strftime(buff, 26, "%D %T ", tm);
    std::string message = std::string(buff);

    if(level_ == LogLevel::Info)
        message.append("INFO");
    if(level_ == LogLevel::Debug)
        message.append("DEBUG");
    if(level_ == LogLevel::Critical)
        message.append("CRITICAL");
    if(level_ == LogLevel::Fatal)
        message.append("FATAL");

    message.append(" ");
    message.append(this->message_);


    for(auto &action : actions_)
    {
        if(action.code_ == 1)
        {
#ifdef DEBUG
            message.append(to_string(boost::stacktrace::stacktrace()));
#endif
        }
        else if(action.code_ == 2)
        {
            int error;
            while((error = glGetError()) != GL_NO_ERROR)
            {
                message.append(std::to_string(error));
                message.append(", ");
            }
        }
    }
    if(target_.code_ == 1)
    {
        std::cout << message << std::endl;
    }
}