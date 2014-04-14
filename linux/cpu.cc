#include <string>
#include <fstream>
#include <unistd.h> // usleep

#include "cpu.h"

float cpu_percentage( unsigned cpu_usage_delay ) {
   std::string line;
   size_t substrStart = 0;
   size_t substrLen;

   // cpu stats
   // user, nice, system, idle
   // in that order
   unsigned long long stats[4];

   std::ifstream stat_file("/proc/stat");
   getline(stat_file, line);
   stat_file.close();

   // skip "cpu"
   substrLen = line.find_first_of(" ", 3);
   // parse cpu line
   for(unsigned i=0; i < 4; i++) {
      substrStart = line.find_first_not_of(" ", substrLen);
      substrLen   = line.find_first_of    (" ", substrStart);
      stats[i] = std::stoll(
            line.substr(substrStart, substrLen));
   }

   usleep( cpu_usage_delay );

   stat_file.open("/proc/stat");
   getline(stat_file, stat_line);
   stat_file.close();

   // skip "cpu"
   substrLen = line.find_first_of(" ", 3);
   // parse cpu line
   for(unsigned i=0; i < 4; i++) {
      substrStart = line.find_first_not_of(" ", substrLen);
      substrLen   = line.find_first_of    (" ", substrStart);
      stats[i] = std::stoll(
            line.substr(substrStart, substrLen)) - stats[i];
   }

   return static_cast<float>( stats[0] + stats[1] + stats[2]) / 
      static_cast<float>( stats[0] + stats[1] + stats[2] + stats[3] ) * 100.0;
}

