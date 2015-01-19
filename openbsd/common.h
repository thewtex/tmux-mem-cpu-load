/*
 * Copyright 2012 Matthew McCormick
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
 * */
// This file was Authored by Justin Crawford <Justasic@gmail.com>
// Based on: github.com/freebsd/freebsd/blob/master/usr.bin/top/machine.c
// Based on: Apple.cpp for load_string/mem_string and apple's documentation

#ifndef BSD_METER_COMMON_H_
#define BSD_METER_COMMON_H_

#include <iostream>

// Memory Sizes
#define KILOBYTES(x) ((x)/1024)
#define MEGABYTES(x) (KILOBYTES((x))/1024)
#define GIGABYTES(x) (MEGABYTES((x))/1024)

// CPU percentages stuff
#define CP_USER   0
#define CP_NICE   1
#define CP_SYS    2
#define CP_INTR   3
#define CP_IDLE   4
#define CPUSTATES 5

inline void error(const char * error) {
  std::cerr << error << std::endl;
  exit(23);
}

#endif
