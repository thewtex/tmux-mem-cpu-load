/* vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 *
 * Copyright 2012 Matthew McCormick
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
#include <sys/sysinfo.h>
#include <linux/kernel.h> // SI_LOAD_SHIFT

#include "load.h"
#include "cpu.h"
#include "luts.h"

std::string load_string( bool use_colors = false )
{
  std::ostringstream oss;

  float f = static_cast<float>( 1 << SI_LOAD_SHIFT );

  struct sysinfo sinfo;
  sysinfo( &sinfo );

  if( use_colors )
  {
    // Likely does not work on BSD, but not tested
    float recent_load = sinfo.loads[0] / f;

    // colors range from zero to twice the number of cpu's 
    // for the most recent load metric
    unsigned load_percent = static_cast< unsigned int >( 
        recent_load / get_cpu_count() * 0.5f * 100.0f );

    if( load_percent > 100 )
    {
      load_percent = 100;
    }

    oss << load_lut[load_percent];
  }

  // set precision so we get results like "0.22"
  oss.setf( std::ios::fixed );
  oss.precision( 2 );

  oss << sinfo.loads[0] / f << " " << sinfo.loads[1] / f << " " 
    << sinfo.loads[2] / f;

  if( use_colors )
  {
    oss << "#[fg=default,bg=default]";
  }

  return oss.str();
}
