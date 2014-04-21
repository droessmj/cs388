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
    
    if (event == EVT_CREATE){
	s = "CREATE";
    }else if (event == EVT_MODIFY){
	s = "MODIFY"; 
    }else if (event == EVT_DELETE){
    	s = "DELETE";
    }else if (event == EVT_UNKNOWN){
	s = "UNKNOWN EVENT";
    }else{
	s = "ERROR Parsing Event Type";
    }

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

    if (strcmp(s, "CREATE") == 0){
	type  = EVT_CREATE;
    }else if (strcmp(s, "MODIFY") == 0){
	type  = EVT_MODIFY;
    }else if (strcmp(s, "DELETE") == 0){
	type  = EVT_DELETE;
    }else if (strcmp(s, "UNKNOWN") == 0){
	type  = EVT_UNKNOWN;
    }else{
	fprintf(stderr, "Unknown event type passed\n");
	exit(EXIT_FAILURE);
    }
	return (type);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
