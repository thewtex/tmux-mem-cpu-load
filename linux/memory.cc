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

#include <sstream>
#include <sys/sysinfo.h>

#include "memory.h"
#include "../luts.h"
#include "../config.h"

std::string mem_string( bool use_colors = false )
{
  std::ostringstream oss;

  struct sysinfo sinfo;
  sysinfo(&sinfo);

  unsigned int total_mem = MEGABYTES(sinfo.totalram);
  unsigned int used_mem = total_mem - MEGABYTES(sinfo.freeram);
  // we don't need this for now
  //unsigned int unused_mem = MEGABYTES(sinfo.freeram);

  if( use_colors )
  {
    oss << mem_lut[(100 * used_mem) / total_mem];
  }

  oss << used_mem << '/' << total_mem << "MB";

  if( use_colors )
  {
    oss << "#[fg=default,bg=default]";
  }

  return oss.str();
}

