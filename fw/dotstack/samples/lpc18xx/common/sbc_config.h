#ifndef __SBC_CONFIG_H_INCLUDED__
#define __SBC_CONFIG_H_INCLUDED__

#include <stddef.h>

typedef ptrdiff_t ssize_t;

#ifndef EIO
#define EIO      -100
#endif

#ifndef ENOMEM
#  define ENOMEM   -101
#endif

#ifndef ENOSPC
#  define ENOSPC   -102
#endif

#ifndef EINVAL
#	define	EINVAL	1
#endif

#endif // __SBC_CONFIG_H_INCLUDED__
