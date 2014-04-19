#include <string>
#include <cstring>

#include "graph.h"

char * getGraphByPercentage(unsigned value, unsigned len) {
   unsigned step = 0;
   char * bars = new char[len + 1];

   unsigned barCount = (float(value) / 99.9 * len);

   for(step; step < barCount; step++)
      bars[step] = '|';
   for(step; step < len; step++)
      bars[step] = ' ';
   bars[len]='\0';

   return bars;
}


char * getGraphByValue(unsigned value, unsigned max, unsigned len) {
   unsigned step = 0;
   char * bars = new char[len + 1];

   unsigned barCount = (float(value) / (float(max) - 0.1) * len);

   for(step; step < barCount; step++)
      bars[step] = '|';
   for(step; step < len; step++)
      bars[step] = ' ';
   bars[len]='\0';

   return bars;
}

