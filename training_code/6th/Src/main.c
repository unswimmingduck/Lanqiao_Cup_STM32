#include "main.h"
#include "RCC\bsp_rcc.h"
#include "KEY_LED\bsp_key_led.h"
#include "LCD\bsp_lcd.h"
#include "UART\bsp_uart.h"
#include "I2C\bsp_i2c.h"
#include "ADC\bsp_adc.h"
//#include "TIM\bsp_tim.h"
#include "RTC\bsp_rtc.h"

//***ȫ�ֱ���������
//*���ٱ���
__IO uint32_t uwTick_Key_Set_Point = 0;//����Key_Proc��ִ���ٶ�
__IO uint32_t uwTick_Led_Set_Point = 0;//����Led_Proc��ִ���ٶ�
__IO uint32_t uwTick_Lcd_Set_Point = 0;//����Lcd_Proc��ִ���ٶ�
__IO uint32_t uwTick_Usart_Set_Point = 0;//����Usart_Proc��ִ���ٶ�



//*����ɨ��ר�ñ���
uint8_t ucKey_Val, unKey_Down, ucKey_Up, ucKey_Old;

//*LEDר�ñ���
uint8_t ucLed;

//*LCD��ʾר�ñ���
uint8_t Lcd_Disp_String[21];//�����ʾ20���ַ�

//*����ר�ñ���
uint16_t counter = 0;
uint8_t str[40];
uint8_t rx_buffer;
uint8_t rx_buf[100];//���յ���ָ����ʱ��Ż�����
uint8_t rx_buf_index = 0;//����������buf��ߴ洢��˳��


//*EEPROM����ر���
//uint8_t EEPROM_String_1[5] = {0x11,0x22,0x33,0x44,0x55};
//uint8_t EEPROM_String_2[5] = {0};
//uint8_t K_Storage_EEPROM;

//*4017��ر���
//uint8_t RES_4017;

//*pwm��ر���
//uint16_t PWM_T_Count;
//uint16_t PWM_D_Count;
//float PWM_Duty;


//*rtc��ر���
RTC_TimeTypeDef H_M_S_Time;
RTC_DateTypeDef Y_M_D_Date;


//�û��Զ��������
uint8_t Interface_Num;//0x00-��ʾ���棬0x10-�����ϱ�ʱ���Сʱ��0x11-���÷��ӣ�0x12-�����롣
float R37_Voltage;
uint8_t k_int = 1;
uint8_t LED_Ctrl = 0;// 0-�򿪣�1�رգ�����LED��������
uint8_t Clock_Comp_Disp[3] = {0,0,0};//���ӱȽ�ֵ�ĳ�ֵ����ʾר�ã�
uint8_t Clock_Comp_Ctrl[3] = {0,0,0};//���ӱȽ�ֵ�ĳ�ֵ������ר�ã�
__IO uint32_t uwTick_SETTING_TIME_Set_Point = 0;//���ƴ����õ�ʱ����ֵ��˸
uint8_t SETTING_TIME_Ctrl = 0;// 0-����1-�𣬿���ʱ�����ý���Ĵ�����ֵ����˸����
uint8_t Ctrl_Uart_Send_Time_Data_Times = 0;// ����ֻ��������ʱ���ֻ�ϱ�һ��
__IO uint32_t Uart_Rev_Data_Delay_Time = 0;//���ƴ��ڽ������ݵĵȴ�ʱ��
_Bool Start_Flag;//��ʼλ�ж�
__IO uint32_t uwTick_LED_bulingbuling_Set_Point = 0;//����LED������˸�Ĵ�����
	
//***�Ӻ���������
void Key_Proc(void);
void Led_Proc(void);
void Lcd_Proc(void);
void Usart_Proc(void);


//***ϵͳ������
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

	/*bsp��Դ�ĳ�ʼ��*/
	KEY_LED_Init();
	
	LCD_Init();
	LCD_Clear(White);
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);	
	
	UART1_Init();
	I2CInit();
//  ADC1_Init();
  ADC2_Init();
//	
//	BASIC_TIM6_Init();
//	PWM_INPUT_TIM2_Init();
//	SQU_OUTPUT_TIM15_Init();
//	PWM_OUTPUT_TIM3_Init();
//	PWM_OUTPUT_TIM17_Init();
  RTC_Init();



	/*����ʹ�û�������*/	
	//*EEPROM����
//	iic_24c02_write(EEPROM_String_1, 0, 5);
//	HAL_Delay(1);	
//	iic_24c02_read(EEPROM_String_2, 0, 5);
	iic_24c02_read(&k_int,0,1);//ȥEEPOM�ж�ȡk�ĳ�ֵ
	
  //*MCP4017����
//	write_resistor(0x77);
//	RES_4017 = read_resistor();	
	
	//*���ڽ����жϴ�
	HAL_UART_Receive_IT(&huart1, (uint8_t *)(&rx_buffer), 1);
	
	//*�򿪻�����ʱ��
//	HAL_TIM_Base_Start_IT(&htim6);//ÿ100ms����һ���ж�
		
	//*���벶��PWM����
//	HAL_TIM_Base_Start(&htim2);  /* ������ʱ�� */
//  HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);		  /* ������ʱ��ͨ�����벶�񲢿����ж� */
//  HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);		
		
	//*�������PA2����	
//  HAL_TIM_OC_Start_IT(&htim15,TIM_CHANNEL_1);
	
	//*������ʱ��3�Ͷ�ʱ��17ͨ�����
//  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);	//PA6
//  HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);		//PA7
		

		
		
		
		
  while (1)
  {
		Key_Proc();
		Led_Proc();
		Lcd_Proc();
		Usart_Proc();
		
		
		
  }

}


//***����ɨ���Ӻ���
void Key_Proc(void)
{
	if((uwTick -  uwTick_Key_Set_Point)<50)	return;//���ٺ���
		uwTick_Key_Set_Point = uwTick;

	ucKey_Val = Key_Scan();
	unKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val); 
	ucKey_Up = ~ucKey_Val & (ucKey_Old ^ ucKey_Val);	
	ucKey_Old = ucKey_Val;
	
//	if(unKey_Down == 4)
//	{
//		ucLed = 0x88;
//	}
//	if(unKey_Down == 3)
//	{
//		ucLed = 0x00;
//	}	
//uint8_t Interface_Num;//0x00-��ʾ���棬0x10-�����ϱ�ʱ���Сʱ��0x11-���÷��ӣ�0x12-�����롣
	
	if(unKey_Down == 1)//B1���LED�������ܵĴ򿪺͹ر�
	{
		LED_Ctrl ^= 0x01;//�����һλ����
	}
	
	if(unKey_Down == 2)//B2�������������л�
	{
		if(Interface_Num == 0x00)
		{
			LCD_Clear(White);//����
			Interface_Num = 0x10;
			
			
		}
		else if((Interface_Num>>4) == 0x1)
		{
			LCD_Clear(White);//����
			Interface_Num = 0x00;	

			Clock_Comp_Ctrl[0] = Clock_Comp_Disp[0];	//����������ʾֵ������ֵ
			Clock_Comp_Ctrl[1] = Clock_Comp_Disp[1];	//����������ʾֵ������ֵ	
			Clock_Comp_Ctrl[2] = Clock_Comp_Disp[2];	//����������ʾֵ������ֵ				
		}			
	}
	if(unKey_Down == 3)//B3�л�ʱ����
	{
	 if((Interface_Num>>4) == 0x1)
			if(++Interface_Num == 0x13)
					Interface_Num = 0x10;
	}	
	if(unKey_Down == 4)//B4�����趨ֵ��ʱ����
	{
		if(Interface_Num == 0x10)
		{
			if( ++Clock_Comp_Disp[0] == 24)
				Clock_Comp_Disp[0] = 0;
		}
		else if(Interface_Num == 0x11)
		{
			if( ++Clock_Comp_Disp[1] == 60)
				Clock_Comp_Disp[1] = 0;		
		}
		else if(Interface_Num == 0x12)
		{
			if( ++Clock_Comp_Disp[2] == 60)
				Clock_Comp_Disp[2] = 0;			
		}
	}		
	
	
	
	
}


//***LEDɨ���Ӻ���
void Led_Proc(void)
{
	if((uwTick -  uwTick_Led_Set_Point)<50)	return;//���ٺ���
		uwTick_Led_Set_Point = uwTick;

	if(LED_Ctrl == 0x1)//�ر�LED�Ĺ��ܵ�ʱ��
	{
		ucLed = 0x00;
	}
	else//����LED���ܵ�ʱ��
	{
		if(R37_Voltage>=(3.3*k_int*0.1))
		{
			if((uwTick-uwTick_LED_bulingbuling_Set_Point)>=200)
			{
				uwTick_LED_bulingbuling_Set_Point = uwTick;
				ucLed ^= 0x1;
			}
		}
		else
			ucLed = 0x00;			
	}
//	
	LED_Disp(ucLed);
}


void Lcd_Proc(void)
{
	if((uwTick -  uwTick_Lcd_Set_Point)<100)	return;//���ٺ���
		uwTick_Lcd_Set_Point = uwTick;
	
//	//������Ļ���Դ���
//	sprintf((char *)Lcd_Disp_String, "TEST :%02X",Interface_Num);
//	LCD_DisplayStringLine(Line9, Lcd_Disp_String);	
//	
//	//*EEPROM����
//	sprintf((char *)Lcd_Disp_String, "EE:%x%x%x%x%x",EEPROM_String_2[0],EEPROM_String_2[1],EEPROM_String_2[2],EEPROM_String_2[3],EEPROM_String_2[4]);
//	LCD_DisplayStringLine(Line0, Lcd_Disp_String);

//	//*ADC����
////	sprintf((char *)Lcd_Disp_String, "R38_Vol:%6.3fV",((((float)getADC1())/4096)*3.3));
////	LCD_DisplayStringLine(Line1, Lcd_Disp_String);	
////	
//	sprintf((char *)Lcd_Disp_String, "R37_Vol:%6.3fV",((((float)getADC2())/4096)*3.3));
//	LCD_DisplayStringLine(Line2, Lcd_Disp_String);	
//	
//	//*PWM���벶����ԣ�����ռ�ձȺ�Ƶ��
////	sprintf((char *)Lcd_Disp_String, "R40P:%05dHz,%4.1f%%",(unsigned int)(1000000/PWM_T_Count),PWM_Duty*100);
////	LCD_DisplayStringLine(Line3, Lcd_Disp_String);		
//	
//	//*RTC������ʾ
//	HAL_RTC_GetTime(&hrtc, &H_M_S_Time, RTC_FORMAT_BIN);//��ȡ���ں�ʱ�����ͬʱʹ��
//	HAL_RTC_GetDate(&hrtc, &Y_M_D_Date, RTC_FORMAT_BIN);
//	sprintf((char *)Lcd_Disp_String, "Time:%02d-%02d-%02d",(unsigned int)H_M_S_Time.Hours,(unsigned int)H_M_S_Time.Minutes,(unsigned int)H_M_S_Time.Seconds);
//	LCD_DisplayStringLine(Line4, Lcd_Disp_String);		

		//���ݲɼ���������λ�ã�
		R37_Voltage = ((((float)getADC2())/4096)*3.3);
		HAL_RTC_GetTime(&hrtc, &H_M_S_Time, RTC_FORMAT_BIN);//��ȡ���ں�ʱ�����ͬʱʹ��
		HAL_RTC_GetDate(&hrtc, &Y_M_D_Date, RTC_FORMAT_BIN);

		//������ʾ��
		if(Interface_Num == 0x00)//������ʾ����
		{
			sprintf((char *)Lcd_Disp_String, "    V1:%4.2fV",R37_Voltage);
			LCD_DisplayStringLine(Line2, Lcd_Disp_String);	
			
			sprintf((char *)Lcd_Disp_String, "    k:%3.1f",(k_int*0.1));
			LCD_DisplayStringLine(Line4, Lcd_Disp_String);	
			
			if(LED_Ctrl == 0)
				sprintf((char *)Lcd_Disp_String, "    LED:ON ");		
			else
				sprintf((char *)Lcd_Disp_String, "    LED:OFF");						
		  LCD_DisplayStringLine(Line6, Lcd_Disp_String);	
		
			sprintf((char *)Lcd_Disp_String, "    T:%02d-%02d-%02d",(unsigned int)H_M_S_Time.Hours,(unsigned int)H_M_S_Time.Minutes,(unsigned int)H_M_S_Time.Seconds);
			LCD_DisplayStringLine(Line8, Lcd_Disp_String);			
		}
		
		//ʱ��������
		if((Interface_Num>>4) == 0x1)//�������ý���
		{		
			sprintf((char *)Lcd_Disp_String, "       Setting");
			LCD_DisplayStringLine(Line2, Lcd_Disp_String);					
			sprintf((char *)Lcd_Disp_String, "      %02d-%02d-%02d",(unsigned int)Clock_Comp_Disp[0],(unsigned int)Clock_Comp_Disp[1],(unsigned int)Clock_Comp_Disp[2]);
			if((uwTick - uwTick_SETTING_TIME_Set_Point)>=500)
			{
				uwTick_SETTING_TIME_Set_Point = uwTick;
				SETTING_TIME_Ctrl ^= 0x1;
			}
			
			if(SETTING_TIME_Ctrl == 0x1)//����������˸��ʱ������ʱ�����˸��
			{
				if(Interface_Num == 0x10)//����ʱ	
					{
						Lcd_Disp_String[6] = ' ';
						Lcd_Disp_String[7] = ' ';						
					}
				else if(Interface_Num == 0x11)//���÷�
				{
						Lcd_Disp_String[9] = ' ';
						Lcd_Disp_String[10] = ' ';					
				}
				else if(Interface_Num == 0x12)//������
				{
						Lcd_Disp_String[12] = ' ';
						Lcd_Disp_String[13] = ' ';					
				}
			}
			LCD_DisplayStringLine(Line5, Lcd_Disp_String);		

			
		}
		






}

void Usart_Proc(void)
{
	if((uwTick -  uwTick_Usart_Set_Point)<30)	return;//���ٺ���
	uwTick_Usart_Set_Point = uwTick;
	
//	sprintf(str, "%04d:Hello,world.\r\n", counter);
//	HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);
//	
//	if(++counter == 10000)
//		counter = 0;
	
	//����ʱ�䵽
	if((H_M_S_Time.Hours == Clock_Comp_Ctrl[0])&&(H_M_S_Time.Minutes == Clock_Comp_Ctrl[1])&&(H_M_S_Time.Seconds == Clock_Comp_Ctrl[2]))
	{

		if(Ctrl_Uart_Send_Time_Data_Times == 0)//����ֻ����һ�����ݣ���Ȼͨ��ɨ����������
		{
				Ctrl_Uart_Send_Time_Data_Times = 1;
				sprintf(str, "%4.2f+%3.1f+%02d%02d%02d\n", R37_Voltage,(k_int*0.1),(unsigned int)H_M_S_Time.Hours,(unsigned int)H_M_S_Time.Minutes,(unsigned int)H_M_S_Time.Seconds);
				HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);			
		}
	}
	else 
		Ctrl_Uart_Send_Time_Data_Times = 0;	//��ʱ��仯���߿���ֵ�仯�����߲��ȵ�ʱ�򣬻ָ���һ�����ݷ�������

	//���ڽ��յ����ݴ���
		if(((uwTick - Uart_Rev_Data_Delay_Time)<=300)&&(uwTick - Uart_Rev_Data_Delay_Time)>=200)//200ms~300ms֮�ڴ�������
	{
		if(rx_buf_index==6)//���յ���6������
		{
			if((rx_buf[0] == 0x6B)&&(rx_buf[1] == 0x30)&&(rx_buf[2] == 0x2E)&&(rx_buf[4] == 0x5C)&&(rx_buf[5] == 0x6E))
			{
				if((rx_buf[3]>=0x31)&&(rx_buf[3]<=0x39))
				{
					k_int = rx_buf[3] - 0x30; 
					sprintf(str, "OK\n");
					HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);		
					iic_24c02_write(&k_int, 0, 1);					
				}
			}			
		}
	  rx_buf_index = 0;	
		Start_Flag = 0;		
	}

}


//���ڽ����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if((rx_buffer == 0x6B)&&(rx_buf_index == 0))
	{
		Uart_Rev_Data_Delay_Time = uwTick;//���յ���һ������������ʱ		
		Start_Flag = 1;
	}
	if(Start_Flag == 1)
	{		
		rx_buf[rx_buf_index] = rx_buffer;
		rx_buf_index++;		
	}
		
	HAL_UART_Receive_IT(&huart1, (uint8_t *)(&rx_buffer), 1);
	
}



//������ʱ��6���»ص�����
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance==TIM6)
//  {
////		if(++counter == 10)
////		{
////			counter = 0;
////			sprintf(str, "Hello,world.\r\n");
////			HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);	
////		}
//	}
//}


//���벶��PWM�жϻص�
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//	  if(htim->Instance==TIM2)
//  {
//				if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//			{
//				PWM_T_Count =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1)+1;
//				PWM_Duty = (float)PWM_D_Count/PWM_T_Count;
//			}
//				else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
//			{
//				PWM_D_Count =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2)+1;
//			}			
//	}	
//}




//��������ص�����
//void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  if(htim->Instance==TIM15)
//  {
//				if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//			{			
//			  __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,(__HAL_TIM_GetCounter(htim)+500));//1Khz
//			}
//	}
//}







/******************************************************************************
/******************************************************************************
/******************************************************************************
/******************************************************************************
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}
