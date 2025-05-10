#include "keypad.h"
#include "main.h"
#include "extern.h"
#include <stdlib.h>
GPIO_TypeDef* keypadRowPort[4] = {GPIOC, GPIOC, GPIOC, GPIOC}; //R1~R4 행
GPIO_TypeDef* keypadColPort[4] = {GPIOC, GPIOC, GPIOC, GPIOC}; //C1~C4 열
uint16_t keypadRowPin[4] = {ROW1_KEYPAD_Pin, ROW2_KEYPAD_Pin, ROW3_KEYPAD_Pin, ROW4_KEYPAD_Pin}; //R1~R4 GPIO Input & Pull-up으로 설정을 해야 한다.
uint16_t keypadColPin[4] = {COL1_KEYPAD_Pin, COL2_KEYPAD_Pin, COL3_KEYPAD_Pin, COL4_KEYPAD_Pin}; //C1~C4  GPIO Output

void keypadInit()
{
	for(uint8_t col = 0; col < 4; col++)
	{
		HAL_GPIO_WritePin(keypadColPort[col], keypadColPin[col], SET); //초기 값 1로 셋팅
	}
}

uint8_t getKeypadState(uint8_t col, uint8_t row)
{
#if 0
	uint8_t keypadChar[4][4] = {
			{'/', '3', '2', '1'}, //  keypadChar[col][row];
			{'*', '6', '5', '4'},
			{'-', '9', '8', '7'},
			{'+', '=', '0', ' '},
	};

#else
	/*
	uint8_t keypadChar[4][4] = {
			{'1', '2', '3', '/'}, // keypadChar[0][0] keypadChar[0][1] keypadChar[0][2] keypadChar[0][3]
			{'4', '5', '6', '*'},
			{'7', '8', '9', '-'},
			{' ', '0', '=', '+'},
	};
	*/
	uint8_t keypadChar[4][4] = {
			{'1', '4', '7', ' '}, // keypadChar[0][0] keypadChar[0][1] keypadChar[0][2] keypadChar[0][3]
			{'2', '5', '8', '0'},
			{'3', '6', '9', '='},
			{'/', '*', '-', '+'},
	};
#endif
	static uint8_t prevState[4][4] = {
			{1, 1, 1, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1},
	};
	uint8_t curState = 1;

	HAL_GPIO_WritePin(keypadColPort[col], keypadColPin[col], RESET);
	curState = HAL_GPIO_ReadPin(keypadRowPort[row], keypadRowPin[row]);

	HAL_GPIO_WritePin(keypadColPort[col], keypadColPin[col], SET);

	if(curState == PUSHED && prevState[col][row] == RELEASED)
	{
		prevState[col][row] = curState;
		return 0;
	}
	else if (curState == RELEASED && prevState[col][row] == PUSHED)
	{
		prevState[col][row] = curState;
		return keypadChar[col][row];
	}
	return 0;
}

uint8_t keypadScan()
{
	uint8_t data;

	for(uint8_t col=0; col<4; col++)
	{
		for(uint8_t row=0; row<4; row++)
		{
			data = getKeypadState(col, row);
			if(data != 0)
			{
				return data;
			}
		}
	}
	return 0;
}

int key_cal(void)
{
	// 버튼을 누르면 숫자, 기호가 큐에 들어감
	// 숫자가 눌리면 num에 저장이 되도록함.
	// 기호가 눌리면 operator에 저장이 되도록함.
	// = 을 누르면 그동안의 결과를 출력하도록 함.
	// key value 에는 현재 눌린 키가 무조건 저장됨.

	char cal_num[4] = {0,}; //
	static uint8_t count = 0;
	int num1, num2;
	uint8_t key_value;

	if(!queue_empty()) // queue에 값이 들어오면 4개의 값을 채움
	{
		// key_value = read_queue();
		// cal_num[count] = key_value;
		cal_num[count] = read_queue();
		//printf("%c\n",key_value);
		printf("cal_num[%d] = %c\n",count,cal_num[count]);
		count++;
		//printf("%d\n",count);
		//printf("%c\n",cal_num[1]);

		if(count==4) // 4개의 입력값이 모두 다 들어오면
		{
			count = 0;
			num1 = atoi(cal_num[0]);
			num2 = atoi(cal_num[2]);
			printf("num1 : %d\n",num1);
			printf("num2 : %d\n",num2);
			switch(cal_num[1]+'0') 	// 큐에 들어오는 값 예시 '1' '+' '1' '=' 4개의 값으로 계산해야함
								// 2번째는 무조건 연산자
			{
				case '+' : printf("num1 + num2 = %d\n", num1 + num2);
					break;

				case '-' : printf("num1 - num2 = %d\n", num1 - num2);
					break;

				case '*' : printf("num1 * num2 = %d\n", num1 * num2);
					break;

				case '/' : printf("num1 / num2 = %d\n", num1 / num2);
					break;

				default :
					break;
			}
		}
	}

}
