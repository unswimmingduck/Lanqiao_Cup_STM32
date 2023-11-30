#include "main.h"
#include "RCC\bsp_rcc.h"
#include "KEY_LED\bsp_key_led.h"
#include "LCD\bsp_lcd.h"
#include "UART\bsp_uart.h"
//#include "I2C\bsp_i2c.h"
//#include "ADC\bsp_adc.h"
#include "TIM\bsp_tim.h"
//#include "RTC\bsp_rtc.h"
#include <string.h>

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
uint8_t str_str[40];
uint8_t rx_buffer;

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


//***�Ӻ���������
void Key_Proc(void);
void Led_Proc(void);
void Lcd_Proc(void);
void Usart_Proc(void);

//ȫ�ֱ�����
_Bool Disp_Num;//0-������ʾ��1-��������
_Bool PWM_Output_Num;//0-�͵�ƽ��1-PWM

float VNBR_Price = 2.00;
float CNBR_Price = 3.50;

uint8_t VNBR_Use_Num;
uint8_t CNBR_Use_Num;
uint8_t No_Use_Num = 8;

uint8_t RX_BUF[200];//���ڻ������200���ֽڵ�����
uint8_t Rx_Counter;//���ڼ�¼�����˶��ٸ����ݣ�ͬʱ��������RX_BUF�е�����λ��


typedef struct
{
	uint8_t type[5];
	uint8_t id[5];
	uint8_t year_in;
	uint8_t month_in;
	uint8_t day_in;
	uint8_t hour_in;
	uint8_t min_in;
	uint8_t sec_in;
	_Bool notEmpty;
} Car_Data_Storage_Type;

Car_Data_Storage_Type Car_Data_Storage[8];//���ݿ⹹����ϣ����ڴ洢8�������ĳ�����Ϣ








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
	
	UART1_Init();
//	I2CInit();
//  ADC1_Init();
//  ADC2_Init();
//	
//	BASIC_TIM6_Init();
//	PWM_INPUT_TIM2_Init();
//	SQU_OUTPUT_TIM15_Init();
//	PWM_OUTPUT_TIM3_Init();
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

	__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1, 0);//ǿ�����óɵ͵�ƽ
  HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);		//PA7
		
//Car_Data_Storage[5].id[0] = 'M';
//Car_Data_Storage[5].id[1] = 'Y';	
//Car_Data_Storage[5].id[2] = 'G';
//Car_Data_Storage[5].id[3] = 'C';		
//Car_Data_Storage[5].id[4] = '\0';
		
		
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

	
	switch(unKey_Down)
	{
		case 1://B1
			Disp_Num ^= 0x1;
			LCD_Clear(Black);
		break;
		
		case 2://B2
			if(Disp_Num == 1)//�������ý���
			{
			 VNBR_Price += 0.5;
       CNBR_Price += 0.5;	
			}			
		break;	
	
		case 3://B3
			if(Disp_Num == 1)//�������ý���
			{
				if((VNBR_Price - 0.5)> 0)
				{
					VNBR_Price -= 0.5;
					CNBR_Price -= 0.5;	
				}
			}				
		break;	
	
		case 4://B4
			PWM_Output_Num ^= 0x1;
			if(PWM_Output_Num == 0)//�͵�ƽ
			{
					__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1, 0);//ǿ�����óɵ͵�ƽ
			}
			else//�ߵ�ƽ 
			{
					__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1, 100);//ǿ�����ó�PWM��ƽ					
			}
		break;	
	
	}
	
	
	
	
}


//***LEDɨ���Ӻ���
void Led_Proc(void)
{
	if((uwTick -  uwTick_Led_Set_Point)<200)	return;//���ٺ���
		uwTick_Led_Set_Point = uwTick;

	if(No_Use_Num > 0)//��ʾ���г�λ
	{
		ucLed |= 0x1;	
	}
	else//�������
	{
		ucLed &= (~0x1);		
	}
	
	if(PWM_Output_Num == 0)//�͵�ƽ
	{
		ucLed &= (~0x2);	
	}
	else//PWM
	{		
    ucLed |= 0x2;		
	}
	
	LED_Disp(ucLed);
}


void Lcd_Proc(void)
{
	if((uwTick -  uwTick_Lcd_Set_Point)<100)	return;//���ٺ���
		uwTick_Lcd_Set_Point = uwTick;

	
	//�û�����
	if(Disp_Num == 0)//���ݽ���
	{
		sprintf((char *)Lcd_Disp_String, "       Data");
		LCD_DisplayStringLine(Line1, Lcd_Disp_String);		
	
		sprintf((char *)Lcd_Disp_String, "   CNBR:%1d",(unsigned int)CNBR_Use_Num);
		LCD_DisplayStringLine(Line3, Lcd_Disp_String);		
	
		sprintf((char *)Lcd_Disp_String, "   VNBR:%1d",(unsigned int)VNBR_Use_Num);
		LCD_DisplayStringLine(Line5, Lcd_Disp_String);			

		sprintf((char *)Lcd_Disp_String, "   IDLE:%1d",(unsigned int)No_Use_Num);
		LCD_DisplayStringLine(Line7, Lcd_Disp_String);		


	}
	else//��������
	{
		sprintf((char *)Lcd_Disp_String, "       Para");
		LCD_DisplayStringLine(Line1, Lcd_Disp_String);			
	
		sprintf((char *)Lcd_Disp_String, "   CNBR:%4.2f",CNBR_Price);
		LCD_DisplayStringLine(Line3, Lcd_Disp_String);		
	
		sprintf((char *)Lcd_Disp_String, "   VNBR:%4.2f",VNBR_Price);
		LCD_DisplayStringLine(Line5, Lcd_Disp_String);		

	}
	
	//������Ļ���Դ���
//	sprintf((char *)Lcd_Disp_String, "TEST CODE:%02X",ucLed);
//	LCD_DisplayStringLine(Line9, Lcd_Disp_String);	
	
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
//	HAL_RTC_GetTime(&hrtc, &H_M_S_Time, RTC_FORMAT_BIN);//��ȡ���ں�ʱ�����ͬʱʹ��
//	HAL_RTC_GetDate(&hrtc, &Y_M_D_Date, RTC_FORMAT_BIN);
//	sprintf((char *)Lcd_Disp_String, "Time:%02d-%02d-%02d",(unsigned int)H_M_S_Time.Hours,(unsigned int)H_M_S_Time.Minutes,(unsigned int)H_M_S_Time.Seconds);
//	LCD_DisplayStringLine(Line4, Lcd_Disp_String);		

}


_Bool CheckCmd(uint8_t* str)//�����б���ܵ�22���ַ��Ƿ�Ϸ�
{
	if(Rx_Counter != 22)
		return 0;//��ʾ������22������
	if(((str[0]=='C')||(str[0]=='V'))&&(str[1]=='N')&&(str[2]=='B')&&(str[3]=='R')&&(str[4]==':')&&(str[9]==':'))
	{
		uint8_t i;
		for(i = 10; i< 22;i++)
		{
			if((str[i]>'9')||(str[i]<'0'))
				return 0;
		}
		return 1;//��ʾ���յ�������û����
	}
}

void substr(uint8_t* d_str, uint8_t* s_str, uint8_t locate, uint8_t length)//�ӳ��ַ��������ȡ��һ�θ����ַ���
{
	uint8_t i = 0;
	for(i=0; i<length; i++)
	{
		d_str[i] = s_str[locate + i];
	}
	d_str[length] = '\0';
}


//�б𳵵�id�Ƿ��ڿ����
uint8_t isExist(uint8_t* str)
{
	uint8_t i = 0;	
	for(i=0; i<8; i++)
	{
		if((strcmp((const char*)str,(const char*)Car_Data_Storage[i].id)) == 0)//��ʾ�ַ���ƥ�䣬������ַ���		
		{
			return i;//�����id�����ݿ���ţ��������id�����ݿ⵱�е�λ��
		}
	}	
	return 0xFF;//���û�У�����oxff
}

//�б����ݿ���0-7�ţ��ĸ�λ���пյ�
uint8_t findLocate(void)
{
	uint8_t i = 0;
	for(i = 0;i <= 7; i++ )
	{
		if(Car_Data_Storage[i].notEmpty == 0)
			return i;//0-7��λ
	}
	return 0XFF;
}

//CNBR:MYGC:201212121212
//CNBR:MYGC:201212121213

//VNBR:MYG7:201212121212
//VNBR:MYG7:211212121213

//CNBR:MYG6:201212121212
//CNBR:MYG5:201212121212
//VNBR:MYG1:201212121212
//VNBR:MYG2:201212121212
//VNBR:MYG3:201212121212
//VNBR:MYG4:201212121212





void Usart_Proc(void)
{
	if((uwTick -  uwTick_Usart_Set_Point)<100)	return;//���ٺ���
	uwTick_Usart_Set_Point = uwTick;
//	
//	sprintf(str, "%04d:Hello,world.\r\n", counter);
//	HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);
//	
//	if(++counter == 10000)
//		counter = 0;
	
//		sprintf((char *)Lcd_Disp_String, "   test:%1d",(unsigned int)CheckCmd(RX_BUF));
//		LCD_DisplayStringLine(Line9, Lcd_Disp_String);			
	if(CheckCmd(RX_BUF))//�ֲڵ��жϣ���һ�����б����ݸ����Լ����ݸ�ʽ�Ƿ�Ϸ�
	{
		
		uint8_t car_id[5];
		uint8_t car_type[5];	
		uint8_t year_temp,month_temp,day_temp,hour_temp,min_temp,sec_temp;
		
		
		year_temp = (((RX_BUF[10] - '0')*10) + (RX_BUF[11] - '0')); 
		month_temp = (((RX_BUF[12] - '0')*10) + (RX_BUF[13] - '0')); 	
		day_temp = (((RX_BUF[14] - '0')*10) + (RX_BUF[15] - '0')); 	
		hour_temp = (((RX_BUF[16] - '0')*10) + (RX_BUF[17] - '0')); 	
		min_temp = (((RX_BUF[18] - '0')*10) + (RX_BUF[19] - '0')); 	
		sec_temp = (((RX_BUF[20] - '0')*10) + (RX_BUF[21] - '0')); 		
		if((month_temp>12)||(day_temp>31)||(hour_temp>23)||(min_temp>59)||(sec_temp>59))//��֤���ں�ʱ���Ƿ�Ϸ�
		{
			goto SEND_ERROR;
		}
		
		substr(car_id, RX_BUF, 5, 4);//��ȡ����id
		substr(car_type, RX_BUF, 0, 4);	//��ȡ��������
//		isExist(car_id);
//		sprintf((char *)Lcd_Disp_String, "   test:%x",isExist(car_id));
//		LCD_DisplayStringLine(Line9, Lcd_Disp_String);					
//		sprintf((char *)Lcd_Disp_String, "   test:%X%X%X%X%x",car_id[0],car_id[1],car_id[2],car_id[3],car_id[4]);
//		LCD_DisplayStringLine(Line9, Lcd_Disp_String);	
			
	
		
		//**********************����û�н���******
		if(isExist(car_id) == 0xFF)//��ʾ�����û����������ID����ʾ�������û����
		{
			uint8_t locate = findLocate();//�ҵ��ĸ��ط��ǿյ�

			
			if(locate == 0xFF)//û���ҵ��ĸ��ط��ǿյ�
			{
			  goto SEND_ERROR;
			}
			
			//׼���洢
			substr(Car_Data_Storage[locate].type, car_type, 0, 4);//�ѵ�ǰ�������ʹ���			
			substr(Car_Data_Storage[locate].id, car_id, 0, 4);//�ѵ�ǰ����id����
			Car_Data_Storage[locate].year_in = year_temp;
			Car_Data_Storage[locate].month_in = month_temp;			
			Car_Data_Storage[locate].day_in = day_temp;			
			Car_Data_Storage[locate].hour_in = hour_temp;			
			Car_Data_Storage[locate].min_in = min_temp;			
			Car_Data_Storage[locate].sec_in = sec_temp;		
			Car_Data_Storage[locate].notEmpty = 1;
			
			if(Car_Data_Storage[locate].type[0] == 'C')
				CNBR_Use_Num++;
			else if(Car_Data_Storage[locate].type[0] == 'V')
				VNBR_Use_Num++;

			No_Use_Num--;
		
		}
		
		//**********************������Ѿ������ˣ������ǳ�ȥ******		
		else if(isExist(car_id) != 0xFF)//��ʾ���ݿ�����������Ϣ�������������ݿ��λ��
		{
			int64_t Second_derta;//���ں���Сʱ�Ĳ�ֵ	
			
			uint8_t in_locate = isExist(car_id);//��ס�����ݿ��е�λ��
			
			
			if(strcmp((const char*)car_type,(const char*)Car_Data_Storage[in_locate].type) != 0)//˵����ƥ��
			{
			  goto SEND_ERROR;			
			}
			
			//2000 2001 2002  //1   2  3
			Second_derta = (year_temp - Car_Data_Storage[in_locate].year_in)*365*24*60*60 + (month_temp - Car_Data_Storage[in_locate].month_in)*30*24*60*60+\
				(day_temp - Car_Data_Storage[in_locate].day_in)*24*60*60 + (hour_temp - Car_Data_Storage[in_locate].hour_in)*60*60 + \
				(min_temp - Car_Data_Storage[in_locate].min_in)*60 + (sec_temp - Car_Data_Storage[in_locate].sec_in);
			
			if(Second_derta < 0)//˵����ȥ��ʱ�䳬ǰ��ȥ��ʱ��
			{
					goto SEND_ERROR;			
			}
//			
			Second_derta = (Second_derta + 3599)/3600;  //Сʱ�����Ѿ��������
			sprintf(str_str, "%s:%s:%d:%.2f\r\n",Car_Data_Storage[in_locate].type,Car_Data_Storage[in_locate].id,(unsigned int)Second_derta,(Second_derta*(Car_Data_Storage[in_locate].id[0]=='C'?CNBR_Price:VNBR_Price)));
			HAL_UART_Transmit(&huart1,(unsigned char *)str_str, strlen(str_str), 50);		
//			
			if(Car_Data_Storage[in_locate].type[0] == 'C')
				CNBR_Use_Num--;
			else if(Car_Data_Storage[in_locate].type[0] == 'V')
				VNBR_Use_Num--;

			No_Use_Num++;			
			
			memset(&Car_Data_Storage[in_locate],0,sizeof(Car_Data_Storage[in_locate]));//��ո�λ���������ݣ�Ϊ0
			
		}
//		
//		
		
		
		
		goto CMD_YES;
		
		SEND_ERROR:	
			sprintf(str_str, "Error\r\n");
			HAL_UART_Transmit(&huart1,(unsigned char *)str_str, strlen(str_str), 50);
		
		CMD_YES:
				memset(&RX_BUF[0],0,sizeof(RX_BUF));//��ո�λ���������ݣ�Ϊ0
				Rx_Counter = 0;
		
		
	
	}
}


//���ڽ����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	counter++;
//	sprintf(str, "%04d:Hello,world.\r\n", counter);
//	HAL_UART_Transmit(&huart1,(unsigned char *)str, strlen(str), 50);	
//	

	
	RX_BUF[Rx_Counter] = rx_buffer;
	Rx_Counter++;
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
