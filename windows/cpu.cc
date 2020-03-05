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
#include <fstream>
#include <unistd.h> // usleep

#include <thread>

#include "TCHAR.h"
#include "pdh.h"

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

#include "cpu.h"
#include "luts.h"

uint32_t get_cpu_count()
{
  return static_cast< uint32_t >( std::thread::hardware_concurrency() );
}

float cpu_percentage( unsigned cpu_usage_delay )
{
  PdhOpenQuery(NULL, 0, &cpuQuery);
  PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
  PdhCollectQueryData(cpuQuery);

  usleep( cpu_usage_delay );

  PDH_FMT_COUNTERVALUE counterVal;

  PdhCollectQueryData(cpuQuery);
  PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
  return static_cast< float >( counterVal.doubleValue );
}

