#include <string>
#include <cstring>

#include "spark.h"

char getSparkByPrecentage(unsigned value) {
   char ticks = " ▁▂▃▄▅▆▇█";

   unsigned tickVal = (float(value) / 9);

   return ticks[tickVal];
}


char getSparkByValue(unsigned value, unsigned max) {
   char ticks = " ▁▂▃▄▅▆▇█";

   unsigned tickVal = (float(value) / float(max) * 9);

   return ticks[tickVal];
}

