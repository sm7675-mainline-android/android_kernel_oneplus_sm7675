#ifndef _WAKEUP_SOURCE_REMOVE_COMPAT_H
#define _WAKEUP_SOURCE_REMOVE_COMPAT_H

#ifndef wakeup_source_remove
#define wakeup_source_remove(_ws) wakeup_source_unregister(_ws)
#endif

#endif
