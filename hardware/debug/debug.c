#include "debug.h"

wifi_command_Q_t g_wifi_cmd_quene;
Command_Info_t   g_SystemVar;

void wifi_cmd_in_quene(unsigned char cmd, unsigned char param)  //wifi�������ѭ������
{
    if (g_wifi_cmd_quene.write_index < WIFI_TO_MCU_COMMAND_BUFFER_SIZE) //д�����index С�ڶ��д�С�ſ���д�룬�������������д��
    {
        g_wifi_cmd_quene.cmd_buf[g_wifi_cmd_quene.write_index].cmd = cmd;  //������ʱ����Ĳ��� cmd param
        g_wifi_cmd_quene.cmd_buf[g_wifi_cmd_quene.write_index].param = param;
        g_wifi_cmd_quene.write_index++;                               //����index ++ ֱ��ѭ��������ʱ�Ų���д�����
    }
}

unsigned char wifi_cmd_out_quene(Command_Info_t *wifi_cmd)
{
    if (g_wifi_cmd_quene.write_index == g_wifi_cmd_quene.read_index) //�ж�wifiѭ�����������Ƿ�������Ϣȫ��ȡ��
    {
        g_wifi_cmd_quene.write_index = 0;
        g_wifi_cmd_quene.read_index = 0;                              //wifi��Ϣ��������û����Ϣ����ȫ����Ϣ���Ѿ�ȡ��
        return 0;                                                     //û����Ϣ����0 ��ʶWiFiû����ϢҪ����
    }

    wifi_cmd->cmd = g_wifi_cmd_quene.cmd_buf[g_wifi_cmd_quene.read_index].cmd;
    wifi_cmd->param = g_wifi_cmd_quene.cmd_buf[g_wifi_cmd_quene.read_index].param;//��WiFiѭ������ȡ����Ϣ
    g_wifi_cmd_quene.read_index++;                                   //ȡһ��ѭ������ index++ ֱ����ָ���дָ����ȱ�ʾ��������û����Ϣ
}

void user_send_command(unsigned char cmd, unsigned char param)
{
    cmd = 1;
    param = 2;
}
void user_respond_command(unsigned char cmd)
{
    cmd = 0;
}
void wifi_routine_handler(void)
{
    if (g_wifi_cmd_quene.command_resend_time > 0) //�жϳ�ʱʱ���Ƿ���0 û�б�ʾû���յ����ڻش�����
    {
        g_wifi_cmd_quene.command_resend_time--;
        if (g_wifi_cmd_quene.command_resend_time == (WIFI_COMMAND_RESEND_TIME -
                2)) //û���յ��豸����ֵ�ٽ���һ�κ������ͱ�֤ͨѶ�ɹ�
        {
            user_send_command(g_SystemVar.cmd, g_SystemVar.param);   //�������·�������
        }
    }
    else
    {
        if (wifi_cmd_out_quene(&g_SystemVar))
        {
            g_wifi_cmd_quene.command_resend_time = WIFI_COMMAND_RESEND_TIME; //��һ�η��ʹ򿪳�ʱ
            user_send_command(g_SystemVar.cmd, g_SystemVar.param);           //���ڷ��ͺ���
        }
    }
}


