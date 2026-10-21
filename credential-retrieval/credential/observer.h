#ifndef FP_OBSERVER_H
#define FP_OBSERVER_H

#include <mach/mach.h>
#include <sys/types.h>

typedef int (*SelectorReturnCallback)(mach_port_t task, void *context);

int observe_selector21(pid_t pid, SelectorReturnCallback callback, void *context, unsigned seconds);

#endif
