#ifndef _POTATO_DEBUG_
#define _POTATO_DEBUG_

#ifdef DEBUG
#include <stdio.h>

#define CL_COLOR_NONE "\033[m"
#define CL_COLOR_RED "\033[1;31m"
#define CL_COLOR_WHITE "\033[1;37m"
#define CL_COLOR_YELLOW "\033[1;33m"


#define info(s, args...) \
    fprintf(stderr, \
            CL_COLOR_WHITE "INFO [file: \"" __FILE__ "\". line: %i] " s CL_COLOR_WHITE "\n", \
            __LINE__, ##args)
#define warn(s, args...) \
    fprintf(stderr, \
            CL_COLOR_YELLOW "WARN [file: \"" __FILE__ "\". line: %i] " s CL_COLOR_WHITE "\n", \
            __LINE__, ##args)
#define eror(s, args...) \
    fprintf(stderr, \
            CL_COLOR_RED "EROR [file: \"" __FILE__ "\". line: %i] " s CL_COLOR_WHITE "\n", \
            __LINE__, ##args)
#else
#define info(s, ...) 
#define warn(s, ...)
#define eror(s, ...)
#endif

#endif
