/// @file Debugfile.h

#ifndef DEBUGFILE_H_

#define DEBUGFILE_H_

#include <fstream>
#include <cstdint>
#include <iomanip>
#include <mutex>
#include <vector>

class Simple_timer;

class Debugfile
{
public:

   enum class newline_type : bool
   {
      e_write_newline = true,
      e_no_newline    = false
   };

   enum class timing_type : int
   {
      e_milli,
      e_micro
   };

   explicit Debugfile(const char *filename, 
                      bool open_now = true, 
                      Debugfile::timing_type t_unit = Debugfile::timing_type::e_micro);

   ~Debugfile();

   // External utility and access functions

   // ---------------------------------------------------------------------------------------------
   /// @brief     Allows or disallows writing.
   /// @author    Tanaya Mankad 11/20/02
   // ---------------------------------------------------------------------------------------------
   void Turn_on_debug_file(bool turn_on);

   // ---------------------------------------------------------------------------------------------
   /// @brief     Write a description and value to file, with timestamp and optional newline.
   /// @author    Tanaya Mankad 11/06/02, 11/30/03
   // ---------------------------------------------------------------------------------------------
   template <typename T>
   void Write(const char *description, const T& value, 
              newline_type nl = newline_type::e_write_newline)
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
         m_bugfile << description << " " << value;
         Write_endline(nl);
      }
   }

   template <>
   void Write<bool>(const char *description, const bool& value, 
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
         m_bugfile << " " << description << " " << (value ? "true" : "false");
         Write_endline(nl);
      }
   }

   // ---------------------------------------------------------------------------------------------
   /// @brief     Template overload to write a description and vector to file.
   /// @author    Tanaya Mankad 10/28/2015
   // ---------------------------------------------------------------------------------------------
   template <typename T>
   void Write(const char *description, const std::vector<T>& vec, 
              newline_type nl = newline_type::e_write_newline)
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
         m_bugfile << description << " = ";
         for (auto i = vec.begin(); i != vec.end(); ++i)
         {
            m_bugfile << *i;
            if (i < vec.end() - 1)
               m_bugfile << ", ";
         }
         Write_endline(nl);
      }
   }

   // ---------------------------------------------------------------------------------------------
   /// @brief     Write a pointer to file
   /// @author    Tanaya Mankad
   // ---------------------------------------------------------------------------------------------
   void Write(const char *description, const void* value, newline_type nl);

   // ---------------------------------------------------------------------------------------------
   /// @brief     Write a user-supplied string to file, with timestamp and optional newline.
   /// @author    Tanaya Mankad 11/06/02, 11/30/03
   // ---------------------------------------------------------------------------------------------
   void Write(const char *str, newline_type nl = newline_type::e_write_newline);

   // ---------------------------------------------------------------------------------------------
   /// @return    @e true if debugging is turned on, @e false otherwise
   /// @author    Tanaya Mankad 11/06/02
   // ---------------------------------------------------------------------------------------------
   bool Is_activated() const { return m_debug_on; }

   // ---------------------------------------------------------------------------------------------
   /// @brief     Close the existing stream and re-open for writing (not appending)
   /// @author    Tanaya Mankad 01/07/16
   // ---------------------------------------------------------------------------------------------
   void Reset();

   // ---------------------------------------------------------------------------------------------
   /// @brief     Indent or de-indent the log message
   /// @author    Tanaya Mankad 04/07/22
   // ---------------------------------------------------------------------------------------------
   void Modify_indentation(const int spaces);

private:

   // Internal utility functions

   // ---------------------------------------------------------------------------------------------
   /// @brief     Checks if file is already open, or opens for writing. fstream::open() will
   ///            only fail if the path does not exist (file not found), or if the object
   ///            m_bugfile is already attached to a stream.
   /// @author    Tanaya Mankad 11/06/02
   // ---------------------------------------------------------------------------------------------
   void Open_file();

   // ---------------------------------------------------------------------------------------------
   /// @brief     Encapsulates the stream flush and close.
   /// @author    Tanaya Mankad 01/07/16
   // ---------------------------------------------------------------------------------------------
   void Close_file();

   // ---------------------------------------------------------------------------------------------
   /// @brief     Writes a current date and time to file.
   /// @author    Tanaya Mankad 11/06/02
   // ---------------------------------------------------------------------------------------------
   void Write_systemtime();

   // ---------------------------------------------------------------------------------------------
   /// @brief     Writes a new line character and flushes the character buffer using std::endl) If 
   ///            there is no end of line, the next item written will not have a time stamp.
   /// @author    Tanaya Mankad 11/13/02
   // ---------------------------------------------------------------------------------------------
   void Write_endline(newline_type newline);

   // ---------------------------------------------------------------------------------------------
   /// @brief     Writes the time in milliseconds to the file.
   /// @author    Tanaya Mankad 11/06/02
   // ---------------------------------------------------------------------------------------------
   void Write_timestamp();

   // ---------------------------------------------------------------------------------------------
   /// @brief     Writes the time in milliseconds to the file.
   /// @author    Tanaya Mankad 11/06/02
   // ---------------------------------------------------------------------------------------------
   void Write_header();

   void Write_thread_ID();

private:

   std::string       m_filename;
   std::ofstream     m_bugfile;
   bool              m_debug_on;
   bool              m_is_open;
   bool              m_newline;
   Simple_timer      *m_timer;
   std::mutex        m_logger_mutex;
   const int         m_padding;
   int               m_indent;
   timing_type       m_timing_unit;
};

// ================================================================================================
// From:
// http://stackoverflow.com/questions/24161402/how-to-replace-some-characters-from-stringstream-in-c
// ================================================================================================
class streampad : public std::streambuf
{
public:
    streampad(std::ostream& os, int padding = 4) : m_sbuf(os.rdbuf()), m_pad(padding)
    {
    }

    int_type overflow(int_type c) override
    {
        return m_sbuf->sputc(c == '\n' ? traits_type::eof() : c);
    }

    int sync() override { return m_sbuf->pubsync() ? 0 : -1; }

private:
    std::streambuf* m_sbuf;
    int m_pad;
};
#endif // DEBUGFILE_H_
