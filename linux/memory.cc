#include <string>
#include <ostream>

#include "memory.h"

std::string mem_string( bool use_colors = false ) {
   std::ostringstream oss;

   unsigned int total_mem;
   unsigned int used_mem;
   unsigned int unused_mem;
   
   size_t substrStart;
   size_t substrLen;

   std::ifstream meminfo_file( "/proc/meminfo" );

   std::string mem_line;

   getline( meminfo_file, mem_line );
   substrStart = mem_line.find_first_of( ':' ) + 1;
   substrLen   = mem_line.find_first_of( 'k' );
   total_mem = stoi(mem_line.substr(substrStart, substrLen);

   used_mem = total_mem;

   for( unsigned int i = 0; i < 3; i++ ) {
      getline( meminfo_file, mem_line );
      substrStart = mem_line.find_first_of( ':' ) + 1;
      substrLen   = mem_line.find_first_of( 'k' );
      unused_mem = stoi(mem_line.substr(substrStart, substrLen);
      used_mem -= unused_mem;
   }

   meminfo_file.close();

   if( use_colors ) {
      oss << mem_lut[(100 * used_mem) / total_mem];
   }
   
   oss << used_mem / 1024 << '/' << total_mem / 1024 << "MB";
   
   if( use_colors ) {
      oss << "#[fg=default,bg=default]";
   }

   return oss.str();
}

