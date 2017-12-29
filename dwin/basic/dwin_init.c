/*
 * @File:   dwin_init.c 
 * @Author: liu2guang 
 * @Date:   2017-12-18 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-18     liu2guang    ʵ��dwin���ʼ�������. 
 */ 
#include "dwin_init.h" 
#include "dwin_trans.h" 
#include "dwin_space.h" 
#include "dwin_system.h" 

/* dwin���ʼ��״̬��־λ */
static uint8_t init_flag = dwin_uninit;

/* dwin���ʼ������ */
uint8_t dwin_init(const char *name, uint32_t baudrate)
{
    uint8_t ret = dwin_err_none;
    
    /* ����Ƿ��Ѿ���ʼ�� */
    if(init_flag == dwin_inited)
    {
        return dwin_err_reinit; 
    }
    
    /* ��ʼ�������� */
    ret = dwin_watch_init(name, baudrate);
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    /* ��ʼ���ռ���� */
    ret = dwin_space_init();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
#if (PKG_DWIN_DEBUG >= 1)
    {
        uint8_t major, minor, revise, data[8];
        
        /* �汾��Ϣ */
        dwin_system_hw_version(&major, &minor, &revise);
        dwin_println("Hardware ver %d.%d.%d", major, minor, revise);
        dwin_system_sw_version(&major, &minor, &revise);
        dwin_println("Sortware ver %d.%d.%d", major, minor, revise);
        
        dwin_system_get_backlight_lvl(data);
        dwin_println("Backlight 0x%.2x(%d)", data[0], data[0]);
        
        /* ���������Զ��ϴ� */
        dwin_system_set_autoupdate(1); 
        
        /* l22�������� */
        dwin_system_set_dataload_mode(1); 
        
        /* Ĭ��ʹ�ܷ����� */
        dwin_system_set_buzzer(1); 
    }
#endif
    
    /* ��ʼ����� */
    init_flag = dwin_inited;
    
    return dwin_err_none;
}
