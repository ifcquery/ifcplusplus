#ifndef SOWIN_DEBUG
#error The define SOWIN_DEBUG needs to be defined to true or false
#endif

#if SOWIN_DEBUG
#include "config-debug.h"
#else /* !SOWIN_DEBUG */
#include "config-release.h"
#endif /* !SOWIN_DEBUG */
