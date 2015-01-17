#include <sstream>
#include <sys/sysinfo.h>

#include "memory.h"
#include "../luts.h"

std::string mem_string( bool use_colors = false ) {
   std::ostringstream oss;

   struct sysinfo sinfo;
   sysinfo(&sinfo);

   unsigned int total_mem = sinfo.totalram / 1014;
   unsigned int used_mem = total_mem - sinfo.freeram / 1024;
   // we don't need this for now
   //unsigned int unused_mem = sinfo.freeram / 1024;
   
   if( use_colors ) {
      oss << mem_lut[(100 * used_mem) / total_mem];
   }
   
   oss << used_mem / 1024 << '/' << total_mem / 1024 << "MB";
   
   if( use_colors ) {
      oss << "#[fg=default,bg=default]";
   }

   return oss.str();
}

