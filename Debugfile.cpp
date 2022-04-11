/// @file Debugfile.cpp

#include "Debugfile.h"
#include "Simple_timer.h"

#include <ctime>
#include <thread>

// ------------------------------------------------------------------------------------------------
Debugfile::Debugfile(const char *filename, bool open_now, Debugfile::timing_type t_unit)
   : m_filename(filename)
   , m_debug_on(open_now)
   , m_is_open(false)
   , m_newline(false)
   , m_timer(new Simple_timer)
   , m_bugfile()
   , m_padding(12)
   , m_indent(1)
   , m_timing_unit(t_unit)
{
   if (m_debug_on)
   {
      Open_file();
   }
}

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
         Write_header();
         Write_endline(Debugfile::newline_type::e_write_newline);
      }
   }
}

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

// ------------------------------------------------------------------------------------------------
void Debugfile::Write(const char *str, Debugfile::newline_type nl)
{
   std::lock_guard<std::mutex> file_lock(m_logger_mutex);

   if (m_debug_on)
   {
      if (m_newline)
      {
         Write_timestamp();
         Write_thread_ID();
         m_bugfile << std::right << std::setw(static_cast<std::streamsize>(m_indent)) << ' ';
      }
      m_bugfile << str;
      Write_endline(nl);
   }
}

void Debugfile::Write(const char *description, const void* value, 
                      newline_type nl)
{
   std::lock_guard<std::mutex> file_lock(m_logger_mutex);

   if (m_debug_on)
   {
      if (m_newline)
      {
         Write_timestamp();
         Write_thread_ID();
         m_bugfile << std::right << std::setw(static_cast<std::streamsize>(m_indent)) << ' ';
      }
      m_bugfile << " " << description << " " << value;
      Write_endline(nl);
   }
}

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
   else
   {
      m_newline = false;
   }
}

#define USE_INCREMENTAL

// ------------------------------------------------------------------------------------------------
void Debugfile::Write_timestamp()
{
   std::ios_base::fmtflags old_flags = m_bugfile.setf(std::ios::fixed, std::ios::floatfield);
   std::streamsize old_p = m_bugfile.precision(2);

   m_bugfile << std::right << std::setw(m_padding) << 
      ((m_timing_unit == timing_type::e_milli) ? m_timer->Elapsed_ms() : m_timer->Elapsed_us());

   m_bugfile.setf(old_flags);
   m_bugfile.precision(old_p); // "precision" doesn't appear to be sticky for all streams

#if defined USE_INCREMENTAL

   m_timer->Reset_timer();

#endif
}

// ------------------------------------------------------------------------------------------------
void Debugfile::Write_header()
{
   m_bugfile << std::right << std::setw(m_padding) << 
                              ((m_timing_unit == timing_type::e_milli) ? 
                                 "Elapsed_ms" : "Elapsed_us") <<
                              std::setw(m_padding) << "Thread_ID" <<
                              std::right << std::setw(static_cast<std::streamsize>(m_indent)) << ' ' <<
                              std::left << "Log_message";
   m_newline = false;

#if defined USE_INCREMENTAL

   m_timer->Reset_timer();

#endif
}

// ------------------------------------------------------------------------------------------------
void Debugfile::Write_thread_ID()
{
   m_bugfile << std::right << std::setw(m_padding) << std::this_thread::get_id();
}

// ------------------------------------------------------------------------------------------------
void Debugfile::Reset()
{
   Close_file();
   Open_file();
}

// ------------------------------------------------------------------------------------------------
void Debugfile::Modify_indentation(const int spaces)
{
   m_indent += spaces;
   if (m_indent < 1) 
      m_indent = 1;
}