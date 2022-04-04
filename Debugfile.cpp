/// @file Debugfile.cpp

#include "Debugfile.h"
#include "Simple_timer.h"

#include <ctime>
#include <thread>

//============================
//Construction and destruction
//============================

// Constructor
// ------------------------------------------------------------------------------------------------
Debugfile::Debugfile(const char *filename, bool open_now)
   : m_filename(filename)
   , m_debug_on(open_now)
   , m_is_open(false)
   , m_newline(false)
   , m_timer(new Simple_timer)
   , m_bugfile()
   , m_padding(9)
{
   if (m_debug_on)
   {
      Open_file();
   }
}

// Destructor
// ------------------------------------------------------------------------------------------------
Debugfile::~Debugfile()
{
   Close_file();

   if (m_timer)
   {
      delete m_timer;
      m_timer = 0;
   }
}

// Turn_on_debug_file
// ------------------------------------------------------------------------------------------------
void Debugfile::Turn_on_debug_file(bool turn_on)
{
   std::lock_guard<std::mutex> file_lock(m_logger_mutex);

   m_debug_on = turn_on;

   if (m_debug_on)
   {
      Open_file();
   }
}

// Open_file
// ------------------------------------------------------------------------------------------------
void Debugfile::Open_file()
{
   if (!m_is_open)
   {
      m_bugfile.open(m_filename, std::ios::out);
      if (m_bugfile.is_open())
      {
         m_is_open = true;
         Write_systemtime();
      }
   }
}

// Close_file
// ------------------------------------------------------------------------------------------------
void Debugfile::Close_file()
{
   if (m_is_open)
   {
      Write("Debug file closed.\n");
      Write_systemtime();
      m_bugfile.flush();
      m_bugfile.close(); // No try/catch - std::streams don't throw by default
   }
}

//================
//Access functions
//================

// Write
// ------------------------------------------------------------------------------------------------
void Debugfile::Write(const char *str, Debugfile::newline_type nl)
{
   std::lock_guard<std::mutex> file_lock(m_logger_mutex);

   if (m_debug_on)
   {
      Write_timestamp();
      Write_thread_ID();
      m_bugfile << str;
      Write_endline(nl);
   }
}


// Write_systemtime
// ------------------------------------------------------------------------------------------------
void Debugfile::Write_systemtime()
{
#pragma warning(disable:4996)

   std::time_t systime = std::time(nullptr);
   m_bugfile << std::asctime(std::localtime(&systime));
   Write_endline(Debugfile::newline_type::e_write_newline);

#pragma warning(default:4996)
}


// Write_endline
// ------------------------------------------------------------------------------------------------
void Debugfile::Write_endline(Debugfile::newline_type newline)
{
   if (newline == Debugfile::newline_type::e_write_newline)
   {
      m_bugfile << std::endl;
      m_newline = true;
   }
}

#define USE_INCREMENTAL

// Write_timestamp
// ------------------------------------------------------------------------------------------------
void Debugfile::Write_timestamp()
{
   if (m_newline)
   {
      std::ios_base::fmtflags old_flags = m_bugfile.setf(std::ios::fixed, std::ios::floatfield);
      std::streamsize old_p = m_bugfile.precision(2);

      m_bugfile << std::right << std::setw(m_padding) << m_timer->Elapsed_ms() << " ";

      m_bugfile.setf(old_flags);
      m_bugfile.precision(old_p); // "precision" doesn't appear to be sticky for all streams

      m_newline = false;
   }

#if defined USE_INCREMENTAL

   m_timer->Reset_timer();

#endif
}

// Write_thread_ID
// ------------------------------------------------------------------------------------------------
void Debugfile::Write_thread_ID()
{
   m_bugfile << "(T" << std::this_thread::get_id() << ") ";
}

// Reset
// ------------------------------------------------------------------------------------------------
void Debugfile::Reset()
{
   Close_file();
   Open_file();
}