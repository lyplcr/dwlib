/*
 * @File:   dwin_plugin_icon.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 21:53:11 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    ʵ��ͼ����. 
 */ 
#include "dwin_plugin_icon.h" 
#include "dwin_trans.h" 

/* �ڲ�ʹ�ú� */
#define DWIN_ICON_SPACE_BYTE  (1)         /* ���������ռ��С */

/* ����icon�� */
uint8_t dwin_plugin_icon_create(const char *name, uint16_t min, uint16_t max, uint16_t cur)
{
    dwin_space_t icon_space;
    dwin_icon_t  icon_handle;
    
    RT_ASSERT(name != RT_NULL);
    
    /* ���䰴���ռ� */
    icon_space = dwin_space_alloc(name, DWIN_ICON_SPACE_BYTE, dwin_type_icon);
    if(icon_space == RT_NULL)
    {
        dwin_println("Icon [%s] space alloc failed");
        return dwin_err_error;
    }
    
    /* ���䰴����� */
    icon_handle = (dwin_icon_t)rt_malloc(sizeof(struct dwin_icon));
    if(icon_handle == RT_NULL)
    {
        dwin_println("Intput [%s] handle alloc failed");
        return dwin_err_error;
    }
    
    /* ���ṹ�� */
    icon_handle->state = icon_start;
    icon_handle->cur_index = cur;
    icon_handle->min_index = min;
    icon_handle->max_index = max;
    icon_space ->plugin = (void *)icon_handle;
    
    /* �ı�icon��� */
    dwin_plugin_icon_update(name, cur);
    
    return dwin_err_none;
}

/* �ı�icon����ʾ״̬ */
#if 0
uint8_t dwin_plugin_icon_mode(const char *name, uint8_t mode)
{
    dwin_space_t space;
    dwin_icon_t icon;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    icon = (dwin_icon_t)(space->plugin);
    icon->state = mode;
    
    return dwin_err_none;
}
#endif 

/* ����icon����ʾ��� */
uint8_t dwin_plugin_icon_update(const char *name, uint16_t current)
{
    uint8_t ret = dwin_err_none;
    dwin_space_t space;
    dwin_icon_t icon;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    icon = (dwin_icon_t)(space->plugin);
    icon->cur_index = current;
    
    if((icon->cur_index < icon->min_index) || (icon->cur_index > icon->max_index))
    {
        return dwin_err_error;
    }
    
    ret = dwin_var_write(space->addr, &(icon->cur_index), 1);
    if(ret != dwin_err_none)
    {
        return dwin_err_error;
    }
        
    return dwin_err_none;
}

/* ��ȡ��ǰ��ʾ��icon������ */
uint16_t dwin_plugin_icon_read(const char *name)
{
    dwin_space_t space;
    dwin_icon_t icon;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    icon = (dwin_icon_t)(space->plugin);
    
    return (icon->cur_index);
}
