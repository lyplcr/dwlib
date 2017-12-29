#ifndef __DWIN_DEF_H_ 
#define __DWIN_DEF_H_ 

#include <stdbool.h>
#include <rtthread.h>
#include <rtdevice.h>

/* ��ӡӦ�ò���Ϣ = 1, ��ӡ�ײ���Ϣ = 2 */
#define PKG_DWIN_DEBUG 1

/* dwin version information */
#define DWIN_MAJOR_VER      1L          /* major version number */
#define DWIN_MINOR_VER      4L          /* minor version number */
#define DWIN_REVISE_VER     1L          /* revise version number */
#define DWIN_VERSION        ((DWIN_MAJOR_VER*10000) + (DWIN_MINOR_VER*100) + DWIN_REVISE_VER)

/* dwin config */
/* transport framehead high byte */
#ifndef PKG_DWIN_HEAD_H
#define PKG_DWIN_HEAD_H (0x5A)
#endif

/* transport framehead low byte */
#ifndef PKG_DWIN_HEAD_L
#define PKG_DWIN_HEAD_L (0xA5)
#endif

#ifndef PKG_DWIN_VAR_MAX_BYTE
#define PKG_DWIN_VAR_MAX_BYTE (4096)
#endif

/* dwin prompt */
#ifndef PKG_DWIN_PROMPT
#define PKG_DWIN_PROMPT "[dwin]"
#endif

/* dwin watch thread priority */
#ifndef PKG_DWIN_WATCH_PRIO
#define PKG_DWIN_WATCH_PRIO (10)
#endif

/* dwin space name length */
#ifndef PKG_DWIN_SPACE_NAME_LEN
#define PKG_DWIN_SPACE_NAME_LEN (12)
#endif

/* ��������ֵ */
#ifndef PKG_DWIN_BUTTON_PRESS_VALUE
#define PKG_DWIN_BUTTON_PRESS_VALUE (0x0D0D)
#endif

/* dwin macro */
/* read and write reg/var cmd */
#define DWIN_REG_READ   (0x81) 
#define DWIN_REG_WRITE  (0x80) 
#define DWIN_VAR_READ   (0x83) 
#define DWIN_VAR_WRITE  (0x82) 

/* dwin debug */
#ifndef PKG_DWIN_DEBUG
#define PKG_DWIN_DEBUG 0
#endif
#if (PKG_DWIN_DEBUG == 0)
#define dwin_print(...) 
#define dwin_println(...) 
#else
#define dwin_print(...)             \
{                                   \
    rt_kprintf(__VA_ARGS__);        \
}
#define dwin_println(...)           \
{                                   \
    rt_kprintf(PKG_DWIN_PROMPT);    \
    rt_kprintf(__VA_ARGS__);        \
    rt_kprintf("\n");               \
}
#endif

#define BITx(n) (1 << n) 
#define BITS_SET(data, bits)  ((data) |=  (bits))  
#define BITS_CLR(data, bits)  ((data) &= ~(bits))  
#define BITS_TGL(data, bits)  ((data) ^=  (bits))  
#define BITS_READ(data, bits) ((data) &   (bits)) 

/* type define */
/* error code */
typedef enum
{
    dwin_err_none = 0, 
    dwin_err_error, 
    dwin_err_timeout,
    dwin_err_reinit
} dwin_err_t;

#endif
