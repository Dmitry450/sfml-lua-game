#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class BaseException: public std::exception
{
public:
    BaseException(const std::string &str) throw(): msg(str) {}
    ~BaseException() throw() = default;
    
    virtual const char * what() const throw()
    {
        return msg.c_str();
    }
protected:
    std::string msg;
};

class SerializationError: public BaseException
{
public:
    SerializationError(const std::string &str): BaseException(str) {}
};

#endif
