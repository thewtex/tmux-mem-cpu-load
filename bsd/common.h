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
#include <cerrno>
#include <sys/sysctl.h>
#include <sys/types.h>

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

#define GETSYSCTL(name, var) getsysctl(name, &(var), sizeof(var))
static inline void getsysctl(const char *name, void *ptr, size_t len) {
  size_t nlen = len;

  if (sysctlbyname(name, ptr, &nlen, NULL, 0) == -1) {
	std::cerr << "sysctl(" << name << "...) failed: " << strerror(errno)
	  << std::endl;
	exit(23);
  }

  if (nlen != len) {
	std::cerr << "sysctl(" << name << "...) expected " 
	  << static_cast<unsigned long>(len) << " bytes, got "
	  << static_cast<unsigned long>(nlen) << " bytes\n";
	//exit(23);
  }
}

#endif
