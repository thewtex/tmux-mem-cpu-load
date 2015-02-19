/*
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

#include <string>
#include <sstream>
#include <fstream>
#include <cmath> // floorf()

#include <unistd.h>
#include <stdlib.h> // getloadavg()

#include "load.h"
#include "cpu.h"
#include "luts.h"

std::string load_string( bool use_colors = false )
{
  std::ostringstream oss;

  // Both apple and BSD style systems have these api calls
  // Only get 3 load averages
  const int nelem = 3;
  double averages[nelem];
  // based on: 
  // http://www.opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c
  if( getloadavg( averages, nelem ) < 0 )
  {
    oss << "0.00 0.00 0.00"; // couldn't get averages.
  }
  else
  {
    for( int i = 0; i < nelem; ++i )
    {
      // Round to nearest, make sure this is 
      // only a 0.00 value not a 0.0000
      float avg = floorf( static_cast<float>( averages[i] ) * 100 + 0.5 ) / 100;
      oss << avg << " ";
    }
  }

  std::string load_line( oss.str() );
  oss.str( "" );

  if( use_colors )
  {
    std::istringstream iss( load_line.substr( 0, 4 ) );
    float recent_load;
    iss >> recent_load;
    // colors range from zero to twice the number of 
    // cpu's for the most recent load metric

    unsigned load_percent = static_cast< unsigned int >( 
        recent_load / get_cpu_count() * 0.5f * 100.0f );

    if( load_percent > 100 )
    {
      load_percent = 100;
    }

    oss << load_lut[load_percent];
  }

  oss << load_line.substr( 0, 14 );

  if( use_colors )
  {
    oss << "#[fg=default,bg=default]";
  }

  return oss.str();
}

