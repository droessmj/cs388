/* events.c: rorschach events */

#include "rorschach.h"

/* Event */

/**
 * Return string corresponding to event type.
 *
 * Example:
 *
 *  event_string(EVT_CREATE) -> "CREATE"
 */
const char *
event_string(event_t event)
{
    const char *s;
    
    /* TODO */

    return (s);
}

/**
 * Return event type corresponding to string.
 *
 * Example:
 *
 *  event_type("CREATE") -> EVT_CREATE
 */
event_t
event_type(const char *s)
{
    event_t type;

    /* TODO */

    return (type);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
