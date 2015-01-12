#include <string>
#include <sstream>
#include <fstream>
#include <cmath> // floorf()

#include <unistd.h>
#include <stdlib.h> // getloadavg()

#include "load.h"
#include "../luts.h"

std::string load_string( bool use_colors = false ) {
   std::ostringstream oss;

   // Both apple and BSD style systems have these api calls
   // Only get 3 load averages
   int nelem = 3;
   double averages[3];
   // based on: 
   // http://www.opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c
   if( getloadavg(averages, nelem) < 0 )
      oss << "0.00 0.00 0.00"; // couldn't get averages.
   else
      for(int i = 0; i < nelem; ++i) {
         // Round to nearest, make sure this is 
         // only a 0.00 value not a 0.0000
         float avg = floorf(
               static_cast<float>(averages[i]) * 100 + 0.5) / 100;
         oss << avg << " ";
      }

   std::string load_line( oss.str() );
   oss.str( "" );

   if( use_colors ) {
      // Likely does not work on BSD, but not tested
      unsigned number_of_cpus = sysconf( _SC_NPROCESSORS_ONLN );

      std::istringstream iss( load_line.substr( 0, 4 ) );
      float recent_load;
      iss >> recent_load;
      // colors range from zero to twice the number of 
      // cpu's for the most recent load metric
      
      unsigned load_percent = static_cast< unsigned int >( 
            recent_load / number_of_cpus * 0.5f * 100.0f );
      
      if( load_percent > 100 )
         load_percent = 100;
      
      oss << load_lut[load_percent];
   }

   oss << load_line.substr( 0, 14 );
   
   if( use_colors )
      oss << "#[fg=default,bg=default]";

   return oss.str();
}

