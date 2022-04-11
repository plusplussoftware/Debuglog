/// @file Simple_timer.h

#ifndef SIMPLE_TIMER_H_
#define SIMPLE_TIMER_H_

#include <chrono>
#include <iostream>

class Simple_timer
{
public:

   Simple_timer();

   Simple_timer(const Simple_timer& r);

   const Simple_timer& operator=(const Simple_timer& r);

   //virtual
   ~Simple_timer();

   // ---------------------------------------------------------------------------------------------
   // Reset_timer
   // ---------------------------------------------------------------------------------------------
   /// @brief
   // ---------------------------------------------------------------------------------------------
   void        Reset_timer();

   // ---------------------------------------------------------------------------------------------
   // Elapsed_ms
   // ---------------------------------------------------------------------------------------------
   /// @brief
   // ---------------------------------------------------------------------------------------------
   double      Elapsed_ms() const;

   // ---------------------------------------------------------------------------------------------
   // Add_delay_ms
   // ---------------------------------------------------------------------------------------------
   /// @brief     
   // ---------------------------------------------------------------------------------------------
   void        Add_delay_ms(double milliseconds) const;

   // ---------------------------------------------------------------------------------------------
   /// @brief
   // ---------------------------------------------------------------------------------------------
   double      Elapsed_us() const;

   // ---------------------------------------------------------------------------------------------
   /// @brief     
   // ---------------------------------------------------------------------------------------------
   void        Add_delay_us(double microseconds) const;

   // ---------------------------------------------------------------------------------------------
   // printClockData
   // ---------------------------------------------------------------------------------------------
   /// @brief     From C++ Standard Library, The: A Tutorial and Reference, 2nd Edition 
   // ---------------------------------------------------------------------------------------------
   template <typename C>
   void printClockData ()
   {
       using namespace std;

       cout << "- precision: ";
       // if time unit is less or equal one millisecond
       typedef typename C::period P;// type of time unit
       if (ratio_less_equal<P,milli>::value) {
          // convert to and print as milliseconds
          typedef typename ratio_multiply<P,kilo>::type TT;
          cout << fixed << double(TT::num)/TT::den
               << " milliseconds" << endl;
       }
       else {
           // print as seconds
           cout << fixed << double(P::num)/P::den << " seconds" << endl;
       }
       cout << "- is_steady: " << boolalpha << C::is_steady << endl;
   }

private:

   std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

};

#endif // SIMPLE_TIMER_H_
