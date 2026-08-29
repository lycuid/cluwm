#ifndef __MISC__MAGNIFY_H__
#define __MISC__MAGNIFY_H__

#include <cluwm/bindings.h>
#include <cluwm/core/client.h>
#include <cluwm/layout.h>

void magnify(Window, const LayoutManager *, const Geometry *, const Geometry *);
void magnify_toggle(const Arg *);

#endif
