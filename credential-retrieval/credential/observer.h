#ifndef FP_OBSERVER_H
#define FP_OBSERVER_H

#include <mach/mach.h>
#include <stdint.h>
#include <sys/types.h>

typedef int (*SelectorReturnCallback)(mach_port_t task, void *context);

int observe_fairplay_requests(pid_t pid, uint32_t selector, SelectorReturnCallback callback, void *context, unsigned seconds);

#endif
