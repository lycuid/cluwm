#include "ewmh.h"
#include <X11/Xatom.h>
#include <cluwm/core.h>
#include <cluwm/layout.h>
#include <config.h>
#include <stdint.h>

void ewmh_maprequest(const XEvent *);
void ewmh_propertynotify(const XEvent *);
void ewmh_destroynotify(const XEvent *);
void ewmh_focusin(const XEvent *);
void ewmh_focusout(const XEvent *);
void ewmh_clientadd(Client *);
void ewmh_clientremove(Client *);

const EventHandler ewmh_event_handlers[LASTEvent] = {
    [MapRequest]     = ewmh_maprequest,
    [PropertyNotify] = ewmh_propertynotify,
    [DestroyNotify]  = ewmh_destroynotify,
    [FocusIn]        = ewmh_focusin,
    [FocusOut]       = ewmh_focusout,
};

const ClientHook ewmh_client_hooks[HookTypeCount] = {
    [ClientAdd]    = ewmh_clientadd,
    [ClientRemove] = ewmh_clientremove,
};

static inline void update_client_list(Window removed)
{
    XDeleteProperty(core->dpy, DefaultRootWindow(core->dpy),
                    core->atoms[_NET_CLIENT_LIST]);
    FOREACH_ALLOCATED_CLIENT(const Client *c)
    {
        if (c->window != removed)
            XChangeProperty(core->dpy, DefaultRootWindow(core->dpy),
                            core->atoms[_NET_CLIENT_LIST], XA_WINDOW, 32,
                            PropModeAppend, (uint8_t *)&c->window, 1);
    }
}

static inline void handle_bypassed(Client *c)
{
    if (!c)
        return;
    Monitor *mon    = core->mon;
    uint32_t *value = NULL;
    core->get_window_property(c->window, core->atoms[_NET_WM_BYPASS_COMPOSITOR],
                              sizeof(uint32_t), (uint8_t **)&value);
    Workspace *ws = mon_get_client_ws(mon, c);
    if (ws) {
        if (value && *value == 1) {
            SET(c->state, ClBypassed);
        } else {
            UNSET(c->state, ClBypassed);
        }
        mon_applylayout(mon);
    }
    if (value)
        XFree(value);
}

static inline void handle_dialog(Client *c)
{
    if (!c)
        return;
    Atom *atom = NULL;
    core->get_window_property(c->window, core->atoms[_NET_WM_WINDOW_TYPE], 1,
                              (uint8_t **)&atom);
    if (!atom)
        return;
    if (*atom == core->atoms[_NET_WM_WINDOW_TYPE_DIALOG])
        SET(c->state, ClFloating);
    XFree(atom);
}

void ewmh_maprequest(const XEvent *xevent)
{
    const XMapRequestEvent *e = &xevent->xmaprequest;
    XWindowAttributes attrs;
    XGetWindowAttributes(core->dpy, e->window, &attrs);
    if (!IS_SET(attrs.your_event_mask, FocusChangeMask))
        XSelectInput(core->dpy, e->window,
                     attrs.your_event_mask | FocusChangeMask);
    update_client_list(0);
}

void ewmh_clientadd(Client *c)
{
    handle_dialog(c);
    handle_bypassed(c);
}

void ewmh_propertynotify(const XEvent *xevent)
{
    const XPropertyEvent *e = &xevent->xproperty;
    Monitor *mon            = core->mon;
    if (e->atom != core->atoms[_NET_WM_BYPASS_COMPOSITOR])
        return;
    Client *c = NULL;
    ITER(workspaces)
    {
        if ((c = ws_getclient(mon_get_workspace_at(mon, it), e->window)))
            break;
    }
    handle_bypassed(c);
}

void ewmh_destroynotify(const XEvent *xevent)
{
    update_client_list(xevent->xdestroywindow.window);
}

void ewmh_clientremove(Client *c) { update_client_list(c ? c->window : 0); }

void ewmh_focusin(const XEvent *xevent)
{
    const XFocusInEvent *e = &xevent->xfocus;
    Monitor *mon           = core->mon;
    Client *c;
    if (!(c = ws_getclient(curr_ws(mon), e->window)))
        return;
    XChangeProperty(core->dpy, DefaultRootWindow(core->dpy),
                    core->atoms[_NET_ACTIVE_WINDOW], XA_WINDOW, 32,
                    PropModeReplace, (uint8_t *)&c->window, 1);
}

void ewmh_focusout(const XEvent *xevent)
{
    (void)xevent;
    XDeleteProperty(core->dpy, DefaultRootWindow(core->dpy),
                    core->atoms[_NET_ACTIVE_WINDOW]);
}
