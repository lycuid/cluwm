#ifndef __EWMH__DOCKS_H__
#define __EWMH__DOCKS_H__

#include <cluwm/bindings.h>
#include <cluwm/core.h>

void dock_toggle(const Arg *);
void dock_mapnotify(const XEvent *);
void dock_propertynotify(const XEvent *);
void dock_unmapnotify(const XEvent *);
void dock_destroynotify(const XEvent *);

static const EventHandler dock_event_handlers[LASTEvent] = {
    [MapNotify]      = dock_mapnotify,
    [PropertyNotify] = dock_propertynotify,
    [UnmapNotify]    = dock_unmapnotify,
    [DestroyNotify]  = dock_destroynotify,
};

#endif
