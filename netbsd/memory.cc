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

#include <sys/types.h>
#include <sys/sysctl.h>
#include <uvm/uvm_extern.h> // uvmexp struct

#include "error.h"
#include "conversions.h"
#include "memory.h"

void mem_status( MemoryStatus & status )
{
  // get vm memory stats
  static int vm_totalmem[] = { CTL_VM, VM_UVMEXP2 };
  struct uvmexp_sysctl mem;
  size_t size = sizeof( mem );
  if( sysctl( vm_totalmem, 2, &mem, &size, NULL, 0 ) < 0 )
  {
    error( "memory: error getting vm memory stats" );
  }

  int64_t total_mem = ( mem.npages << mem.pageshift );
  int64_t used_mem =
    ( mem.active + mem.wired - mem.filepages ) << mem.pageshift;

  // add 1 to used which gets lost somewhere along conversions
  status.used_mem = convert_unit(static_cast< float >( used_mem ), MEGABYTES );
  status.total_mem = convert_unit(static_cast< float >( total_mem ), MEGABYTES );
}
