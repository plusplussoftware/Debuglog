/// @file Simple_timer.cpp

#include "Simple_timer.h"

#pragma warning (disable:4996)

// Constructor
// ------------------------------------------------------------------------------------------------
Simple_timer::Simple_timer()
   : m_start(std::chrono::high_resolution_clock::now())
{
}

// Destructor
// ------------------------------------------------------------------------------------------------
Simple_timer::~Simple_timer()
{
}

// Copy constructor
// ------------------------------------------------------------------------------------------------
Simple_timer::Simple_timer(const Simple_timer &r)
   : m_start(std::chrono::high_resolution_clock::now())
{
}

// Assignment op
// ------------------------------------------------------------------------------------------------
const Simple_timer& Simple_timer::operator =(const Simple_timer& r)
{
   m_start = r.m_start;

   return *this;
}

// Reser_timer
// ------------------------------------------------------------------------------------------------
void Simple_timer::Reset_timer()
{
   m_start = std::chrono::high_resolution_clock::now();
}

// ------------------------------------------------------------------------------------------------
double Simple_timer::Elapsed_ms() const
{
   using namespace std::chrono;

   duration<double, std::milli> elapsed_ms = high_resolution_clock::now() - m_start;

   return elapsed_ms.count();
}

// ------------------------------------------------------------------------------------------------
void Simple_timer::Add_delay_ms(double milliseconds) const
{
   using namespace std::chrono;

   time_point<high_resolution_clock> now = high_resolution_clock::now();

   while (duration_cast<std::chrono::milliseconds>(now - m_start).count() < milliseconds)
   {
      now = high_resolution_clock::now();
   }
}

// ------------------------------------------------------------------------------------------------
double Simple_timer::Elapsed_us() const
{
   using namespace std::chrono;

   duration<double, std::micro> elapsed_us = high_resolution_clock::now() - m_start;

   return elapsed_us.count();
}

// ------------------------------------------------------------------------------------------------
void Simple_timer::Add_delay_us(double microseconds) const
{
   using namespace std::chrono;

   time_point<high_resolution_clock> now = high_resolution_clock::now();

   while (duration_cast<std::chrono::microseconds>(now - m_start).count() < microseconds)
   {
      now = high_resolution_clock::now();
   }
}

#pragma warning(default:4996)
