
#include "ds1302.h"
#include "extern.h"
#include "button.h"
#define alarm_set_mode 1
#define clock_mode 0

t_ds1302 ds1302;
uint8_t button_state = 0;
uint8_t alarm_state = 0;
uint8_t toggle_lcd = 0;
t_alarm set_alarm;

void display_date_time(void);

void ds1302_main(void)
{
	//init_date_time();
	button_pull_up();
	int i=0;
	init_alarm();
	flash_set_time();
	init_gpio_ds1302();
	init_ds1302();
	i2c_lcd_init();

	while(1)
	{
		read_time_ds1302();
		read_date_ds1302();
		pc_command_processing();

		state_trans(); // 상태천이함수는 계속 호출
		printf("check_ds1302!\n");
		if (TIM11_1ms_counter > 1000)
		{
			TIM11_1ms_counter=0;

			if(!alarm_state)
			{
				if(button_state == clock_mode)
				{
					display_date_time(); // 시계출력
				}
				else if(button_state == alarm_set_mode)
				{
					display_alarm_time();
				}
			}
			else // 알람이 울릴 때
			{
				toggle_lcd = !toggle_lcd;
				if(toggle_lcd)
					display_date_time();
				else
					lcd_command(0x01); // 화면 지우기
			}
			if(++i > 7) // flash memory는 너무 자주 사용하지 않도록 설정
			{
				i = 0;
				ds1302.magic=0x55555555;
				flash_erase();
				flash_write((uint32_t *) &ds1302, sizeof(ds1302));
			}
		}
	}
}

void display_date_time(void) // 시계 출력 함수
{
	char lcd_buff[40];

	sprintf(lcd_buff, "DATE:%4d-%02d-%02d ",
				ds1302.year+2000,
				ds1302.month,
				ds1302.date);
	move_cursor(0,0);
	lcd_string(lcd_buff);

	sprintf(lcd_buff, "TIME:%02d:%02d:%02d   ",
				ds1302.hours,
				ds1302.minutes,
				ds1302.seconds);
	move_cursor(1,0);
	lcd_string(lcd_buff);
}

void display_alarm_time(void) // 알람 설정 화면 출력 함수
{
	char lcd_buff[40];

	sprintf(lcd_buff, "ALARM : %02d-%02d-%02d",
				set_alarm.hours,
				set_alarm.minutes,
				set_alarm.seconds);
	move_cursor(0,0);
	lcd_string(lcd_buff);

	move_cursor(1,0);
	lcd_string("Setting Mode!!!!");
}

void init_alarm(void) // 알람값 초기화
{
	set_alarm.hours = 0;
	set_alarm.minutes = 0;
	set_alarm.seconds = 0;
}

void state_trans(void) // 버튼에 따른 상태천이
{
	if (get_button(GPIOC, GPIO_PIN_0, BTN0 ) == BUTTON_PRESS)
	{
		button_state = !button_state; // 0 시계 display, 1 알람시계 세팅화면 display
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // 디버깅용
	}
	if(button_state == alarm_set_mode) // 알람시각 설정하는 모드
	{
		if (get_button(GPIOC, GPIO_PIN_1, BTN1 ) == BUTTON_PRESS) // 알람시계 시각설정
		{
			set_alarm.hours++;
			set_alarm.hours %= 24;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
		}
		if (get_button(GPIOC, GPIO_PIN_2, BTN2 ) == BUTTON_PRESS) // 알람시계 분 설정
		{
			set_alarm.minutes++;
			set_alarm.minutes %= 60;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		}
		if (get_button(GPIOC, GPIO_PIN_3, BTN3 ) == BUTTON_PRESS) // 알람시계 초 설정
		{
			set_alarm.seconds++;
			set_alarm.seconds %= 60;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
		}
	}

	if( set_alarm.hours == ds1302.hours &&
		set_alarm.minutes == ds1302.minutes &&
		set_alarm.seconds == ds1302.seconds)
	{
		alarm_state = 1; // 알람이 울리게 함
	}

	if (alarm_state)
	{
		if (get_button(GPIOC, GPIO_PIN_1, BTN1 ) == BUTTON_PRESS)
		{
			alarm_state = 0; // 알람끄기
			init_alarm();    // 저장된 알람값 초기화
		}
	}
}

void read_time_ds1302(void)
{
	ds1302.seconds = read_ds1302(ADDR_SECONDS);
	ds1302.minutes = read_ds1302(ADDR_MINUTES);
	ds1302.hours = read_ds1302(ADDR_HOURS);
}

void read_date_ds1302(void)
{
	ds1302.date = read_ds1302(ADDR_DATE);
	ds1302.month = read_ds1302(ADDR_MONTH);
	ds1302.dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	ds1302.year = read_ds1302(ADDR_YEAR);
}

uint8_t read_ds1302(uint8_t addr)
{
	unsigned char data8bits=0;  // 1bit씩 넘어온것을 담을 그릇(변수)
	// 1. CE high
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);
	// 2. addr 전송
	tx_ds1302(addr+1);   // read addr
	// 3. data를 읽어 들인다.
	rx_ds1302(&data8bits);
	// 4. CE low
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
	// 5. return (bcd to dec )
	return bcd2dec(data8bits);
}
// 1. 입력 : bcd
// 예) 25년의 bcd --> dec
//     7654 3210
//     0010 0101
//       2   5
//     x10  X1
//  =============
//       25
uint8_t bcd2dec(uint8_t bcd)
{
	uint8_t high, low;

	low = bcd & 0x0f;
	high = (bcd >> 4) * 10;     // 00100101 bcd >> 4 ==> 0000010 x 10

	return (high+low);
}
// dec --> bcd
// 예) 25
//  dec        bcd
// 00011001   0010 0101
uint8_t dec2bcd(uint8_t dec)
{
	uint8_t high, low;

	high = (dec / 10) << 4;
	low = dec % 10;

	return (high+low);
}

void rx_ds1302(unsigned char *pdata)
{
	unsigned char temp=0;
	// IO 포트를 입력 모드로 전환
	input_dataline_ds1302();
	// DS1302로 부터 들어온 bit를 LSB부터 8bit를 받아서 temp변수에 저장
	for (int i=0; i < 8; i++)
	{
		// 1bit를 읽어 들인다.
		if (HAL_GPIO_ReadPin(GPIOA, IO_DS1302_Pin))
		{
			// 1의 조건만 set
			temp |= 1 << i;
		}
		if (i != 7)
			clock_ds1302();
	}
	*pdata = temp;
}

void init_ds1302(void)
{
	write_ds1302(ADDR_SECONDS, ds1302.seconds);
	write_ds1302(ADDR_MINUTES, ds1302.minutes);
	write_ds1302(ADDR_HOURS, ds1302.hours);
	write_ds1302(ADDR_DATE, ds1302.date);
	write_ds1302(ADDR_MONTH, ds1302.month);
	write_ds1302(ADDR_DAYOFWEEK, ds1302.dayofweek);
	write_ds1302(ADDR_YEAR, ds1302.year);
}

write_ds1302(uint8_t addr, uint8_t data)
{
	// 1. CE low --> high
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);
	// 2. addr 전송
	tx_ds1302(addr);
	// 3. data 전송
	tx_ds1302(dec2bcd(data));
	// 4. CE high --> low
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
}

tx_ds1302(uint8_t value)
{
	output_dataline_ds1302();
   // 예) addr 초를 write하는
   // 80h M       L
   //     1000 0000  실제값  (B0를 전송시의 )
   //     0000 0001 &
   //     0000 0000  ==> HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
   //     1000 0000  실제값  (B7를 전송시의 )
   //     1000 0000 &
	//    1000 0000  ==> HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);
	for (int i=0; i < 8; i++)
	{
		if (value & (1 << i))
		{
			HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, 1);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, 0);
		}
		clock_ds1302();
	}

}

void input_dataline_ds1302()
{
	GPIO_InitTypeDef GPIO_init = {0,};

	GPIO_init.Pin = IO_DS1302_Pin;
	GPIO_init.Mode = GPIO_MODE_INPUT;  // input mode
	GPIO_init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_init);
}

void output_dataline_ds1302()
{
	GPIO_InitTypeDef GPIO_init = {0,};

	GPIO_init.Pin = IO_DS1302_Pin;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;  // output mode
	GPIO_init.Pull = GPIO_NOPULL;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;   // LOW: 2M HIGHL 25~100MHz
	HAL_GPIO_Init(GPIOA, &GPIO_init);
}

void clock_ds1302(void)
{
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 0);
}

void init_gpio_ds1302(void)
{
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 0);
}
void init_date_time(void)
{
	ds1302.magic=0x55555555;
	ds1302.year=25;
	ds1302.month=4;
	ds1302.date=1;
	ds1302.dayofweek=2; // tue
	ds1302.hours=15;
	ds1302.minutes=25;
	ds1302.seconds=00;
}

// setrtc250331120500
//       YYMMDDHHmmSS
//  date_time에는 241008154500 의 주소
void set_rtc(char *date_time)
{
    char yy[4], mm[4], dd[4];   // date
    char hh[4], min[4], ss[4];  // time

    strncpy(yy, date_time, 2);
    strncpy(mm, date_time+2, 2);
    strncpy(dd, date_time+4, 2);

    strncpy(hh, date_time+6, 2);
    strncpy(min, date_time+8, 2);
    strncpy(ss, date_time+10, 2);

    // 1. ascii --> int --> 2 bcd --> 3 RTC에 적용
    ds1302.year = atoi(yy);
    ds1302.month = atoi(mm);
    ds1302.date = atoi(dd);

    ds1302.hours = atoi(hh);
    ds1302.minutes = atoi(min);
    ds1302.seconds = atoi(ss);

    init_ds1302();
}
