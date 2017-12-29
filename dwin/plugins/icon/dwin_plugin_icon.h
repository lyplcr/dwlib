#ifndef __DWIN_PLUGIN_ICON_H_ 
#define __DWIN_PLUGIN_ICON_H_ 

#include "dwin_space.h" 

/* icon�ṹ�� */
typedef struct dwin_icon
{
    uint8_t state;
    uint16_t cur_index;
    uint16_t min_index;
    uint16_t max_index;
} *dwin_icon_t; 

/* icon״̬ */
enum icon_state
{
    icon_stop = 0,    /* ֹͣģʽ */
    icon_start,       /* ����ģʽ */
    icon_autoplay     /* �Զ�����ģʽ, �Ժ�ʵ�� */
};

/* �����ӿ� */
uint8_t  dwin_plugin_icon_create(const char *name, uint16_t min, uint16_t max, uint16_t cur);
uint8_t  dwin_plugin_icon_update(const char *name, uint16_t current);
// uint8_t  dwin_plugin_icon_mode(const char *name, uint8_t mode);
uint16_t dwin_plugin_icon_read(const char *name);

#endif
