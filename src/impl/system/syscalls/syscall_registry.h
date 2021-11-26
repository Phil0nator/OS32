#ifndef OS32_SYSCALL_REGISTRY
#define OS32_SYSCALL_REGISTRY

typedef int (*syscall_routine_t) (int a, int b, int c, int d, int e, int f);

extern syscall_routine_t __os32_syscall_registry[256];




#endif