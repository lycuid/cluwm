#ifndef __EWMH__DOCKS_H__
#define __EWMH__DOCKS_H__

#include <cluwm/bindings.h>
#include <cluwm/core.h>

void dock_toggle(const Arg *);

extern const EventHandler dock_event_handlers[LASTEvent];

#endif
