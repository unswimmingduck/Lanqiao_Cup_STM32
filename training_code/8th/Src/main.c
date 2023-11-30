#include "main.h"
#include "RCC\bsp_rcc.h"
#include "KEY_LED\bsp_key_led.h"
#include "LCD\bsp_lcd.h"
//#include "UART\bsp_uart.h"
//#include "I2C\bsp_i2c.h"
//#include "ADC\bsp_adc.h"
#include "TIM\bsp_tim.h"
#include "RTC\bsp_rtc.h"

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
RTC_TimeTypeDef H_M_S_Time;
RTC_DateTypeDef Y_M_D_Date;

//ȫ�ֱ���
uint8_t ucPlat = 1;//1234��ʾ��ǰ������ƽ̨
//_Bool PA4_Voltage = 0;
//_Bool PA5_Voltage = 0;
uint8_t ucSet;//���ڼ�¼���Ű�������ȥ�ˡ�ֻ�õ�4λ,�ֱ��ӦLD4~LD1������B4~B1�� _ _ _ _  [ _ _ _ _ ].
uint8_t ucState;//״̬����0~8.
uint8_t Dir;//�������з������ 0 -û���У�1-�ϣ�2-��
uint8_t Flow = 0x10;//��ˮ�ı���
__IO uint32_t uwTick_Set_Point = 0;//��ʱר��





//***�Ӻ���������
void Key_Proc(void);
void Led_Proc(void);
void Lcd_Proc(void);
//void Usart_Proc(void);
void Elev_Proc(void);

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
//	I2CInit();
//  ADC1_Init();
//  ADC2_Init();
//	
//	BASIC_TIM6_Init();
//	PWM_INPUT_TIM2_Init();
//	SQU_OUTPUT_TIM15_Init();
	PWM_OUTPUT_TIM3_Init();   // PA6     ------> TIM3_CH1
	PWM_OUTPUT_TIM17_Init();   //PA7     ------> TIM17_CH1
	RTC_Init();



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
	
	//*������ʱ��3�Ͷ�ʱ��17ͨ�����
//	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 800);//�޸�ռ�ձȵĻ������� D=0.8
//  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);	//PA6   1khz  ��ɫ����
//	
//	__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, 250);//�޸�ռ�ձȵĻ������� D=0.5	
//  HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);		//PA7   2khz  ��ɫ����
	
	
  while (1)
  {
		Key_Proc();
		Led_Proc();
		Lcd_Proc();
		Elev_Proc();
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
//	ucSet
//	ucPlat
	
	if(ucState == 0)
	{
		if(unKey_Down == 1)//B1��ѹ
		{
			if(ucPlat != 1) ucSet |= 0x01; 
		}	
		else if(unKey_Down == 2)//B2��ѹ
		{
			if(ucPlat != 2) ucSet |= 0x02; 
		}	
		else if(unKey_Down == 3)//B3��ѹ
		{
			if(ucPlat != 3) ucSet |= 0x04; 	
		}		
		else if(unKey_Down == 4)//B4��ѹ
		{
			if(ucPlat != 4) ucSet |= 0x08; 	
		}		
		
		ucLed &= 0xF0;
		ucLed |= ucSet;
		
		if(unKey_Down != 0)//���а�������ȥ��������ʱ
		{
			uwTick_Set_Point = uwTick;
		}
		
	}
}


//***LEDɨ���Ӻ���
void Led_Proc(void)
{
	if((uwTick -  uwTick_Led_Set_Point)<200)	return;//���ٺ���
		uwTick_Led_Set_Point = uwTick;

	LED_Disp(ucLed);
}


void Lcd_Proc(void)
{
	if((uwTick -  uwTick_Lcd_Set_Point)<100)	return;//���ٺ���
		uwTick_Lcd_Set_Point = uwTick;
	
	//������Ļ���Դ���
	sprintf((char *)Lcd_Disp_String, " Current Platform",ucLed);
	LCD_DisplayStringLine(Line1, Lcd_Disp_String);	
	
	
	sprintf((char *)Lcd_Disp_String, "          %1d",(unsigned int)ucPlat);
	LCD_DisplayStringLine(Line3, Lcd_Disp_String);	

	//*EEPROM����
//	sprintf((char *)Lcd_Disp_String, "EE:%x%x%x%x%x R:%x",EEPROM_String_2[0],EEPROM_String_2[1],EEPROM_String_2[2],EEPROM_String_2[3],EEPROM_String_2[4], RES_4017);
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
	HAL_RTC_GetTime(&hrtc, &H_M_S_Time, RTC_FORMAT_BIN);//��ȡ���ں�ʱ�����ͬʱʹ��
	HAL_RTC_GetDate(&hrtc, &Y_M_D_Date, RTC_FORMAT_BIN);
	sprintf((char *)Lcd_Disp_String, "       %02d-%02d-%02d",(unsigned int)H_M_S_Time.Hours,(unsigned int)H_M_S_Time.Minutes,(unsigned int)H_M_S_Time.Seconds);
	LCD_DisplayStringLine(Line6, Lcd_Disp_String);		


//	sprintf((char *)Lcd_Disp_String, " PA4:%1d  PA5:%1d",(unsigned int)PA4_Voltage,(unsigned int)PA5_Voltage);
//	LCD_DisplayStringLine(Line8, Lcd_Disp_String);	

}



void Elev_Proc(void)
{
	if(ucSet)//����û�û������Ŀ��㣬��ִ�д˳���ֻ�а��������ˣ��ſ��ԡ�
	{
		switch(ucState)
		{
			
			
			case 0://�ȴ��������º�1sʱ��ĵ���
				if((uwTick - uwTick_Set_Point) >= 1000)
					ucState = 1;
				else 
					break;
				
				
			case 1://���Ƶ����Źر� PA7 -  PWM    2KHZ    D=50%     PA5= 0  
				
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);		//����			
				__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, 250);//�޸�ռ�ձȵĻ������� D=0.5	
				HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);		//PA7   2khz  ��ɫ����		
			
				sprintf((char *)Lcd_Disp_String, " Door Closing          ");
				LCD_DisplayStringLine(Line8, Lcd_Disp_String);	
			
			  uwTick_Set_Point = uwTick;
				ucState = 2;
			
			
			case 2://�ȴ����ݹ���ʱ�䵽��4s������֮��ִ�к�ߵĲ����������˳���
				if((uwTick - uwTick_Set_Point) >= 4000)
				{
					HAL_TIM_PWM_Stop(&htim17,TIM_CHANNEL_1);		//PA7   2khz  ��ɫ����	
					
					sprintf((char *)Lcd_Disp_String, " Door Closed          ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);	
					
					ucState = 3;				
				}
				else 
					break;	
				
				
			case 3://�жϵ�ǰ�����������趨����֮��Ĺ�ϵ������Ӧ�����л������У����������С�
				//ucPlat=1~4=1(0001) 2(0010) 3(0100) 4(1000)      ucSet= 0010B
				if(ucSet > (1<<(ucPlat-1)))//���� PA6  -  PWM    1KHZ   D=80%      PA4 = 1
				{
					Dir = 1;//����
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);		//	��������		
//					PA4_Voltage = 1;//Ϊ����Ļ�������з���
					__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 800);//�޸�ռ�ձȵĻ������� D=0.8
					HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);	//PA6   1khz  ��ɫ����
					
					sprintf((char *)Lcd_Disp_String, "Elev Upping          ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);	
					
				}
				else if(ucSet < (1<<(ucPlat-1)))//����  PA6  -  PWM    1KHZ    D=60%         PA4 = 0 
				{
					Dir = 2;//����
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);		//	��������		
//					PA4_Voltage = 0;//Ϊ����Ļ�������з���
					__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 600);//�޸�ռ�ձȵĻ������� D=0.6
					HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);	//PA6   1khz  ��ɫ����		

					sprintf((char *)Lcd_Disp_String, "Elev Downing           ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);									
				}
				
			  uwTick_Set_Point = uwTick;
				ucState = 4;				
			
				
			case 4://�жϵ�����6sʱ���Ƿ����������������ǰ���ڲ����仯1����λ�����û�е�6s����ˮ��Ч����
				if((uwTick - uwTick_Set_Point) >= 6000)
				{
					if(Dir == 1) ucPlat++;
					else if(Dir == 2) ucPlat--;
					
					sprintf((char *)Lcd_Disp_String, "          %1d",(unsigned int)ucPlat);
					LCD_DisplayStringLine(Line3, Lcd_Disp_String);					

					sprintf((char *)Lcd_Disp_String, "Elev Runned 1 Floor        ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);						
					
					ucLed &=	0x0F;
					Flow = 0x10;
					ucState = 5;				
				}
				else 
				{
					if(Dir == 1)//���У�����
					{
						Flow = (Flow >> 1);//0001 0000  -  0000 1000
						if(Flow == 0x08)
							Flow = 0x80;
						ucLed &=	0x0F;
						ucLed |= Flow;
					}
					else if(Dir == 2)//���У��ҵ���
					{
						ucLed &=	0x0F;
						ucLed |= Flow;
						Flow = (Flow << 1);//1000 0000  -  0000 0000
						if(Flow == 0x00)
							Flow = 0x10;					
					}
					HAL_Delay(300);
					break;	
				}
				
				
				
				
			case 5://ucPlat��ǰ�� 3    ucSet 0000 1100
				if((1<<(ucPlat-1))&ucSet)//����Ϊ���ʱ�򣬱�ʾ����Ŀ���
				{
					HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);	//PA6   1khz  ��ɫ���Σ�ֹͣ���εķ���						

					HAL_Delay(300);					
					sprintf((char *)Lcd_Disp_String, "             ");
					LCD_DisplayStringLine(Line3, Lcd_Disp_String);	
					HAL_Delay(300);								
					sprintf((char *)Lcd_Disp_String, "          %1d",(unsigned int)ucPlat);
					LCD_DisplayStringLine(Line3, Lcd_Disp_String);						
					HAL_Delay(300);					
					sprintf((char *)Lcd_Disp_String, "             ");
					LCD_DisplayStringLine(Line3, Lcd_Disp_String);	
					HAL_Delay(300);								
					sprintf((char *)Lcd_Disp_String, "          %1d",(unsigned int)ucPlat);
					LCD_DisplayStringLine(Line3, Lcd_Disp_String);					
					
					//���ݿ��ţ�  PA7 -  PWM    2KHZ    D=60%          PA5= 1 ��
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);		//����			
//					PA5_Voltage = 1;//Ϊ����Ļ���Թ��ŷ���
					__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, 300);//�޸�ռ�ձȵĻ������� D=0.6
					HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);		//PA7   2khz  ��ɫ����		
					
					sprintf((char *)Lcd_Disp_String, "Comed , Door Opening            ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);	
					
					uwTick_Set_Point = uwTick;
					ucState = 6;					
				}		
				else //û�е���Ŀ���
				{
					uwTick_Set_Point = uwTick;
					ucState = 4;
					break;		
				}
				
			case 6://�ȴ����ݿ���4sʱ���Ƿ񵽴�
				if((uwTick - uwTick_Set_Point) >= 4000)
				{
					HAL_TIM_PWM_Stop(&htim17,TIM_CHANNEL_1);		//PA7   2khz  ��ɫ����	
					
					sprintf((char *)Lcd_Disp_String, "Door Opened           ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);	
					
					ucSet &= (~(1<<(ucPlat-1)));//�û����õ�Ŀ�����Ŀ�� 1000
					ucLed &= 0xF0;
					ucLed |= ucSet;					
					LED_Disp(ucLed);					
					
					ucState = 7;				
				}
				else 
					break;	
				
				
			case 7://�б��Ƿ��б��Ŀ���
				if(ucSet)//������б��Ŀ��ƽ̨���ȴ�2��
				{
					uwTick_Set_Point = uwTick;
					
					sprintf((char *)Lcd_Disp_String, "Waitting 2s           ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);						
					
					ucState = 8;						
				}
				else //û�б��Ŀ����
				{
					ucState = 0;
					
					sprintf((char *)Lcd_Disp_String, "                  ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);	
					
					break;					
				}
			
				
			case 8://�жϵ���Ŀ���֮�󣬵��ݿ��ź��Ƿ�ȴ���2s��ʱ���Ѿ����
				if((uwTick - uwTick_Set_Point) >= 2000)
				{		
					sprintf((char *)Lcd_Disp_String, "                  ");
					LCD_DisplayStringLine(Line8, Lcd_Disp_String);
					
					ucState = 1;										
				}	
		}
	
	}
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
