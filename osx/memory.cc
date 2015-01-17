#include <string>
#include <sstream>
#include <mach/mach.h>
#include <sys/sysctl.h> // for sysctl

#include "memory.h"
#include "../luts.h"

std::string mem_string( bool use_colors ) {
   std::ostringstream oss;
   
   // These values are in bytes
   int64_t total_mem;
   int64_t used_mem;
   int64_t unused_mem;
   
   vm_size_t page_size;
   mach_port_t mach_port;
   mach_msg_type_number_t count;
   vm_statistics_data_t vm_stats;

   // Get total physical memory
   int mib[2];
   mib[0] = CTL_HW;
   mib[1] = HW_MEMSIZE;
   size_t length = sizeof(int64_t);
   sysctl(mib, 2, &total_mem, &length, NULL, 0);

   mach_port = mach_host_self();
   count = sizeof(vm_stats) / sizeof(natural_t);
   if (KERN_SUCCESS == host_page_size( mach_port, &page_size) &&
       KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO,
          (host_info_t)&vm_stats, &count)) {
      unused_mem = (int64_t)vm_stats.free_count * (int64_t)page_size;

      used_mem = (
            (int64_t)vm_stats.active_count +
            (int64_t)vm_stats.inactive_count +
            (int64_t)vm_stats.wire_count) * 
         (int64_t)page_size;
   }

   // To kilobytes
   used_mem /= 1024;
   total_mem /= 1024;

   if( use_colors )
      oss << mem_lut[(100 * used_mem) / total_mem];
   
   oss << used_mem / 1024 << '/' << total_mem / 1024 << "MB";
   
   if( use_colors )
      oss << "#[fg=default,bg=default]";

   return oss.str();
}
