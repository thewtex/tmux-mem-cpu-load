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

#include <sys/types.h>
#include <unistd.h> // usleep

#include "common.h"
#include "cpu.h"

float cpu_percentage(unsigned int cpu_usage_delay)
{
    int32_t load1[CPUSTATES];
    int32_t load2[CPUSTATES];
    
    GETSYSCTL("kern.cp_time", load1);
    usleep(cpu_usage_delay);
    GETSYSCTL("kern.cp_time", load2);

    // Current load times
    unsigned long long current_user = load1[CP_USER];
    unsigned long long current_system = load1[CP_SYS];
    unsigned long long current_nice = load1[CP_NICE];
    unsigned long long current_idle = load1[CP_IDLE];
    // Next load times
    unsigned long long next_user = load2[CP_USER];
    unsigned long long next_system = load2[CP_SYS];
    unsigned long long next_nice = load2[CP_NICE];
    unsigned long long next_idle = load2[CP_IDLE];
    // Difference between the two
    unsigned long long diff_user = next_user - current_user;
    unsigned long long diff_system = next_system - current_system;
    unsigned long long diff_nice = next_nice - current_nice;
    unsigned long long diff_idle = next_idle - current_idle;

    return static_cast<float>(diff_user + diff_system + diff_nice) / 
	  static_cast<float>(diff_user + diff_system + diff_nice + diff_idle) * 
	  100.0;
}
