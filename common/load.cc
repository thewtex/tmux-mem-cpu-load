/* vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 *
 * Copyright 2012 Matthew McCormick
 * Copyright 2013 Justin Crawford <Justasic@gmail.com>
 * Copyright 2015 Pawel 'l0ner' Soltys
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Based on: github.com/freebsd/freebsd/blob/master/usr.bin/top/machine.c
// Based on: Apple.cpp for load_string/mem_string and apple's documentation

#include <sstream>
#include <string>
#include <stdlib.h> // getloadavg()
#include <cmath> // floorf()
#include <sys/types.h>

#include "cpu.h"
#include "load.h"
#include "luts.h"

// Load Averages
std::string load_string( bool use_colors = false )
{
  std::stringstream ss;
  // Get only 3 load averages
  const int nelem = 3;
  double averages[nelem];
  // based on: opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c

  if( getloadavg( averages, nelem ) < 0 )
  {
    ss << " 0.00 0.00 0.00"; // couldn't get averages.
  }
  else
  {
    if( use_colors )
    {
      unsigned load_percent = static_cast<unsigned int>( averages[0] /
          get_cpu_count() * 0.5f * 100.0f );

      if( load_percent > 100 )
      {
        load_percent = 100;
      }
      ss << load_lut[load_percent];
    }

    ss << ' ';
    for( int i = 0; i < nelem; ++i )
    {
      // Round to nearest, make sure this is only a 0.00 value not a 0.0000
      float avg = floorf( static_cast<float>( averages[i] ) * 100 + 0.5 ) / 100;
      // Don't print trailing whitespace for last element
      if ( i == nelem-1 )
      {
        ss << avg;
      }
      else
      {
        ss << avg << " ";
      }
    }

    if( use_colors )
    {
      ss << "#[fg=default,bg=default]";
    }
  }

  return ss.str();
}

