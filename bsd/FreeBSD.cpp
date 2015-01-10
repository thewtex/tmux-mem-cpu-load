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
// Based on: https://github.com/freebsd/freebsd/blob/master/usr.bin/top/machine.c
// Based on: Apple.cpp for load_string/mem_string and apple's documentation


#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cerrno>
#include <string>
#include <stdlib.h>
#include <cmath>
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
static inline void getsysctl(const char *name, void *ptr, size_t len)
{
    size_t nlen = len;

    if (sysctlbyname(name, ptr, &nlen, NULL, 0) == -1)
    {
        fprintf(stderr, "sysctl(%s...) failed: %s\n", name, strerror(errno));
        exit(23);
    }

    if (nlen != len)
    {
        fprintf(stderr, "sysctl(%s...) expected %lu bytes, got %lu bytes\n", name, (unsigned long)len, (unsigned long)nlen);
        //exit(23);
    }
}

float cpu_percentage(unsigned int cpu_usage_delay)
{
    int64_t load1[CPUSTATES];
    int64_t load2[CPUSTATES];
    
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

    return static_cast<float>(diff_user + diff_system + diff_nice)/static_cast<float>(diff_user + diff_system + diff_nice + diff_idle)*100.0;
}

std::string mem_string()
{
	// These values are in bytes
	int64_t total_mem = 0;
	int64_t used_mem = 0;
	int64_t unused_mem = 0;
    int32_t inactive_mem = 0;
    int32_t active_mem = 0;
    int32_t free_mem = 0;
    int32_t wired_mem = 0;
    int32_t page_size = 0;
    int32_t cache_mem = 0;
    std::ostringstream oss;

	// Get total physical memory, page size, and some other needed info
    GETSYSCTL("hw.realmem", total_mem);
    GETSYSCTL("hw.pagesize", page_size);

    GETSYSCTL("vm.stats.vm.v_free_count", free_mem);
    GETSYSCTL("vm.stats.vm.v_inactive_count", inactive_mem);
    GETSYSCTL("vm.stats.vm.v_cache_count", cache_mem);
    GETSYSCTL("vm.stats.vm.v_wire_count", wired_mem);
    GETSYSCTL("vm.stats.vm.v_active_count", active_mem);

    // Get all memory which can be allocated
    //unused_mem = (cache_mem + free_mem) * page_size;
    used_mem   = ((int64_t)active_mem + (int64_t)inactive_mem + (int64_t)wired_mem) * (int64_t)page_size;

    oss << MEGABYTES(used_mem) << '/' << MEGABYTES(total_mem) << "MB";

    return oss.str();
}

// Load Averages
std::string load_string()
{
	std::stringstream ss;
	// Only get 3 load averages
	int nelem = 3;
	double averages[3];
	// based on: http://www.opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c
	if(getloadavg(averages, nelem) < 0)
		ss << "0.00 0.00 0.00"; // couldn't get averages.
	else
	{
		for(int i = 0; i < nelem; ++i)
		{
			// Round to nearest, make sure this is only a 0.00 value not a 0.0000
			float avg = floorf(static_cast<float>(averages[i]) * 100 + 0.5) / 100;
			ss << avg << " ";
		}
	}

	return ss.str();
}

