#include <string>
#include <sstream>
#include <unistd.h> // sysconf()?
#include <sys/sysinfo.h>
#include <linux/kernel.h> // SI_LOAD_SHIFT

#include "load.h"
#include "../luts.h"

std::string load_string( bool use_colors = false ) {
   std::ostringstream oss;

   float f = static_cast<float>(1 << SI_LOAD_SHIFT);

   struct sysinfo sinfo;
   sysinfo(&sinfo);

   if( use_colors ) {
      // Likely does not work on BSD, but not tested
      unsigned number_of_cpus = sysconf( _SC_NPROCESSORS_ONLN );

      float recent_load = sinfo.loads[0] / f;

      // colors range from zero to twice the number of cpu's 
      // for the most recent load metric
      unsigned load_percent = static_cast< unsigned int >( 
            recent_load / number_of_cpus * 0.5f * 100.0f );
      
      if( load_percent > 100 )
         load_percent = 100;
      
      oss << load_lut[load_percent];
   }
  
   // set precision so we get results like "0.22"
   oss.setf( std::ios::fixed );
   oss.precision(2);
   
   oss << sinfo.loads[0] / f << " " << sinfo.load[1] / f << " " 
	 << sinfo.load[2] / f;
   
   if( use_colors )
      oss << "#[fg=default,bg=default]";

   return oss.str();
}
