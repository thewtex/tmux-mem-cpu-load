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
// Pawel Soltys <pwslts@gmail.com> 

//#error ToDo: OpenBSD. This file is incomplete and likely will not compile if 
//you remove this error (it is here to tell you it's unfinished)

#include <sstream>
#include <string>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/vmmeter.h> // vmtotal struct

#include "common.h"
#include "memory.h"
#include "../luts.h"

static int pageshift;

#define pagesh(size) ((size) << pageshift)

std::string mem_string( bool use_colors = false )
{
    std::ostringstream oss;
	
	// These values are in bytes
	int64_t total_mem = 0;
	int64_t used_mem = 0;
    int32_t free_mem = 0;
	size_t size;

	// get page size
	static int hw_pagesize[] = { CTL_HW, HW_PAGESIZE };
	int page_size = 0;
	size = sizeof( page_size );
	if( sysctl( hw_pagesize, 2, &page_size, &size, NULL, 0) < 0)
	{
	  error("memory: error getting page size");
	  exit(23);
	}
	
	// calculate how far we must shift the variables
	pageshift = 0;
	while( page_size > 1 )
	{
	  pageshift++;
	  page_size >>= 1;
	}

	// get vm memory stats
	static int vm_totalmem[] = { CTL_VM, VM_METER };
	struct vmtotal vm_total;
	size = sizeof( vm_total );
	if( sysctl( vm_totalmem, 2, &vm_total, &size, NULL, 0 ) < 0 )
	{
	  error("memory: error getting vm memory stats");
	  exit(23);
	}
  
	// In case we need it, this gets the cached memory (bcstats.numbufpages)
	static int vm_bcstats[] = { CTL_VFS, VFS_GENERIC, VFS_BCACHESTAT };
	struct bcachestats bcstats;
	size = sizeof( bcstats );
	if( sysctl( vm_bcstats, 3, &bcstats, &size, NULL, 0 ) < 0 )
	{
	  error( "memory: error getting cached memory size" );
	  exit( 23 );
	}

	// calculations based on conky openbsd port
	used_mem = pagesh( vm_total.t_rm );
	free_mem = pagesh( vm_total.t_free );

	// from nagios-memory plugin
	used_mem -= pagesh( bcstats.numbufpages );
	free_mem += pagesh( bcstats.numbufpages );
	
	// calculate total memory
	total_mem = used_mem + free_mem;
    
	if( use_colors )
	{
	  oss << mem_lut[( 100 * used_mem ) / total_mem];
	}

    oss << convert_unit( used_mem, MEGABYTES ) 
	  << '/' << convert_unit( total_mem, MEGABYTES ) << "MB";

	if( use_colors )
	{
	  oss << "#[fg=default,bg=default]";
	}

    return oss.str();
}
