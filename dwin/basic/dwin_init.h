#ifndef __DWIN_INIT_H_
#define __DWIN_INIT_H_

#include "dwin_def.h"

/* dwin���ʼ��״̬ */
enum dwin_init_flag
{
    dwin_uninit = 0,
    dwin_inited
};

/* �ⲿ���ýӿ� */
uint8_t dwin_init(const char *name, uint32_t baudrate);

#endif
