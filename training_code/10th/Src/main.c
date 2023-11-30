#include "main.h"
#include "RCC\bsp_rcc.h"
#include "KEY_LED\bsp_key_led.h"
#include "LCD\bsp_lcd.h"
//#include "UART\bsp_uart.h"
#include "I2C\bsp_i2c.h"
#include "ADC\bsp_adc.h"
//#include "TIM\bsp_tim.h"
//#include "RTC\bsp_rtc.h"

//***ȫ�ֱ���������
//*���ٱ���
__IO uint32_t uwTick_Key_Set_Point = 0;//����Key_Proc��ִ���ٶ�
__IO uint32_t uwTick_Led_Set_Point = 0;//����Led_Proc��ִ���ٶ�
__IO uint32_t uwTick_Lcd_Set_Point = 0;//����Lcd_Proc��ִ���ٶ�
//__IO uint32_t uwTick_Usart_Set_Point = 0;//����Usart_Proc��ִ���ٶ�

//*����ɨ��ר�ñ���
uint8_t ucKey_Val, unKey_Down, ucKey_Up, ucKey_Old;

//*LEDר�ñ���
uint8_t ucLed = 0x0;
uint8_t ucLed_Bor = 0x01;

//*LCD��ʾר�ñ���
uint8_t Lcd_Disp_String[21];//�����ʾ20���ַ�

//*����ר�ñ���
//uint16_t counter = 0;
//uint8_t str[40];
//uint32_t rx_buffer;

//*EEPROM����ر���
uint8_t EEPROM_String_1[5] = {0x11,0x22,0x33,0x44,0x55};
uint8_t EEPROM_String_2[5] = {0};

//*4017��ر���
//uint8_t RES_4017;

//*pwm��ر���
//uint16_t PWM_T_Count;
//uint16_t PWM_D_Count;
//float PWM_Duty;


//*rtc��ر���
//RTC_TimeTypeDef H_M_S_Time;
//RTC_DateTypeDef Y_M_D_Date;


//ϵͳȫ�ֱ���
uint8_t Sys_State;//����״̬
float Voltage;//R37�ĵ�ѹֵ
uint8_t Vot_State;//��ѹ״̬ 0-���� 1-�м� 2-����
uint8_t Max_Min_LED[4] = {24,12,1,2};//0-����ֵ 1-����ֵ 2-�߱����� 3-�ͱ�����


//***�Ӻ���������
void Key_Proc(void);
void Led_Proc(void);
void Lcd_Proc(void);
//void Usart_Proc(void);


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
	
//	UART1_Init();
	I2CInit();
//  ADC1_Init();
  ADC2_Init();
//	
//	BASIC_TIM6_Init();
//	PWM_INPUT_TIM2_Init();
//	SQU_OUTPUT_TIM15_Init();
//	PWM_OUTPUT_TIM3_Init();
//	PWM_OUTPUT_TIM17_Init();
//	RTC_Init();



	/*����ʹ�û�������*/	
	//*EEPROM����
//	iic_24c02_write(EEPROM_String_1, 0, 5);
//	HAL_Delay(1);	
  	iic_24c02_read(Max_Min_LED, 0, 4);	
	
  //*MCP4017����
//	write_resistor(0x77);
//	RES_4017 = read_resistor();	
	
	//*���ڽ����жϴ�
//	HAL_UART_Receive_IT(&huart1, (uint8_t *)(&rx_buffer), 4);
	
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
//		Usart_Proc();
		
		
		
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

	
	switch(unKey_Down)
	{
		case 1://B1����
			if(Sys_State == 0)  
			{
				Sys_State = 1;
			}
			else 
			{
				Sys_State = 0;	
				iic_24c02_write(Max_Min_LED, 0, 4);
			}	
			LCD_Clear(White);
		break;
		
		case 2://B2����
			if(Sys_State)
			{
				if(++Sys_State == 5)
					Sys_State = 1;
			}
		break;
	
	//	uint8_t Max_Min_LED[4];//0-����ֵ 1-����ֵ 2-�߱����� 3-�ͱ�����
    case 3://B3����+
			switch(Sys_State)
			{
				case 1://����ֵ+0.3
					Max_Min_LED[0] += 3;
					if((Max_Min_LED[0] >= 33)&&(Max_Min_LED[0] <= 40))
						Max_Min_LED[0] = 33;
				break;
				
				case 2://����ֵ+0.3
					if((Max_Min_LED[1]+3) < Max_Min_LED[0])
								Max_Min_LED[1] += 3;						
				break;	
				
				case 3:
						if(++Max_Min_LED[2] >=8) Max_Min_LED[2] = 8;
						if(Max_Min_LED[2] == Max_Min_LED[3])
						{
							if(Max_Min_LED[3] == 8)
								Max_Min_LED[2] = 7;
							else 
							{
								if(++Max_Min_LED[2] >=8) 
								Max_Min_LED[2] = 8;		
							}								
						}							
				break;	
				
				case 4:
						if(++Max_Min_LED[3] >=8) Max_Min_LED[3] = 8;
						if(Max_Min_LED[3] == Max_Min_LED[2])
						{
							if(Max_Min_LED[2] == 8)
								Max_Min_LED[3] = 7;
							else 
							{
								if(++Max_Min_LED[2] >=8) 
								Max_Min_LED[2] = 8;		
							}								
						}									
				break;			
			
			
			}	
		break;
	
		
		case 4://B4����-
			switch(Sys_State)
			{
				case 1://����ֵ-0.3
					if((Max_Min_LED[0]-3) > Max_Min_LED[1])
								Max_Min_LED[0] -= 3;						
				break;
				
				case 2://����ֵ-0.3
					Max_Min_LED[1] -= 3;
					if(Max_Min_LED[1] >= 200)
						Max_Min_LED[1] = 0;					
				break;		

				case 3://-
						if(--Max_Min_LED[2] == 0) Max_Min_LED[2] = 1;
						if(Max_Min_LED[2] == Max_Min_LED[3])
						{
							if(Max_Min_LED[3] == 1)
								Max_Min_LED[2] = 2;
							else 
							{
								if(--Max_Min_LED[2] == 0) 
								Max_Min_LED[2] = 1;		
							}								
						}							
				break;	
				
				case 4://-
						if(--Max_Min_LED[3] == 0) Max_Min_LED[3] = 1;
						if(Max_Min_LED[3] == Max_Min_LED[2])
						{
							if(Max_Min_LED[2] == 1)
								Max_Min_LED[3] = 2;
							else 
							{
								if(--Max_Min_LED[2] ==0) 
								Max_Min_LED[2] = 1;		
							}								
						}									
				break;

					
			}
		break;
	
	
	
	
	
	
	
	
	}
	
	
}


//***LEDɨ���Ӻ���
void Led_Proc(void)
{
	if((uwTick -  uwTick_Led_Set_Point)<200)	return;//���ٺ���
		uwTick_Led_Set_Point = uwTick;
//Max_Min_LED    2-�߱����� 3-�ͱ�����    LD8    ucLed  1000 0000
	switch(Vot_State)
	{
		case 0://��������
			ucLed ^= ( ucLed_Bor << (Max_Min_LED[3]-1));
		
		break;
		
		case 1://�м�
			ucLed = 0x00;
		
		break;		
	
		case 2://��������
			ucLed ^= ( ucLed_Bor << (Max_Min_LED[2]-1));			
		
		break;	
	}
	
	
	
	
	LED_Disp(ucLed);
}


void Lcd_Proc(void)
{
	if((uwTick -  uwTick_Lcd_Set_Point)<100)	return;//���ٺ���
		uwTick_Lcd_Set_Point = uwTick;
	//�ɼ�����ר������
//	uint8_t Max_Min_LED[4];//0-����ֵ 1-����ֵ 2-�߱����� 3-�ͱ�����
	Voltage = ((((float)getADC2())/4096)*3.3);
	if((Voltage*10) > Max_Min_LED[0])
		Vot_State = 2;
	else if((Voltage*10) < Max_Min_LED[1])
		Vot_State = 0;	
	else 
		Vot_State = 1;			
	
	
	//�ҵĴ���
	if(!Sys_State)//��������
	{
		sprintf((char *)Lcd_Disp_String, "    Main   ");
		LCD_DisplayStringLine(Line1, Lcd_Disp_String);	
		
		sprintf((char *)Lcd_Disp_String, "  Volt:%4.2fV",Voltage);
		LCD_DisplayStringLine(Line4, Lcd_Disp_String);		
		
		switch(Vot_State)
		{
			case 0:
				sprintf((char *)Lcd_Disp_String, "  Status:Lower    ");
			break;
			
			case 1:
				sprintf((char *)Lcd_Disp_String, "  Status:Normal    ");
			break;				
			
			case 2:
				sprintf((char *)Lcd_Disp_String, "  Status:Upper    ");
			break;				
		}
		LCD_DisplayStringLine(Line6, Lcd_Disp_String);			
		
	}
	else//�����ý���
	{
//	uint8_t Max_Min_LED[4];//0-����ֵ 1-����ֵ 2-�߱����� 3-�ͱ�����		
		sprintf((char *)Lcd_Disp_String, "    Setting   ");
		LCD_DisplayStringLine(Line1, Lcd_Disp_String);	

		sprintf((char *)Lcd_Disp_String, " Max Volt:%3.1fV   ",((float)Max_Min_LED[0]/10.0));
		if(Sys_State == 1) 		LCD_SetBackColor(Green);
		LCD_DisplayStringLine(Line4, Lcd_Disp_String);				
		LCD_SetBackColor(White);
		
		sprintf((char *)Lcd_Disp_String, " Min Volt:%3.1fV   ",((float)Max_Min_LED[1]/10.0));
		if(Sys_State == 2) 		LCD_SetBackColor(Green);
		LCD_DisplayStringLine(Line5, Lcd_Disp_String);			
		LCD_SetBackColor(White);
		
		sprintf((char *)Lcd_Disp_String, " Upper:LD%1d    ",(unsigned int)Max_Min_LED[2]);
		if(Sys_State == 3) 		LCD_SetBackColor(Green);		
		LCD_DisplayStringLine(Line6, Lcd_Disp_String);				
		LCD_SetBackColor(White);		
		
		sprintf((char *)Lcd_Disp_String, " Lower:LD%1d     ",(unsigned int)Max_Min_LED[3]);
		if(Sys_State == 4) 		LCD_SetBackColor(Green);				
		LCD_DisplayStringLine(Line7, Lcd_Disp_String);		
		LCD_SetBackColor(White);		
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//������Ļ���Դ���
//	sprintf((char *)Lcd_Disp_String, "TEST CODE:%02X",ucLed);
//	LCD_DisplayStringLine(Line9, Lcd_Disp_String);	
//	
//	//*EEPROM����
//	sprintf((char *)Lcd_Disp_String, "EE:%x%x%x%x%x",EEPROM_String_2[0],EEPROM_String_2[1],EEPROM_String_2[2],EEPROM_String_2[3],EEPROM_String_2[4]);
//	LCD_DisplayStringLine(Line0, Lcd_Disp_String);

	//*ADC����
//	sprintf((char *)Lcd_Disp_String, "R38_Vol:%6.3fV",((((float)getADC1())/4096)*3.3));
//	LCD_DisplayStringLine(Line1, Lcd_Disp_String);	
//	
//	sprintf((char *)Lcd_Disp_String, "R37_Vol:%6.3fV",((((float)getADC2())/4096)*3.3));
//	LCD_DisplayStringLine(Line2, Lcd_Disp_String);	
	
	//*PWM���벶����ԣ�����ռ�ձȺ�Ƶ��
//	sprintf((char *)Lcd_Disp_String, "R40P:%05dHz,%4.1f%%",(unsigned int)(1000000/PWM_T_Count),PWM_Duty*100);
//	LCD_DisplayStringLine(Line3, Lcd_Disp_String);		
	
	//*RTC������ʾ
//	HAL_RTC_GetTime(&hrtc, &H_M_S_Time, RTC_FORMAT_BIN);//��ȡ���ں�ʱ�����ͬʱʹ��
//	HAL_RTC_GetDate(&hrtc, &Y_M_D_Date, RTC_FORMAT_BIN);
//	sprintf((char *)Lcd_Disp_String, "Time:%02d-%02d-%02d",(unsigned int)H_M_S_Time.Hours,(unsigned int)H_M_S_Time.Minutes,(unsigned int)H_M_S_Time.Seconds);
//	LCD_DisplayStringLine(Line4, Lcd_Disp_String);		

}

//void Usart_Proc(void)
//{
//	if((uwTick -  uwTick_Usart_Set_Point)<1000)	return;//���ٺ���
//	uwTick_Usart_Set_Point = uwTick;
//	
////	sprintf(str, "%04d:Hello,world.\r\n", counter);
////	HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);
//	
////	if(++counter == 10000)
////		counter = 0;
//}


//���ڽ����жϻص�����
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
////	counter++;
////	sprintf(str, "%04d:Hello,world.\r\n", counter);
////	HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);	
////	
////	HAL_UART_Receive_IT(&huart1, (uint8_t *)(&rx_buffer), 4);

//}



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
