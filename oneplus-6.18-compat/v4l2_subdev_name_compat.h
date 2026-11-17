#ifndef V4L2_SUBDEV_NAME_COMPAT_H
#define V4L2_SUBDEV_NAME_COMPAT_H

#include <media/v4l2-subdev.h>

#ifndef V4L2_SUBDEV_NAME_SIZE
#define V4L2_SUBDEV_NAME_SIZE sizeof(((struct v4l2_subdev *)0)->name)
#endif

#endif
