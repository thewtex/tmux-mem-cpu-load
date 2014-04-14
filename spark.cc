#include <string>
#include <cstring>

#include "spark.h"

char getSparkByPrecentage(unsigned value) {
   char ticks = " ▁▂▃▄▅▆▇█9";

   unsigned tickVal = (float(value) / 10);

   return ticks[tickVal];
}


char getSparkByValue(unsigned value, unsigned max) {
   char ticks = " ▁▂▃▄▅▆▇█9";

   unsigned tickVal = (float(value) / float(max) * 10);

   return ticks[tickVal];
}

