/// @file Debug_logger_macros.h

#ifndef DEBUG_LOGGER_MACROS_H_
#define DEBUG_LOGGER_MACROS_H_

#if defined ENABLE_DEBUG_LOGGING

#include "Debugfile.h"

#define  G_LOG_DEFINE(path_to_file)    Debugfile g_log(#path_to_file);
#define  G_LOG_EXTERN                  extern Debugfile g_log;
#define  G_LOG_ENABLE                  g_log.Turn_on_debug_file(true);
#define  G_LOG_DISABLE                 g_log.Turn_on_debug_file(false);
#define  G_LOG_WRITE_VAR(var)          g_log.Write(#var, var);
#define  G_LOG_WRITE_STR(msg)          g_log.Write(msg);
#define  G_LOG_FUNCTION                Logger_helper lh(__FUNCTION__);        // Put at absolute beginning of a function
#define  G_LOG_RESET                   g_log.Reset();
//#define  G_LOG_FUNCTION_RETURN(var)    Logger_helper lh(__FUNCTION__);        // This too

#else

#define  G_LOG_DEFINE
#define  G_LOG_EXTERN
#define  G_LOG_ENABLE 
#define  G_LOG_DISABLE
#define  G_LOG_WRITE_VAR(var)
#define  G_LOG_WRITE_STR(msg)
#define  G_LOG_FUNCTION 
#define  G_LOG_FUNCTION_RETURN(var)
#define  G_LOG_RESET 

#endif // ENABLE_DEBUG_LOGGING

#include <string>
#include <sstream>

class Logger_helper
{
public:

   // ---------------------------------------------------------------------------------------------
   /// @brief     Default constructor
   // ---------------------------------------------------------------------------------------------
   Logger_helper(const char* fname)
   : m_function_name(fname)
   , m_return_variable_value("")
   { 
      G_LOG_EXTERN
      std::stringstream ss;
      ss << "Entering  into --> " << m_function_name;
      G_LOG_WRITE_STR(ss.str().c_str())
   }

   //// ---------------------------------------------------------------------------------------------
   ///// @brief     Variable-logging constructor
   //// ---------------------------------------------------------------------------------------------
   //Logger_helper(const char* fname, const char* retval)
   //: m_function_name(fname)
   //, m_return_variable_value(retval)
   //{ 
   //   G_LOG_EXTERN
   //   std::stringstream ss;
   //   ss << "Entering  into --> " << m_function_name;
   //   G_LOG_WRITE_STR(ss.str().c_str())
   //}

   // ---------------------------------------------------------------------------------------------
   /// @brief     Destructor
   // ---------------------------------------------------------------------------------------------
   ~Logger_helper()
   {
      G_LOG_EXTERN
      std::stringstream ss;
      ss << "Returning from <-- " << m_function_name;
      G_LOG_WRITE_STR(ss.str().c_str())
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

   std::string          m_function_name;
   std::string          m_return_variable_value;
};


#endif // DEBUG_LOGGER_MACROS_H_
