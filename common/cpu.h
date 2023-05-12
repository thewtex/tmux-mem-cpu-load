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

#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <sys/types.h>

#if defined(__APPLE__) && defined(__MACH__)
  #define CP_USER 0
  #define CP_SYS  1
  #define CP_IDLE 2
  #define CP_NICE 3
  #define CP_STATES 4
#elif defined(__OpenBSD__)
  #include <sys/sched.h>
  #define CP_STATES CPUSTATES
#else
  #define CP_USER   0
  #define CP_NICE   1
  #define CP_SYS    2

  #if defined(__FreeBSD__) || defined(__NetBSD__)
    // *BSD or OSX
    #define CP_INTR   3
    #define CP_IDLE   4
    #define CP_STATES 5
  #else
    //linux
    #define CP_IDLE 3
    #define CP_STATES 4
  #endif
#endif

float cpu_percentage( unsigned );
uint32_t get_cpu_count();

/** CPU percentage output mode.
 *
 * Examples:
 *
 * CPU_MODE_DEFAULT: 100%
 * CPU_MODE_THREADS: 800% (8 cores, fully loaded)
 */
enum CPU_MODE
{
  CPU_MODE_DEFAULT,
  CPU_MODE_THREADS
};

#endif
