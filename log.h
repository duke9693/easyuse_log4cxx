#pragma once

/*
*      Author: fancongchong
*      mail:   jarvis0707@outlook.com
*/
#include <stdarg.h>
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/xml/domconfigurator.h"
#include "log4cxx/helpers/exception.h"

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;
using namespace log4cxx::spi;

#ifndef _FCC_LOG4CXX_NOT_SET_DEFAULT_LOCALE
#include <locale.h>
#ifdef _WIN32 || _WIN64
static char* log4cxx_locale = setlocale(LC_ALL, "zh_CN.GB2312");//set default locale,避免Unicode中文乱码
#else
static char* log4cxx_locale = setlocale(LC_ALL, "UTF-8");//set default locale,避免Unicode中文乱码
#endif

enum LOG_LEAVEL
{
    LOGGER_DEBUG = 0,
    LOGGER_INFO,
    LOGGER_WARN,
    LOGGER_ERROR,
    LOGGER_FATAL
};

//1 获得日志配置文件的LOGGER。。
//MAX_LOGGER_INFO("hello world,i hava %d girlfriend",1);
static LoggerPtr logger(Logger::getLogger("MAXWELL"));

static bool MAX_LOGGER(LOG_LEAVEL enleavel, char* pLogger, ...)
{
    int nSize = strlen(pLogger) + 100;
    char* pBuffer = new(std::nothrow) char[nSize];
    if (pBuffer == nullptr)
        return false;
    va_list args;
    va_start(args, pLogger);
    vsprintf(pBuffer, pLogger, args);
    va_end(args);
    switch (enleavel)
    {
    case LOGGER_DEBUG:
        LOG4CXX_DEBUG(logger, pBuffer);
        break;
    case LOGGER_INFO:
        LOG4CXX_INFO(logger, pBuffer);
        break;
    case LOGGER_WARN:
        LOG4CXX_WARN(logger, pBuffer);
        break;
    case LOGGER_ERROR:
        LOG4CXX_ERROR(logger, pBuffer);
        break;
    case LOGGER_FATAL:
        LOG4CXX_FATAL(logger, pBuffer);
        break;
    default:
        delete[] pBuffer;
        return false;
    }
    delete[] pBuffer;
    return true;
}

#define MAX_LOGGER_DEBUG(format, ...) MAX_LOGGER(LOGGER_DEBUG,"<%d>\t<%s>,"format"", __LINE__,__FUNCTION__,##__VA_ARGS__)
#define MAX_LOGGER_INFO(format, ...) MAX_LOGGER(LOGGER_INFO,"<%d>\t<%s> "format"", __LINE__,__FUNCTION__,##__VA_ARGS__)
#define MAX_LOGGER_WARN(format, ...) MAX_LOGGER(LOGGER_WARN,"<%d>\t<%s> "format"", __LINE__,__FUNCTION__,##__VA_ARGS__)
#define MAX_LOGGER_ERROR(format, ...) MAX_LOGGER(LOGGER_ERROR,"<%d>\t<%s> "format"", __LINE__,__FUNCTION__,##__VA_ARGS__)
#define MAX_LOGGER_FATAL(format, ...) MAX_LOGGER(LOGGER_FATAL,"<%d>\t<%s> "format"", __LINE__,__FUNCTION__,##__VA_ARGS__)

/************************************************************************
使用说明：

//Logger::getLogger函数，对于相同名字返回的都是同一个对象指针
//日志名字最好使用.分隔，以体现层次关系，可以针对不同层次设置不同的输出级别

//Windows平台Log4cxx底层使用UNICODE存储日志字符串，因此Windows平台输出字符串最好统一为UNICODE，
//以减少字符集转换，使用UNICODE效率比使用ANSI可以提高一倍。
//单条日志最大支持120M

LOG4CXX_TRACE(logger, _T("trace msg"));
LOG4CXX_DEBUG(logger, _T("debug msg"));
LOG4CXX_INFO(logger, _T("info msg"));
LOG4CXX_WARN(logger, _T("warn msg"));
LOG4CXX_ERROR(logger, _T("error msg"));
LOG4CXX_FATAL(logger, _T("fatal msg"))

初始化时，log4cxx默认会读取当前目录下的log4cxx.xml、log4cxx.properties或者log4j.xml、log4j.properties
等配置文件。上层也可以调用PropertyConfigurator::configure("otherfile.properties");
或者DOMConfigurator::configure("otherfile.xml");设置自定义的配置文件名称。
一般使用默认配置文件就可以了，便于统一管理。

输出模式语法，与Log4J完全一致：以%开头
c：日志名称 For example,for the logger name "a.b.c" the pattern %c{2} will output "b.c".
d：时间 For example, d{HH:mm:ss,SSS}, d{dd MMM yyyy HH:mm:ss,SSS} or d{DATE}.
F： 文件名
l：位置信息
L：行号
m：具体消息
n：换行
p：级别
r：应用启动以来的毫秒数
t：线程ID
x：线程相关信息（NDC）
X:线程MDC信息
%: %%代表%

下面是格式修饰符：
Format modifier/left justify/minimum width/maximum width/comment
%20c         false     20     none    输出小于20时，左边补空格
%-20c         true     20     none     输出小于20时，右边补空格
%.30c         NA         none 30        如果输出大于30，从右边截取30字节
%20.30c     false     20     30        不足20左边补空格，最大30
%-20.30c     true     20     30        不足20右边补空格，最大30

//例子
#ifdef _DEBUG
#pragma comment(lib,"log4cxx_15D.lib")
#else
#pragma comment(lib,"log4cxx_15U.lib")
#endif
int main()
{
PropertyConfigurator::configure("logxxTest.cfg.txt");
LoggerPtr logger1 = Logger::getLogger("TraceYourMama");
LOG4CXX_INFO(logger1, "跟踪");  //unicode 可以不用写_T("")
//或者这么用
MAX_LOGGER_DEBUG("我有%d颗瓜子,%d颗给你，我自己留%d", 5, 3, 2);

return 1;
}
************************************************************************/

