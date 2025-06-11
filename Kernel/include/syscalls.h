#ifndef __SYSCALLS__H_
#define __SYSCALLS__H_

#include <stdint.h>

void _syscallHandler(void);

int64_t write(int64_t fd, const char *buf, int64_t count);// write video
int64_t read(int64_t fd, char *buf, int64_t count);       // read

#endif /* __SYSCALLS__H_ */

