#ifndef OS32_SYS_WAIT
#define OS32_SYS_WAIT
#include "types.h"
#include "time.h"

#define WNOHANG 1
#define WUNTRACED 2
#define WCONTINUED 4

pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);

#endif