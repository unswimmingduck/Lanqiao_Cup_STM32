#include "main.h"
#include "RCC\bsp_rcc.h"
#include "KEY_LED\bsp_key_led.h"
#include "LCD\bsp_lcd.h"
//#include "UART\bsp_uart.h"
//#include "I2C\bsp_i2c.h"
#include "ADC\bsp_adc.h"
#include "TIM\bsp_tim.h"
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
uint8_t ucLed;

//*LCD��ʾר�ñ���
uint8_t Lcd_Disp_String[21];//�����ʾ20���ַ�

//*����ר�ñ���
//uint16_t counter = 0;
//uint8_t str[40];
//uint32_t rx_buffer;

//*EEPROM����ر���
//uint8_t EEPROM_String_1[5] = {0x11,0x22,0x33,0x44,0x55};
//uint8_t EEPROM_String_2[5] = {0};

//*4017��ر���
//uint8_t RES_4017;

//*pwm��ر���
//uint16_t PWM_T_Count;
//uint16_t PWM_D_Count;
//float PWM_Duty;


//*rtc��ر���
//RTC_TimeTypeDef H_M_S_Time;
//RTC_DateTypeDef Y_M_D_Date;


//ȫ�ֱ���
_Bool Lcd_Show_State;//0- ���ݽ��棬1- ��������
uint8_t PWM_Running_State = 2;
float R37_Voltage;
uint8_t PA6_Duty_Int = 10;
uint8_t PA7_Duty_Int = 10;





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
	LCD_Clear(Black);
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);	
	
//	UART1_Init();
//	I2CInit();
//  ADC1_Init();
  ADC2_Init();
//	
//	BASIC_TIM6_Init();
//	PWM_INPUT_TIM2_Init();
//	SQU_OUTPUT_TIM15_Init();
	PWM_OUTPUT_TIM3_Init();
	PWM_OUTPUT_TIM17_Init();
//	RTC_Init();



	/*����ʹ�û�������*/	
	//*EEPROM����
//	iic_24c02_write(EEPROM_String_1, 0, 5);
//	HAL_Delay(1);	
//	iic_24c02_read(EEPROM_String_2, 0, 5);	
	
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
	
	//PA6��ɫ
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1000);
		__HAL_TIM_SET_AUTORELOAD(&htim3, 9999);	
	//PA7��ɫ
		__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, 500);
		__HAL_TIM_SET_AUTORELOAD(&htim17, 4999);		
	
	//*������ʱ��3�Ͷ�ʱ��17ͨ�����
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);	//PA6
  HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);		//PA7
		
//		__HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__)
//		__HAL_TIM_SET_AUTORELOAD(__HANDLE__, __AUTORELOAD__)

		
		
		
		
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
		case 1://B1
			Lcd_Show_State ^= 1;
			LCD_Clear(Black);
		break;
		
		case 2://B2
			if(Lcd_Show_State == 1)//�����ڲ�������
			{
				PA6_Duty_Int += 10;
				if(PA6_Duty_Int == 100) PA6_Duty_Int = 10;			
			}
		break;		
	
		case 3://B3   //0- ���ݽ��棬1- ��������
			if(Lcd_Show_State == 0)//���������ݽ���
			{
				if(PWM_Running_State == 0) PWM_Running_State = 1;
				else if(PWM_Running_State == 1) PWM_Running_State = 0;
				else if(PWM_Running_State == 2) PWM_Running_State = 3;
				else if(PWM_Running_State == 3) PWM_Running_State = 2;	
			}
			
			if(Lcd_Show_State == 1)//�����ڲ�������
			{
				PA7_Duty_Int += 10;
				if(PA7_Duty_Int == 100) PA7_Duty_Int = 10;			
			}
		break;	
	
		case 4://B4
			if(PWM_Running_State == 0) PWM_Running_State = 2;
			else if(PWM_Running_State == 2) PWM_Running_State = 0;
			else if(PWM_Running_State == 1) PWM_Running_State = 3;
			else if(PWM_Running_State == 3) PWM_Running_State = 1;		
		break;
	}
	
	
	
	
	
	
}


//***LEDɨ���Ӻ���
void Led_Proc(void)
{
	if((uwTick -  uwTick_Led_Set_Point)<200)	return;//���ٺ���
		uwTick_Led_Set_Point = uwTick;

	if(PWM_Running_State >= 2)//�Զ�ģʽ
	{	
		ucLed |= 0x01;//LD1����
	}
	else //�ֶ�ģʽ
	{
		ucLed &= (~0x01);//LD1Ϩ��	
	}
	
	if(Lcd_Show_State)//��������
	{
		ucLed &= (~0x02);//LD2Ϩ��		
	}
	else//���ݽ���
	{
		ucLed |= 0x02;//LD2����	
	}
	
	
	
	LED_Disp(ucLed);
}


void Lcd_Proc(void)
{
	if((uwTick -  uwTick_Lcd_Set_Point)<100)	return;//���ٺ���
		uwTick_Lcd_Set_Point = uwTick;
	//���ݲɼ���
	R37_Voltage = ((((float)getADC2())/4096)*3.3);
	
	
	//�ҵ���ʾ
	if(Lcd_Show_State == 0)//���ݽ���
	{
		sprintf((char *)Lcd_Disp_String, "      Data");
		LCD_DisplayStringLine(Line0, Lcd_Disp_String);	
		
		sprintf((char *)Lcd_Disp_String, "    V:%4.2fV",R37_Voltage);
		LCD_DisplayStringLine(Line2, Lcd_Disp_String);		
	
		if(PWM_Running_State <= 1)
		{
			sprintf((char *)Lcd_Disp_String, "    Mode:MANU");
			LCD_DisplayStringLine(Line4, Lcd_Disp_String);				
		}
		else
		{
			sprintf((char *)Lcd_Disp_String, "    Mode:AUTO");
			LCD_DisplayStringLine(Line4, Lcd_Disp_String);				
		}			
	}
	else //��������
	{
		sprintf((char *)Lcd_Disp_String, "      Para");
		LCD_DisplayStringLine(Line0, Lcd_Disp_String);		
	
		sprintf((char *)Lcd_Disp_String, "    PA6:%02d%%",(unsigned int)PA6_Duty_Int);
		LCD_DisplayStringLine(Line2, Lcd_Disp_String);	

		sprintf((char *)Lcd_Disp_String, "    PA7:%02d%%",(unsigned int)PA7_Duty_Int);
		LCD_DisplayStringLine(Line4, Lcd_Disp_String);		
	}
		
	
	
	//���������
	
		switch(PWM_Running_State)
		{
			case 0:
					__HAL_TIM_SET_AUTORELOAD(&htim3, 9999);	//PA6����ɫ����Ƶ��Ϊ100hz
					__HAL_TIM_SET_AUTORELOAD(&htim17, 4999);//PA7����ɫ����Ƶ��Ϊ200hz
			
					__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (100*PA6_Duty_Int));//PA6����ɫ����ռ�ձȸ�������
					__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, (50*PA7_Duty_Int));//PA7����ɫ����ռ�ձȸ�������			
			break;
		
			case 1:
					__HAL_TIM_SET_AUTORELOAD(&htim3, 4999);	//PA6����ɫ����Ƶ��Ϊ200hz
					__HAL_TIM_SET_AUTORELOAD(&htim17, 9999);//PA7����ɫ����Ƶ��Ϊ100hz
			
					__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (50*PA6_Duty_Int));//PA6����ɫ����ռ�ձȸ�������
					__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, (100*PA7_Duty_Int));//PA7����ɫ����ռ�ձȸ�������			
			break;		
		
			case 2:
					__HAL_TIM_SET_AUTORELOAD(&htim3, 9999);	//PA6����ɫ����Ƶ��Ϊ100hz
					__HAL_TIM_SET_AUTORELOAD(&htim17, 4999);//PA7����ɫ����Ƶ��Ϊ200hz
			
					__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (unsigned int)(10000*(R37_Voltage/3.3)));//PA6����ɫ����ռ�ձȸ�������
					__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, (unsigned int)(5000*(R37_Voltage/3.3)));//PA7����ɫ����ռ�ձȸ�������			
			break;			
		
			case 3:
					__HAL_TIM_SET_AUTORELOAD(&htim3, 4999);	//PA6����ɫ����Ƶ��Ϊ200hz
					__HAL_TIM_SET_AUTORELOAD(&htim17, 9999);//PA7����ɫ����Ƶ��Ϊ100hz
			
					__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (unsigned int)(5000*(R37_Voltage/3.3)));//PA6����ɫ����ռ�ձȸ�������
					__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, (unsigned int)(10000*(R37_Voltage/3.3)));//PA7����ɫ����ռ�ձȸ�������			
			break;				
			
		}
	
	
	
	
	//������Ļ���Դ���
//	sprintf((char *)Lcd_Disp_String, "PWM-%1d LCD-%1d",(unsigned int)PWM_Running_State, (unsigned int)Lcd_Show_State);
//	LCD_DisplayStringLine(Line9, Lcd_Disp_String);	
	
	//*EEPROM����
//	sprintf((char *)Lcd_Disp_String, "EE:%x%x%x%x%x R:%x",EEPROM_String_2[0],EEPROM_String_2[1],EEPROM_String_2[2],EEPROM_String_2[3],EEPROM_String_2[4], RES_4017);
//	LCD_DisplayStringLine(Line0, Lcd_Disp_String);

	//*ADC����
//	sprintf((char *)Lcd_Disp_String, "R38_Vol:%6.3fV",((((float)getADC1())/4096)*3.3));
//	LCD_DisplayStringLine(Line1, Lcd_Disp_String);	
	
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
