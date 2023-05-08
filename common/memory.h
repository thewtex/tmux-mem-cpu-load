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

#ifndef MEMORY_H_
#define MEMORY_H_

#include <string>

/** Memory status in megabytes */
struct MemoryStatus
{
  float used_mem;
  float total_mem;
};

/** Get the current memory status */
void mem_status( MemoryStatus & status );


/** Memory status string output mode.
 *
 * Examples:
 *
 * MEMORY_MODE_DEFAULT:          11156/16003MB
 * MEMORY_MODE_FREE_MEMORY:
 * MEMORY_MODE_USAGE_PERCENTAGE: 
 */
enum MEMORY_MODE
{
  MEMORY_MODE_DEFAULT,
  MEMORY_MODE_FREE_MEMORY,
  MEMORY_MODE_USAGE_PERCENTAGE
};

std::string mem_string( const MemoryStatus & mem_status,
  MEMORY_MODE mode = MEMORY_MODE_DEFAULT,
  bool use_colors = false,
  bool use_powerline_left = false,
  bool use_powerline_right = false,
  bool segments_to_left = false,
  short left_color = 0 );

#endif
