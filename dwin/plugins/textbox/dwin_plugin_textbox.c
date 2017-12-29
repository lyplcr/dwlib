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
 * 2017-12-08     liu2guang    ʵ���ı���. 
 */ 
#include "dwin_plugin_textbox.h" 
#include "dwin_trans.h" 

/* charתshort */ 
static uint16_t *char2short(uint8_t *string)
{
    uint8_t temp, index; 
    
    for(index = 0; index < rt_strlen((const char *)string); index+=2)
    {
        temp = string[index]; 
        string[index] = string[index+1];
        string[index+1] = temp;
    }
    
    return (uint16_t *)string;
}

uint8_t dwin_plugin_textbox_create(const char *name, uint8_t *text, uint8_t len)
{
    dwin_space_t space; 
    dwin_textbox_t handle;
    
    /* �ж��ı����� */
    if(len%2 == 1)
    {
        dwin_println("Text is singular");
        return dwin_err_error;
    }
    
    space = dwin_space_alloc(name, len/2, dwin_type_textbox);
    if(space == RT_NULL)
    {
        dwin_println("textbox [%s] space alloc failed");
        return dwin_err_error;
    }
    
    
    /* ������ */
    handle = (dwin_textbox_t)rt_malloc(sizeof(struct dwin_textbox));
    if(handle == RT_NULL)
    {
        dwin_println("textbox [%s] handle alloc failed");
        return dwin_err_error;
    }
    
    handle->len = len;
    
    /* ����text�����ڴ� */
    handle->text = (uint8_t *)rt_malloc(len);
    if(handle->text == RT_NULL)
    {
        dwin_println("textbox [%s] text alloc failed");
        return dwin_err_error;
    }
    
    rt_strncpy((char *)handle->text, (const char *)text, len);
    space->plugin = (void *)handle;
    
    /* д���� */
    dwin_var_write(space->addr, char2short(text), len/2);
    
    return dwin_err_none;
}

uint8_t dwin_plugin_textbox_update(const char *name, uint8_t *text, uint8_t len)
{
    dwin_space_t space; 
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        dwin_println("No find space");
        return dwin_err_error;
    }
    
    /* ������ֳ����Ƿ񳬳�����ķ�Χ */
    if(len > ((space->len)*2))
    {
        dwin_println("Text is too long");
        return dwin_err_error;
    }
    
    /* ������ֳ����Ƿ�Ϊ���� */
    if(len%2 == 1)
    {
        dwin_println("Text is singular");
        return dwin_err_error;
    }
    
    /* д���� */
    dwin_var_write(space->addr, char2short(text), len/2);
    rt_strncpy((char *)(((dwin_textbox_t)(space->plugin))->text), (const char *)text, len);
    
    return dwin_err_none;
}
