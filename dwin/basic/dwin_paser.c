/*
 * @File:   dwin_paser.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 15:00:00 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    ʵ�ֲ������ͬ������, ������ռ���ϴ�����ʱ��, 
 *                             ���Բ���Ļص�����ȥ�����ʱ����. 
 */ 
#include "list.h" 
#include "dwin_paser.h" 
#include "dwin_space.h" 

/* ���ͷ�ļ� */
#ifdef PKG_DWIN_ENABLE_PLUGIN_BUTTON
#include "dwin_plugin_button.h" 
#endif

#ifdef PKG_DWIN_ENABLE_PLUGIN_INPUTBOX
#include "dwin_plugin_inputbox.h" 
#endif

#ifdef PKG_DWIN_ENABLE_PLUGIN_ICON
#include "dwin_plugin_icon.h" 
#endif

/* ����ռ����������ͷ */
extern list_t *dwin_space_list;

/* �Զ��ϴ������������ */
uint8_t dwin_paser(uint8_t *data, uint8_t len)
{
    list_node_t *node = RT_NULL;
    list_iterator_t *iterator = RT_NULL;
    dwin_space_t space = RT_NULL;
    
    /* �ж�����֡�Ƿ��ǺϷ��� */
    if(data[3] != DWIN_VAR_READ)
    {
        dwin_println("dwin paser data error");
        return dwin_err_error;
    }
    
    /* ��������� */
    iterator = list_iterator_new(dwin_space_list, LIST_HEAD);
    
    /* �����������б� */
    while((node = list_iterator_next(iterator)) != RT_NULL)
    {
        space = (dwin_space_t)(node->val);
        
        /* ��ַƥ�� and ����ƥ�� */
        if(space->addr == ((data[4]<<8)+(data[5])) && space->len == data[6])
        {
            switch(space->type)
            {
#ifdef PKG_DWIN_ENABLE_PLUGIN_BUTTON
                /* ������� */
                case dwin_type_button:
                {
                    dwin_button_t button = (dwin_button_t)(space->plugin);
                    
                    /* δע�ᰴ���ص����� */
                    if(button->press_cb == RT_NULL)
                    {
                    #if (PKG_DWIN_DEBUG >= 2)
                        dwin_println("Button %s no callback func", space->name);
                    #endif
                        break;
                    }
                    
                    /* ����ƥ���ж� */
                    if(button->match_value == ((data[7]<<8)+data[8]))
                    {
                        switch(button->state)
                        {
                            /* ��������״̬ */
                            case button_press:
                            {
                                /* �ص����� */
                                button->press_cb(button->args);
                            }
                            break;
                            
                            /* ����ֹͣ״̬ */
                            case button_stop:
                            {
                            #if (PKG_DWIN_DEBUG >= 2)
                                dwin_println("Button %s stop state", space->name);
                            #endif
                            }
                            break;
                        }
                    }
                }
                break;
#endif
                
                /* ������� */
#ifdef PKG_DWIN_ENABLE_PLUGIN_INPUTBOX
                case dwin_type_inputbox:
                {
                    dwin_inputbox_t inputbox = (dwin_inputbox_t)(space->plugin);
                    
                    /* δע�ᰴ���ص����� */
                    if(inputbox->inputbox_cb == RT_NULL)
                    {
                    #if (PKG_DWIN_DEBUG >= 2)
                        dwin_println("Inputbox %s no callback func", space->name);
                    #endif
                        break;
                    }
                    
                    switch(inputbox->state)
                    {
                        case inputbox_start:
                        {
                            /* ��ȡ����ֵ */
                            inputbox->input_value = (data[7]<<24)+(data[8]<<16)+(data[9]<<8)+data[10];
                        #if (PKG_DWIN_DEBUG >= 2)
                            dwin_println("Inputbox %s input %d", space->name, inputbox->input_value);
                        #endif
                            
                            /* �ص����� */
                            inputbox->inputbox_cb(inputbox->input_value, inputbox->args);
                        }
                        break;
                        
                        /* ����ֹͣ״̬ */
                        case inputbox_stop:
                        {
                        #if (PKG_DWIN_DEBUG >= 2)
                            dwin_println("Inputbox %s stop state", space->name);
                        #endif
                        }
                        break;
                    }
                }
                break;
#endif
            }
        }
    }
    
    list_iterator_destroy(iterator);
    
    return dwin_err_none;
}
