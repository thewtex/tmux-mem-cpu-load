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
#include <string>

// Ugly fix to compilation on OpenBSD 5.6
// these need to be before include sys/mount.h
// and be in this exact order. Otherwise compiling on
// OpenBSD 5.6 will fail with missing NGROUPS define
// or missing gid_t typedefs
#ifdef OPENBSD_WORKAROUND
#include <sys/types.h> // typedefs
#include <sys/param.h> // missing NGROUPS
#include <sys/ucred.h>
#else
#include <sys/types.h>
#endif

#include <sys/mount.h> // VFS_* which we use to get cache
#include <sys/sysctl.h>

#include "error.h"
#include "memory.h"
#include "conversions.h"

void mem_status( MemoryStatus & status )
{
  // These values are in bytes
  int64_t total_mem = 0;
  int64_t used_mem = 0;
  int32_t free_mem = 0;
  size_t size;

  // get page size
  static int hw_pagesize[] = { CTL_HW, HW_PAGESIZE };
  int page_size = 0;
  size = sizeof( page_size );
  if( sysctl( hw_pagesize, 2, &page_size, &size, NULL, 0 ) < 0 )
  {
    error( "memory: error getting page size" );
  }

  // get vm memory stats
  static int uvmexp_mib[] = { CTL_VM, VM_UVMEXP };
  struct uvmexp uvmexp;
  size = sizeof( uvmexp );
  if( sysctl( uvmexp_mib, 2, &uvmexp, &size, NULL, 0 ) < 0 )
  {
    error( "memory: error getting vm memory stats" );
  }

  // In case we need it, this gets the cached memory (bcstats.numbufpages)
  static int vm_bcstats[] = { CTL_VFS, VFS_GENERIC, VFS_BCACHESTAT };
  struct bcachestats bcstats;
  size = sizeof( bcstats );
  if( sysctl( vm_bcstats, 3, &bcstats, &size, NULL, 0 ) < 0 )
  {
    error( "memory: error getting cached memory size" );
  }

  // calculations based on libgtop
  used_mem = ( (uint64_t) uvmexp.npages - uvmexp.free ) << uvmexp.pageshift;
  free_mem = ( (uint64_t) uvmexp.free ) << uvmexp.pageshift;

  // from nagios-memory plugin
  used_mem -= ( (uint64_t) bcstats.numbufpages ) << uvmexp.pageshift;
  free_mem += ( (uint64_t) bcstats.numbufpages ) << uvmexp.pageshift;

  // calculate total memory
  total_mem = ( (uint64_t) uvmexp.npages ) << uvmexp.pageshift;

  status.used_mem = convert_unit(static_cast< float >( used_mem ), MEGABYTES );
  status.total_mem = convert_unit(static_cast< float >( total_mem ), MEGABYTES );
}
