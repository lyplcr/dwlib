/*
 * @File:   dwin_plugin_textbox.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 21:53:11 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    ʵ�����������. 
 */ 
#include "dwin_plugin_inputbox.h" 

/* �ڲ�ʹ�ú� */
#define DWIN_INPUTBOX_SPACE_BYTE (2)    /* ���������ռ��С */

/* ����inputbox */
uint8_t dwin_plugin_inputbox_create(const char *name, inputbox_cb cb, void *args)
{
    dwin_space_t input_space;
    dwin_inputbox_t input_handle;
    
    RT_ASSERT(name != RT_NULL);
    
    /* ���䰴���ռ� */
    input_space = dwin_space_alloc(name, DWIN_INPUTBOX_SPACE_BYTE, dwin_type_inputbox);
    if(input_space == RT_NULL)
    {
        dwin_println("intput [%s] space alloc failed");
        return dwin_err_error;
    }
    
    /* ���䰴����� */
    input_handle = (dwin_inputbox_t)rt_malloc(sizeof(struct dwin_inputbox));
    if(input_handle == RT_NULL)
    {
        dwin_println("intput [%s] handle alloc failed");
        return dwin_err_error;
    }
    
    /* ���ṹ�� */
    input_handle->state = inputbox_start;
    input_handle->input_value = 0;
    input_handle->inputbox_cb = cb;
    input_handle->args = args;
    input_space ->plugin = (void *)input_handle;
    
    return dwin_err_none;
}

/* �޸�inputboxģʽ */
uint8_t dwin_plugin_inputbox_mode(const char *name, uint8_t mode)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    inputbox->state = mode;
    
    return dwin_err_none;
}

/* ����inputbox��Ϣ */
uint8_t dwin_plugin_inputbox_update(const char *name, inputbox_cb cb, void *args)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    inputbox->args = args;
    inputbox->inputbox_cb = cb;
    
    return dwin_err_none;
}

/* ��inputboxֵ */
int32_t dwin_plugin_inputbox_read(const char *name)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return -dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    
    return (int32_t)(inputbox->input_value);
}

/* ���inputboxֵ */
uint8_t dwin_plugin_inputbox_clear(const char *name)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    inputbox->input_value = (-1);
    
    return dwin_err_none;
}
