/** 
 * @author: pikaqiu_zcj
 * @date: 2024-06-29
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-01
 * @file: \include\log.h
 * @message: 
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved. 
 */

#ifndef __LOG_H__
#define __LOG_H__

#include "global.h"
#include "NanoLog/NanoLog.hpp"
#include <string>

namespace simu
{
#ifdef __cplusplus
    extern "C"
    {
#endif //__cplusplus

        /**
         * @brief Initialize the logger
         * @param[in] logFileName   name of the log file
         * @param[in] logDirectory  path of the log file
         * @param[in] logLevel      level of the log file
         *      @enum nanoLog::LogLevel::INFO
         *      @enum nanoLog::LogLevel::WARN
         *      @enum nanoLog::LogLevel::CRIT
         * @param[in] log_file_roll_size_mb The maximum value of each log file
         */
        void nanoLogInit(std::string logFileName = LOG_FILENAME, std::string logDirectory = LOG_DIRECTORY, nanolog::LogLevel logLevel = LOG_LEVEL, uint32_t log_file_roll_size_mb = LOG_FILE_ROLL_SIZE_MB);

#ifdef __cplusplus
    }
#endif //__cplusplus

} // namespace Log

#endif // __LOG_H__