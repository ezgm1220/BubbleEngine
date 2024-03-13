# 1. QuickStart

```
#include "spdlog/spdlog.h"
int main()
{
    //Use the default logger (stdout, multi-threaded, colored)
    spdlog::info("Hello, {}!", "World");
}
```

spdlog is a header only library. Just copy the files under [include](https://github.com/gabime/spdlog/tree/master/include/spdlog) to your build tree and use a C++11 compiler.

It provides a python like formatting API using the bundled [fmt](https://github.com/fmtlib/fmt) lib (see the [reference](https://fmt.dev/latest/syntax.html)):

```
logger->info("Hello {} {} !!", "param1", 123.4);
```



spdlog 采用的是 "包含所需功能 "的方法--您的代码应包含实际需要的功能。

例如，如果只需要轮转记录器，则需要包含 "**spdlog/sinks/rotating_file_sink.h**"。

另一个例子是加入 "**spdlog/async.h**"以获得异步日志功能。

## Basic example

```
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging

int main(int, char* [])
{
    try 
    {
        // Create basic file logger (not rotated)
        auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
        
        // 创建文件轮转记录器，最大容量为 5 MB，轮转 3 个文件
        auto file_logger = spdlog::rotating_logger_mt("file_logger", "myfilename", 1024 * 1024 * 5, 3);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```



## 使用工厂方法创建异步日志记录器



```C++
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/async.h" //支持同步日志记录。
#include "spdlog/sinks/basic_file_sink.h"

int main(int, char* [])
{
    try
    {        
        auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");
        for (int i = 1; i < 101; ++i)
        {
            async_file->info("Async message #{}", i);
        }
        // 在 VisualStudio 下，必须在主程序结束前调用该函数，以解决已知的 VS 问题
        spdlog::drop_all(); 
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```



## 创建异步日志记录器并更改线程池设置



```C++
#include "spdlog/async.h" //support for async logging
#include "spdlog/sinks/daily_file_sink.h"
int main(int, char* [])
{
    try
    {                                        
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logfile", 23, 59);
        // 默认线程池设置可在*创建异步日志记录器之前修改：
        spdlog::init_thread_pool(10000, 1); // queue with 10K items and 1 backing thread.
        auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");       
        spdlog::drop_all(); 
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```

## 创建共享同一文件（接收器）的多个记录器，即类别

```C++
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
int main(int, char* [])
{
    try
    {
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logfile", 23, 59);
        // create synchronous  loggers
        auto net_logger = std::make_shared<spdlog::logger>("net", daily_sink);
        auto hw_logger  = std::make_shared<spdlog::logger>("hw",  daily_sink);
        auto db_logger  = std::make_shared<spdlog::logger>("db",  daily_sink);      

        net_logger->set_level(spdlog::level::critical); // independent levels
        hw_logger->set_level(spdlog::level::debug);
         
        // 全局注册日志记录器，以便使用 spdlog::get(logger_name) 访问它们
        spdlog::register_logger(net_logger);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```



## 创建具有多个汇的日志记录器，每个汇有自己的格式和日志级别



```C++
//
// Logger with console and file output.
// the console will show only warnings or worse, while the file will log all messages.
// 
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h" // or "../stdout_sinks.h" if no colors needed
#include "spdlog/sinks/basic_file_sink.h"
int main(int, char* [])
{
    try
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);
        console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
        file_sink->set_level(spdlog::level::trace);

        spdlog::sinks_init_list sink_list = { file_sink, console_sink };

        spdlog::logger logger("multi_sink", sink_list.begin(), sink_list.end());
        logger.set_level(spdlog::level::debug);
        logger.warn("this should appear in both console and file");
        logger.info("this message should not appear in the console, only in the file");

        // or you can even set multi_sink logger as default logger
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink})));

    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```



#### Logging macros



You can define SPDLOG_ACTIVE_LEVEL to the desired log level *before* including "spdlog.h".

This will turn on/off logging statements at compile time

```
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

spdlog::set_level(spdlog::level::debug); // or spdlog::set_level(spdlog::level::trace); 

SPDLOG_LOGGER_TRACE(file_logger , "Some trace message that will not be evaluated.{} ,{}", 1, 3.23);
SPDLOG_LOGGER_DEBUG(file_logger , "Some Debug message that will be evaluated.. {} ,{}", 1, 3.23);
SPDLOG_DEBUG("Some debug message to default logger that will be evaluated");
```



Notice that `spdlog::set_level` is also necessary to print out debug or trace messages.

#### Log user-defined objects



```
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h" // must be included
#include "spdlog/sinks/stdout_sinks.h"

class some_class {};
std::ostream& operator<<(std::ostream& os, const some_class& c)
{ 
    return os << "some_class"; 
}

void custom_class_example()
{
    some_class c;
    auto console = spdlog::stdout_logger_mt("console");
    console->info("custom class with operator<<: {}..", c);
}
```



or use the bundled `fmt` lib when advanced formatting is needed:

```C++
#include <iterator>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h" // must be included
#include "spdlog/sinks/stdout_sinks.h"

class some_class {
    int code;
};

template<typename OStream>
friend OStream &operator<<(OStream &os, const some_class& to_log)
{
    fmt::format_to(std::ostream_iterator<char>(os), "{:04X}", to_log.code);
    return os;
}

void custom_class_example()
{
    some_class c; c.code = 17;
    auto console = spdlog::stdout_logger_mt("console");
    console->info("custom class with operator<< using fmt: {}..", c);
}
```