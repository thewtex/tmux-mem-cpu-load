/*
 * Copyright 2012 Matthew McCormick
 * Copyright 2013 Justin Crawford <Justasic@gmail.com>
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

#include <string>
#include <sstream>
#include <mach/mach.h>
#include <sys/sysctl.h> // for sysctl

#include "memory.h"
#include "conversions.h"

void mem_status( MemoryStatus & status )
{
  // These values are in bytes
  u_int64_t total_mem;
  // float app_mem = 0.0f;
  // float cached_file_mem = 0.0f;
  float available_mem = 0.0f;
  float percentage_mem;
  float free_mem;
  float free_mem_in_gigabytes; // used to check if free mem < 1 GB
  //u_int64_t unused_mem;

  vm_size_t page_size;
  vm_statistics64_data_t vm_stats; // Use 64-bit VM statistics

  // Get total physical memory
  int mib[] = { CTL_HW, HW_MEMSIZE };
  size_t length = sizeof( total_mem );
  sysctl( mib, 2, &total_mem, &length, NULL, 0 );

  mach_port_t mach_port = mach_host_self();

  // Use HOST_VM_INFO64_COUNT as the count for vm_statistics64_data_t
  mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

  if( KERN_SUCCESS == host_page_size( mach_port, &page_size ) &&
      KERN_SUCCESS == host_statistics64( mach_port, HOST_VM_INFO64,
        ( host_info_t )&vm_stats, &count )
    )
  {
    //unused_mem = static_cast<u_int64_t>( vm_stats.free_count * page_size );
    // app_mem = static_cast<float>(( vm_stats.internal_page_count - vm_stats.purgeable_count ) * page_size );
    // cached_file_mem = static_cast<float>(( vm_stats.purgeable_count + vm_stats.external_page_count ) * page_size );
    available_mem = static_cast<float>(( vm_stats.free_count + vm_stats.external_page_count - vm_stats.speculative_count ) * page_size );
  }

  status.used_mem = convert_unit(static_cast< float >(total_mem - available_mem ), MEGABYTES );
  status.total_mem = convert_unit(static_cast< float >( total_mem ), MEGABYTES );
}
