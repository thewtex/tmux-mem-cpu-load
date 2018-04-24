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
#include <fstream>

#include "windows.h"

#include "memory.h"
#include "conversions.h"

void mem_status( MemoryStatus & status )
{
  MEMORYSTATUSEX memory_status;
  memory_status.dwLength = sizeof(memory_status);

  GlobalMemoryStatusEx(&memory_status);

  // we want megabytes on output, but since the values already come as
  // kilobytes we need to divide them by 1024 only once, thus we use
  // KILOBYTES
  status.used_mem = convert_unit(static_cast< float >( memory_status.ullAvailPhys ), MEGABYTES, BYTES);
  status.total_mem = convert_unit(static_cast< float >( memory_status.ullTotalPhys ), MEGABYTES, BYTES);
}

