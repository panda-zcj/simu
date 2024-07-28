/**
 * @author: pikaqiu_zcj
 * @date: 2024-07-14
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-11
 * @file: \src\log\log.cpp
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "log.h"
using namespace simu;

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

    void nanoLogInit(std::string logFileName, std::string logDirectory, nanolog::LogLevel logLevel, uint32_t log_file_roll_size_mb)
    {
        // Ensure initialize is called once prior to logging.
        // This will create log files like /tmp/nanolog1.txt, /tmp/nanolog2.txt etc.
        // Log will roll to the next file after every 1MB.
        // This will initialize the guaranteed logger.
        // nanolog::initialize(nanolog::GuaranteedLogger(), "/tmp/", "nanolog", 1);
        initialize(nanolog::GuaranteedLogger(), logDirectory, logFileName, log_file_roll_size_mb);

        // Or if you want to use the non guaranteed logger -
        // ring_buffer_size_mb - LogLines are pushed into a mpsc ring buffer whose size
        // is determined by this parameter. Since each LogLine is 256 bytes,
        // ring_buffer_size = ring_buffer_size_mb * 1024 * 1024 / 256
        // In this example ring_buffer_size_mb = 3.
        // nanolog::initialize(nanolog::NonGuaranteedLogger(3), "/tmp/", "nanolog", 1);

        // Change log level at run-time.
        nanolog::set_log_level(logLevel);

        LOG_DEBUG << "This log line will be logged since we are at loglevel = DEBUG";
        LOG_INFO << "This log line will be logged since we are at loglevel = INFO";
        LOG_WARN << "This log line will be logged since we are at loglevel = WARN";
        LOG_CRIT << "This log line will be logged since we are at loglevel = CRIT";
    }

#ifdef __cplusplus
}
#endif //__cplusplus