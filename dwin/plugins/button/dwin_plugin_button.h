#ifndef __DWIN_PLUGIN_BUTTON_H_ 
#define __DWIN_PLUGIN_BUTTON_H_ 

#include "dwin_space.h" 

/* ������� */
typedef struct dwin_button
{
    uint8_t state;
    uint16_t match_value;
    void (*press_cb)(void *args);
    void *args;
} *dwin_button_t; 

/* ���»ص����� */
typedef void (*press_cb)(void *args);

/* ����ģʽ */
enum button_state
{
    button_stop = 0,    /* ֹͣģʽ */
    button_press        /* ����ģʽ */
};

/* �ⲿ���� */
uint8_t dwin_plugin_button_create(const char *name, press_cb cb, void *args);
uint8_t dwin_plugin_button_mode(const char*name, uint8_t mode);
uint8_t dwin_plugin_button_update(const char*name, press_cb cb, void *args);

#endif
