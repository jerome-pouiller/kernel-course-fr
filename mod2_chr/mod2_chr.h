/*
 * Licence: GPL
 * Created: 2012-04-02 14:48:46+02:00
 * Main authors:
 *     - Jérôme Pouiller <jezz@sysmic.org>
 */
#ifndef MOD2_CHR_H
# define MOD2_CHR_H

#include <linux/ioctl.h>

struct m_arg {
   int sz;
};

#define FIFO_GET_LEN _IOR('f', 0x00000001, struct m_arg)

#endif /* MOD2_CHR_H */

