#ifndef __CORE_H__
#define __CORE_H__

#include <X11/Xutil.h>
#include <cluwm/core/monitor.h>
#include <cluwm/core/utils.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define RootWindowEventMasks (SubstructureRedirectMask | SubstructureNotifyMask)
#define ButtonMasks          (ButtonPressMask | ButtonReleaseMask)

// hooks are only called on clients which are attached to the workspaces managed
// my the monitor.
typedef void (*EventHandler)(const XEvent *);

typedef enum CursorType {
    CurNormal,
    CurResize,
    CurMove,
    CursorTypeCount
} CursorType;

typedef enum WMAtom {
    WM_PROTOCOLS,
    WM_NAME,
    WM_DELETE_WINDOW,
    WM_TRANSIENT_FOR,
    WM_WINDOW_ROLE,
    _NET_ACTIVE_WINDOW,
    _NET_CLIENT_LIST,
    _NET_WM_BYPASS_COMPOSITOR,
    _NET_WM_NAME,
    _NET_WM_STRUT,
    _NET_WM_STRUT_PARTIAL,
    _NET_WM_WINDOW_TYPE,
    _NET_WM_WINDOW_TYPE_DOCK,
    _NET_WM_WINDOW_TYPE_DIALOG,
    WMAtomCount,
} WMAtom;

// These are mainly the values that don't (shouldn't) change throughout the
// application lifetime.
typedef struct Core {
    bool running;
    Display *dpy;
    Monitor *mon;
    Cursor cursors[CursorTypeCount];
    Atom atoms[WMAtomCount];
    FILE *logger;

    void (*init)(void);
    Window (*input_focused_window)(void);
    Geometry (*get_screen_rect)(void);
    bool (*send_event)(Window, Atom);
    int (*get_window_property)(Window, Atom, int, uint8_t **);
    int (*get_window_title)(Window, XTextProperty *);
    uint32_t (*get_window_list)(Window **);
    void (*stop_running)(void);
} Core;
extern const Core *const core;

#endif
