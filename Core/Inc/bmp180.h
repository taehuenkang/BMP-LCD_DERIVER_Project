/*
#define BMP_SCL_Pin GPIO_PIN_8
#define BMP_SCL_GPIO_Port GPIOB
#define BMP_SDA_Pin GPIO_PIN_9
#define BMP_SDA_GPIO_Port GPIOB
*/
//main.h


#include <stdio.h>
#include "main.h"
/*
#include "bmp180.h"
#include "main.h"
#include "math.h"

extern I2C_HandleTypeDef hi2c1;  // HAL I2C 핸들러

#define BMP180_ADDR 0xEE  // BMP180 8비트 주소 (7비트 주소는 0x77)

// BMP180 레지스터
#define BMP180_REG_CALIB_START 0xAA  // 보정 데이터 시작 주소
#define BMP180_REG_CONTROL 0xF4
#define BMP180_REG_RESULT 0xF6

// 측정 명령어
#define BMP180_CMD_TEMP 0x2E
#define BMP180_CMD_PRESSURE 0x34

*/


//C code function type:
//1. start signal
//2. Read calibration data from the E2PROM of the BMP180
//			read out E2PROM registers, 16bit, MSB first
//3. read uncompensated temparature value
//			3-1.write 0x2E into regi 0xF4
//			3-2.wait 4.5ms
//			3-3.read reg 0xF6(MSB),0xF7(LSB)
//4.read uncompensated pressure value
//			4-1.write 0x34+(oss << 6 )into reg 0xF4
//			4-2.wait
//			4-3.read reg 0xF6(MSB),0xF7(LSB),0xF8(XLSB)
//			4-4.UP(pressure)=(MSB << 16 + LSB << 8 + XLSB) >> (8-oss)
//5.calculate true temparature
//			5.1 X1=(UT-AC6)*AC5/2^15
//			5.2 X2=MC * 2 ^11 / (X1+MD)
//			5.3 B5= X1+X2
//			5.4 T=(B5+8)/2^4
//6.calculate true pressure
//			6.1 B6 = B5 -4000
//			6.2 X1=(B2*(B6*B6/2^12)/2^11
//			6.3 X2=AC2*B6 / 2 ^11
//   		6.4 X3=
//				....
//7.display temparature and pressure value

#define BMP180_ADDR (0x77 << 1)  // ✅ 0xEE가 자동 처리됨
 // BMP180 8비트 주소 (7비트 주소는 0x77)

// BMP180 레지스터
#define BMP180_REG_CALIB_START 0xAA  // 보정 데이터 시작 주소
#define BMP180_REG_CONTROL 0xF4
#define BMP180_REG_RESULT 0xF6

// 측정 명령어
#define BMP180_CMD_TEMP 0x2E
#define BMP180_CMD_PRESSURE 0x34
//OSS-모드 설정값
#define oss 0  // standard

// BMP180 보정값 저장 변수
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;

//BMP180 중간변수
int32_t B5, B6, X1, X2, X3, B3;
uint32_t B4, B7;


//bmp180.c
uint16_t BMP180_Read16(uint8_t reg);
void BMP180_ReadCalibrationData();
int32_t BMP180_ReadUncompensatedTemperature();
int32_t BMP180_ReadUncompensatedPressure();
float BMP180_CalculateTemperature(int32_t UT);
float BMP180_CalculatePressure(int32_t UP);
void bmp180_test_main(void);
int BMP180_Init(void) ;
