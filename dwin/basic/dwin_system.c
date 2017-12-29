/*
 * @File:   dwin_system.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 16:28:29 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    ʵ��ϵͳ�ӿ�. 
 */ 
#include "dwin_system.h" 
#include "dwin_trans.h" 

/* �Ĵ�����ַ */
#define DWIN_REG_00H_001_VERSION               (0x00)  // ������Ļ�̼��汾
#define DWIN_REG_01H_001_NOW_BL                (0x01)  // ��ǰ��������
#define DWIN_REG_02H_001_MAKE_BEEP             (0x02)  // ��������, ��λ10ms
#define DWIN_REG_03H_002_NOW_PAGEID            (0x03)  // ϵͳ��ǰҳ��ID
#define DWIN_REG_05H_001_TOUCH_FLAG            (0x05)  // ������־λ
#define DWIN_REG_06H_001_TOUCH_STATE           (0x06)  // ����״̬
#define DWIN_REG_07H_004_TOUCH_POS             (0x07)  // ����λ��
#define DWIN_REG_0BH_001_TOUCH_EN              (0x0B)  // ����ʹ��
#define DWIN_REG_10H_001_DRV_MODE              (0x10)  // R0, ����ģʽ(���Ĺ�˾�ڲ�ʹ��)
#define DWIN_REG_11H_001_BRUDRATE              (0x11)  // R1, ͨ�Ų�����
#define DWIN_REG_12H_001_SYSCFG                (0x12)  // R2, ϵͳ����
#define DWIN_REG_13H_001_FARAME_H              (0x13)  // R3, ͨ��֡ͷ���ֽ�
#define DWIN_REG_14H_002_RESERVE               (0x14)  // R4R5, ����
#define DWIN_REG_16H_001_LEDON_LEVEL           (0x16)  // R6, �����������
#define DWIN_REG_17H_001_LEDOFF_LEVEL          (0x17)  // R7, ����Ϩ������
#define DWIN_REG_18H_001_LEDON_KEEPTIME        (0x18)  // R8, �����������ʱ��, ��λ1s
#define DWIN_REG_19H_001_RESERVE               (0x19)  // R9, ����
#define DWIN_REG_1AH_001_FARAME_L              (0x1A)  // RA, ͨ��֡ͷ���ֽ�
#define DWIN_REG_1FH_001_RTC_UPDATE_FLAG       (0x1F)  // �ֶ��޸�RTC���±�־λ
#define DWIN_REG_20H_016_RTC                   (0x20)  // RTC����
#define DWIN_REG_40H_001_FONTLIB_OPT_EN        (0x40)  // �ֿ����ʹ��
#define DWIN_REG_41H_001_FONTLIB_OPT_MODE      (0x41)  // �ֿ�ģʽ
#define DWIN_REG_42H_001_FONTLIB_OPT_ID        (0x42)  // �ֿ�ID
#define DWIN_REG_43H_003_FONTLIB_OPT_ADDR      (0x43)  // �ֿ��ַ
#define DWIN_REG_46H_002_FONTLIB_OPT_VP        (0x46)  // 05ϵ��֧��
#define DWIN_REG_48H_002_FONTLIB_OPT_LENGTH    (0x48)  // 05ϵ��֧��
#define DWIN_REG_4AH_002_TIMER0_16BIT_4MS      (0x4A)  // ��ʱ��0, 16λ����, ����4MS, ����4mS
#define DWIN_REG_4CH_001_TIMER1_08BIT_4MS      (0x4C)  // ��ʱ��1, 08λ����, ����4MS, ����4mS
#define DWIN_REG_4DH_001_TIMER2_08BIT_4MS      (0x4D)  // ��ʱ��2, 08λ����, ����4MS, ����4mS
#define DWIN_REG_4FH_001_KEYCODE               (0x4F)  // ����ֵ
#define DWIN_REG_EBH_001_CURVE_CLR             (0xEB)  // �����������
#define DWIN_REG_ECH_001_FAST_REFRESH          (0xEC)  // ����ˢ��
#define DWIN_REG_F3H_002_DOWNLOAD_LIB          (0xF3)  // �����ֿ�
#define DWIN_REG_F5H_004_DOWNLOAD_PIC          (0xF5)  // ����ͼƬ
#define DWIN_REG_F9H_001_RESERVE               (0xF9)  // ����
#define DWIN_REG_FAH_001_DOWNLOAD_FLAG         (0xFA)  // ���سɹ���־λ

#define DWIN_R2BIT0_BUZZER_EN_MASK             (1<<0)  // ������ʹ��
#define DWIN_R2BIT2_VAR_INIT_MODE_MASK         (1<<2)  // �����ռ��ʼ��ģʽ, 1:�ϵ�ȫ����ʼ��Ϊ0x00, 2��22���ֿ����
#define DWIN_R2BIT3_DATA_AUTO_UPLOAD_MASK      (1<<3)  // ���������Զ��ϴ�
#define DWIN_R2BIT4_CRC_EN_MASK                (1<<4)  // ����֡CRCʹ��, ��֧��
#define DWIN_R2BIT5_BL_CTR_MASK                (1<<5)  // �������Ʊ���
#define DWIN_R2BIT6_HDS_MASK                   (1<<6)  // ��ת180��
#define DWIN_R2BIT7_VHS_MASK                   (1<<7)  // ��ת90��

/* ����ת�� */
static uint8_t bcd2dec(uint8_t bcd)
{
    return bcd-(bcd>>4)*6;
}
static uint8_t dec2bcd(uint8_t dec)
{
    return dec+(dec/10)*6;
}

/* ��ȡ����汾 */
uint32_t dwin_system_sw_version(uint8_t *major, uint8_t *minor, uint8_t *revise)
{
    *major  = DWIN_MAJOR_VER; 
    *minor  = DWIN_MINOR_VER; 
    *revise = DWIN_REVISE_VER; 
    
    return (uint32_t)DWIN_VERSION; 
}

/* ��ȡӲ���ͺ� */
uint32_t dwin_system_hw_version(uint8_t *major, uint8_t *minor, uint8_t *revise)
{
    uint8_t ret = dwin_err_none;
    uint8_t version = 0;
    
    ret = dwin_reg_read(DWIN_REG_00H_001_VERSION, &version, 1);
    if(ret != dwin_err_none)
    {
        return 0;
    }
    
    version = bcd2dec(version);
    *major = version/10;
    *minor = version%10;
    *revise = 0;
    
    return (*major)*10000+(*minor)*100+(*revise); 
}

/* ��ȡ�������� */
uint8_t dwin_system_get_backlight_lvl(uint8_t *lvl)
{
    uint8_t ret = dwin_err_none;
    
    RT_ASSERT(lvl != RT_NULL);
    
    ret = dwin_reg_read(DWIN_REG_01H_001_NOW_BL, lvl, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ���ñ������� */
uint8_t dwin_system_set_backlight_lvl(uint8_t lvl)
{
    uint8_t ret = dwin_err_none;
    
    ret = dwin_reg_write(DWIN_REG_01H_001_NOW_BL, &lvl, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ���Ʒ������� */
uint8_t dwin_system_set_beep(uint8_t time)
{
    uint8_t ret = dwin_err_none;
    
    ret = dwin_reg_write(DWIN_REG_02H_001_MAKE_BEEP, &time, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ���õ�ǰҳid */
uint8_t dwin_system_set_page_id(uint16_t id)
{
    uint8_t ret = dwin_err_none;
    uint8_t id_temp[2];
    
    RT_ASSERT(id != RT_NULL);
    
    id_temp[0] = (id >> 8);
    id_temp[1] = (id << 8) >> 8;
    
    ret = dwin_reg_write(DWIN_REG_03H_002_NOW_PAGEID, id_temp, 2);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ��ȡ��ǰҳid */
uint8_t dwin_system_get_page_id(uint16_t *id)
{
    uint8_t ret = dwin_err_none;
    uint8_t id_temp[2];
    
    RT_ASSERT(id != RT_NULL);
    
    ret = dwin_reg_read(DWIN_REG_03H_002_NOW_PAGEID, id_temp, 2);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    *id = (uint16_t)((id_temp[0] << 8) + id_temp[1]);
    
    return dwin_err_none; 
}

/* ���ñ������ */
uint8_t dwin_system_set_backligth(uint8_t enable)
{
    uint8_t r2  = 0;
    uint8_t ret = dwin_err_none;
    
    ret = dwin_reg_read(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    if(enable == 0)
    {
        BITS_CLR(r2, DWIN_R2BIT5_BL_CTR_MASK);
    }
    else
    {
        BITS_SET(r2, DWIN_R2BIT5_BL_CTR_MASK);
    }
    
    ret = dwin_reg_write(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ���ô��������Ƿ����ϴ� */
uint8_t dwin_system_set_autoupdate(uint8_t enable)
{
    uint8_t r2  = 0;
    uint8_t ret = dwin_err_none;
    
    ret = dwin_reg_read(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    if(enable == 0)
    {
        BITS_CLR(r2, DWIN_R2BIT3_DATA_AUTO_UPLOAD_MASK);
    }
    else
    {
        BITS_SET(r2, DWIN_R2BIT3_DATA_AUTO_UPLOAD_MASK);
    }
    
    ret = dwin_reg_write(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* �������ݼĴ����ϵ��ʼ��ģʽ, 0�ϵ��ʼ��Ϊ0x00, 1�ϵ���L22�ֿ��ļ����� */
uint8_t dwin_system_set_dataload_mode(uint8_t enable)
{
    uint8_t r2  = 0;
    uint8_t ret = dwin_err_none;
    
    ret = dwin_reg_read(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    if(enable == 0)
    {
        BITS_CLR(r2, DWIN_R2BIT2_VAR_INIT_MODE_MASK);
    }
    else
    {
        BITS_SET(r2, DWIN_R2BIT2_VAR_INIT_MODE_MASK);
    }
    
    ret = dwin_reg_write(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ���÷������Ƿ���, 0���� 1δ����, ��Ӱ��dwin_system_set_beep�ӿ� */
uint8_t dwin_system_set_buzzer(uint8_t enable)
{
    uint8_t r2  = 0;
    uint8_t ret = dwin_err_none;
    
    ret = dwin_reg_read(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    if(enable == 0)
    {
        BITS_SET(r2, DWIN_R2BIT0_BUZZER_EN_MASK);
    }
    else
    {
        BITS_CLR(r2, DWIN_R2BIT0_BUZZER_EN_MASK);
    }
    
    ret = dwin_reg_write(DWIN_REG_12H_001_SYSCFG, &r2, 1);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ��ȡʱ�� */
uint8_t dwin_system_get_time(uint8_t *year, uint8_t *month, uint8_t *day, 
    uint8_t *hour, uint8_t *minute, uint8_t *seconds)
{
    uint8_t ret = dwin_err_none;
    uint8_t date_and_time[7] = {0};
    
    ret = dwin_reg_read(DWIN_REG_20H_016_RTC, date_and_time, 7);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    /* �������ݵ�������� */
    *year    = bcd2dec(date_and_time[0]);
    *month   = bcd2dec(date_and_time[1]);
    *day     = bcd2dec(date_and_time[2]);
    *hour    = bcd2dec(date_and_time[4]);
    *minute  = bcd2dec(date_and_time[5]);
    *seconds = bcd2dec(date_and_time[6]);
    
    return dwin_err_none; 
}

/* ����ʱ�� */
uint8_t dwin_system_set_time(uint8_t year, uint8_t month, uint8_t day, 
    uint8_t hour, uint8_t minute, uint8_t seconds)
{
    uint8_t ret = dwin_err_none;
    uint8_t date_and_time[8] = {0};

    /* ������RTC��ַΪ0x20, �����޸���RTC��Ҫ��0x1F��
       д0x5a�����û��޸���RTC����, ������Ļ���޸�ϵ
       RTC����Զ����� */
    date_and_time[0] = 0x5a;
    date_and_time[1] = dec2bcd(year);
    date_and_time[2] = dec2bcd(month);
    date_and_time[3] = dec2bcd(day);
    date_and_time[4] = 0x00;                /* �ܼ�, ��������趨 */
    date_and_time[5] = dec2bcd(hour);
    date_and_time[6] = dec2bcd(minute);
    date_and_time[7] = dec2bcd(seconds);
    
    ret = dwin_reg_write(DWIN_REG_1FH_001_RTC_UPDATE_FLAG, date_and_time, 8);
    if(ret != dwin_err_none)
    {
        return ret;
    }
    
    return dwin_err_none; 
}

/* ��ȡʱ��� */
uint8_t dwin_system_get_timestamp(time_t *timestamp)
{
    return dwin_err_none; 
}

/* ����ʱ��� */
uint8_t dwin_system_set_timestamp(time_t timestamp)
{
    return dwin_err_none; 
}
