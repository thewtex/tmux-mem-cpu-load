#ifndef CPU_H_
#define CPU_H_

#include <sys/types.h>

#define CP_USER   0
#define CP_NICE   1
#define CP_SYS    2
#define CP_INTR   3
#define CP_IDLE   4
#define CPUSTATES 5

float cpu_percentage( unsigned );
uint8_t get_cpu_count();

#endif
