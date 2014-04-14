#include <mach/vm_statistics.h>
#include <mach/processor_info.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/host_info.h>
#include <mach/mach_error.h>
#include <mach/vm_map.h>
#include <mach/mach.h>
#include <sys/sysctl.h> // for sysctl
#include <sys/types.h> // for integer types

// OSX or BSD based system, use BSD APIs instead
// See: http://www.opensource.apple.com/source/xnu/xnu-201/osfmk/mach/host_info.h
// and: http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/
//
host_cpu_load_info_data_t _get_cpu_percentage() {
   kern_return_t              error;
   mach_msg_type_number_t     count;
   host_cpu_load_info_data_t  r_load;
   mach_port_t                mach_port;

   count     = HOST_CPU_LOAD_INFO_COUNT;
   mach_port = mach_host_self();
   error     = host_statistics(mach_port, HOST_CPU_LOAD_INFO, (
            host_info_t)&r_load, &count);
   
   if (error != KERN_SUCCESS)
      return host_cpu_load_info_data_t();

   return r_load;
}

float cpu_percentage( unsigned int cpu_usage_delay ) {
   // Get the load times from the XNU kernel
   host_cpu_load_info_data_t load1 = _get_cpu_percentage();
   usleep(cpu_usage_delay);
   host_cpu_load_info_data_t load2 = _get_cpu_percentage();

   // Current load times
   unsigned long long current_user    = load1.cpu_ticks[CPU_STATE_USER];
   unsigned long long current_system  = load1.cpu_ticks[CPU_STATE_SYSTEM];
   unsigned long long current_nice    = load1.cpu_ticks[CPU_STATE_NICE];
   unsigned long long current_idle    = load1.cpu_ticks[CPU_STATE_IDLE];
   // Next load times
   unsigned long long next_user       = load2.cpu_ticks[CPU_STATE_USER];
   unsigned long long next_system     = load2.cpu_ticks[CPU_STATE_SYSTEM];
   unsigned long long next_nice       = load2.cpu_ticks[CPU_STATE_NICE];
   unsigned long long next_idle       = load2.cpu_ticks[CPU_STATE_IDLE];
   // Difference between the two
   unsigned long long diff_user       = next_user - current_user;
   unsigned long long diff_system     = next_system - current_system;
   unsigned long long diff_nice       = next_nice - current_nice;
   unsigned long long diff_idle       = next_idle - current_idle;

   return static_cast<float>(diff_user + diff_system + diff_nice) / 
      static_cast<float>(
            diff_user + diff_system + diff_nice + diff_idle) * 100.0;
}

