/// @file Debug_logger_macros.h

#ifndef DEBUG_LOGGER_MACROS_H_
#define DEBUG_LOGGER_MACROS_H_

#if defined ENABLE_DEBUG_LOGGING

#include "Debugfile.h"
#include <string>
#include <sstream>

#define  G_LOG_DEFINE(path_to_file)    Debugfile g_log(#path_to_file);
#define  G_LOG_EXTERN                  extern Debugfile g_log;
#define  G_LOG_ENABLE                  g_log.Turn_on_debug_file(true);
#define  G_LOG_DISABLE                 g_log.Turn_on_debug_file(false);
#define  G_LOG_VAR(var)                g_log.Write(#var, var);
#define  G_LOG_MSG(msg)                g_log.Write(msg);
#define  G_LOG_MSG_NONL(msg)           g_log.Write(msg, Debugfile::newline_type::e_no_newline);
#define  G_LOG_MSG_VAR(msg, var)       g_log.Write(msg, var);
#define  G_LOG_FUNCTION                std::stringstream ss; \
                                       ss << "Entering into --> " << __FUNCTION__; \
                                       G_LOG_MSG(ss.str().c_str()) \
                                       Logger_helper lh(g_log, __FUNCTION__); 
#define  G_LOG_RESET                   g_log.Reset();
//#define  G_LOG_FUNCTION_RETURN(var)    Logger_helper lh(__FUNCTION__);        // This too

#else

#define  G_LOG_DEFINE
#define  G_LOG_EXTERN
#define  G_LOG_ENABLE 
#define  G_LOG_DISABLE
#define  G_LOG_VAR(var)
#define  G_LOG_MSG(msg)
#define  G_LOG_MSG_NONL(msg)
#define  G_LOG_FUNCTION 
#define  G_LOG_FUNCTION_RETURN(var)
#define  G_LOG_RESET 

#endif // ENABLE_DEBUG_LOGGING

class Logger_helper
{
public:

   // ---------------------------------------------------------------------------------------------
   /// @brief     Default constructor
   // ---------------------------------------------------------------------------------------------
   Logger_helper(Debugfile &logger, const char* fname)
   : m_logger(logger)
   , m_function_name(fname)
   , m_return_variable_value("")
   { 
      m_logger.Modify_indentation(m_func_indent);
   }

   // ---------------------------------------------------------------------------------------------
   /// @brief     Destructor
   // ---------------------------------------------------------------------------------------------
   ~Logger_helper()
   {
      std::stringstream ss;
      ss << "Returning from <-- " << m_function_name;
      m_logger.Write(ss.str().c_str());
      m_logger.Modify_indentation(-m_func_indent);
   }

private:
   // ---------------------------------------------------------------------------------------------
   /// @brief     Copy constructor
   // ---------------------------------------------------------------------------------------------
   Logger_helper(const Logger_helper& c);

   // ---------------------------------------------------------------------------------------------
   /// @brief     Assignment
   // ---------------------------------------------------------------------------------------------
   Logger_helper& operator=(const Logger_helper& c);

   Debugfile&           m_logger;
   std::string          m_function_name;
   std::string          m_return_variable_value;
   const int            m_func_indent{3};
};


#endif // DEBUG_LOGGER_MACROS_H_
