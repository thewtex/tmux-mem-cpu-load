#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include "../luts.h"

std::string load_string( bool use_colors = false ) {
   std::ostringstream oss;

   std::ifstream loadavg_file( "/proc/loadavg" );
   std::string load_line;
   std::getline( loadavg_file, load_line );
   loadavg_file.close();

   if( use_colors ) {
      // Likely does not work on BSD, but not tested
      unsigned number_of_cpus = sysconf( _SC_NPROCESSORS_ONLN );

      float recent_load = stof(load_line.substr( 0, 4 ));

      // colors range from zero to twice the number of cpu's 
      // for the most recent load metric
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
