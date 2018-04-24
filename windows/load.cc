/* vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 *
 * Copyright 2012 Matthew McCormick
 * Copyright 2013 Justin Crawford <Justasic@gmail.com>
 * Copyright 2015 Pawel 'l0ner' Soltys
 * Copyright 2016 Compilenix
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
#include <cmath> // floorf()
#include <sys/types.h>

#include "cpu.h"
#include "load.h"
#include "luts.h"

#include "powerline.h"

// Load Averages
std::string load_string( bool use_colors,
  bool use_powerline_left, bool use_powerline_right,
  short num_averages )
{
  // Not implemented
  std::ostringstream ss;
  ss << (char) 0;
  return ss.str();
}
