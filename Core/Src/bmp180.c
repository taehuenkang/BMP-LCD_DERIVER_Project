#include <bmp180.h>
#include <stdio.h>
#include "main.h"
#include "math.h"
#include "main.h"
#include "stm32f4xx_hal_conf.h"  //

extern I2C_HandleTypeDef hi2c1;  //

int I2C_ReadRegister(uint8_t devAddr, uint8_t regAddr, uint8_t* data) {
    // BMP180 주소는 8비트지만 HAL 함수에선 7비트 주소 사용함 -> 0x77
    // devAddr가 0xEE인 경우, 실제 주소는 (0xEE >> 1) = 0x77
    if (HAL_I2C_Mem_Read(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY) != HAL_OK) {
        return -1;  // 실패
    }
    return 0;  // 성공
}
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


int BMP180_Init(void) {
    uint8_t id = 0;
    //check-init
    if (I2C_ReadRegister(BMP180_ADDR, 0xD0, &id) != 0) {
        printf("I2C 통신 실패!\n");
        return -1;
    }

    printf("센서 ID: 0x%X\n", id);
    //check-init_id
    if (id != 0x55) {
        printf("센서 ID 불일치 (기대값: 0x55)\n");
        return -1;
    }

    // 보정 계수 읽기
    BMP180_ReadCalibrationData();

    // 중간 변수 초기화
    B5 = 0;
    B6 = X1 = X2 = X3 = B3 = 0;
    B4 = B7 = 0;

    return 0;
}


//read 16bit and return to buffer
uint16_t BMP180_Read16(uint8_t reg)
{
	uint8_t data[2];
	//                hi2c     bmp reg주소 memregister,memregister size, data, datasize, timeout
	HAL_I2C_Mem_Read(&hi2c1, BMP180_ADDR, reg, 1, data, 2, 100);
	//MSB      LSB return 해서 측정
	//data[0]  data[1]
	return (data[0] << 8) | data[1];
}
//2. Read calibration data from the E2PROM of the BMP180
// 16bit calibration data 11개의
void BMP180_ReadCalibrationData()
{
    AC1 = (int16_t)BMP180_Read16(0xAA);
    AC2 = (int16_t)BMP180_Read16(0xAC);
    AC3 = (int16_t)BMP180_Read16(0xAE);
    AC4 = BMP180_Read16(0xB0);
    AC5 = BMP180_Read16(0xB2);
    AC6 = BMP180_Read16(0xB4);
    B1  = (int16_t)BMP180_Read16(0xB6);
    B2  = (int16_t)BMP180_Read16(0xB8);
    MB  = (int16_t)BMP180_Read16(0xBA);
    MC  = (int16_t)BMP180_Read16(0xBC);
    MD  = (int16_t)BMP180_Read16(0xBE);

    // calibration check
    printf("보정값 확인:\n");
    printf("AC1=%d AC2=%d AC3=%d AC4=%u AC5=%u AC6=%u\n", AC1, AC2, AC3, AC4, AC5, AC6);
    printf("B1=%d B2=%d MB=%d MC=%d MD=%d\n", B1, B2, MB, MC, MD);
}


//3. read uncompensated temparature value
int32_t BMP180_ReadUncompensatedTemperature()
{

    uint8_t cmd = BMP180_CMD_TEMP;
    //3-1.write 0x2E(reg_temp) into regi 0xF4(reg_control)
    HAL_I2C_Mem_Write(&hi2c1, BMP180_ADDR, BMP180_REG_CONTROL, 1, &cmd, 1, 100);
    //3-2.wait 4.5ms 여유를 줘서 5ms
    HAL_Delay(5);
    //3-3.read reg 0xF6(MSB),0xF7(LSB)
    return BMP180_Read16(BMP180_REG_RESULT);
}

//4.read uncompensated pressure value
int32_t BMP180_ReadUncompensatedPressure()
{
	//4-1.write 0x34(BMP180_CMD_PRESSURE)+(oss << 6 )into reg 0xF4
    uint8_t cmd = BMP180_CMD_PRESSURE;
    HAL_I2C_Mem_Write(&hi2c1, BMP180_ADDR, BMP180_REG_CONTROL, 1, &cmd, 1, 100);

    //4-2.wait
    HAL_Delay(8);  // 8ms 대기
    // MSB        LSB      XLSB
    // data[0]    data[1]  data[2]
    uint8_t data[3];
    //4-3.read reg 0xF6(MSB),0xF7(LSB),0xF8(XLSB)
    HAL_I2C_Mem_Read(&hi2c1, BMP180_ADDR, BMP180_REG_RESULT, 1, data, 3, 100);
    //4-4.UP(pressure)=(MSB << 16 + LSB << 8 + XLSB) >> (8-oss)
    return ((data[0] << 16) | (data[1] << 8) | data[2]) >> (8 - oss);
}

//5.calculate true temparature
float BMP180_CalculateTemperature(int32_t UT)
{
	//5.1 X1=(UT-AC6)*AC5/2^15
    int32_t X1 = (UT - AC6) * AC5 / (1 << 15);
    //5.2 X2=MC * 2 ^11 / (X1+MD)
    int32_t X2 = (MC * (1 << 11)) / (X1 + MD);
    //5.3 B5= X1+X2
    int32_t B5 = X1 + X2;
    //5.4 T=(B5+8)/2^4---->float형으로 정확히 판단하기 위해 /10.0
    return (B5 + 8) / 16.0 / 10.0;
}

//6.calculate true pressure
//			6.1 B6 = B5 -4000
//			6.2 X1=(B2*(B6*B6/2^12)/2^11
//			6.3 X2=AC2*B6 / 2 ^11
//   		6.4 X3=
//				....


float BMP180_CalculatePressure(int32_t UP)
{
    // 6.1 B6 = B5 - 4000
    int32_t B6 = B5 - 4000;

    // 6.2 X1 = (B2 * (B6 * B6 / 2^12)) / 2^11
    int32_t X1 = (B2 * ((B6 * B6) >> 12)) >> 11;

    // 6.3 X2 = (AC2 * B6) / 2^11
    int32_t X2 = (AC2 * B6) >> 11;

    // 6.4 X3 = ((X1 + X2) + 2) / 2^2
    int32_t X3 = ((X1 + X2) + 2) >> 2;

    // 6.5 B3 = (((AC1 * 4 + X3) << oss) + 2) / 4
    int32_t B3 = ((((int32_t)AC1 * 4 + X3) << oss) + 2) >> 2;

    // 6.6 X1 = (AC3 * B6) / 2^13
    X1 = (AC3 * B6) >> 13;

    // 6.7 X2 = (B1 * (B6 * B6 / 2^12)) / 2^16
    X2 = (B1 * ((B6 * B6) >> 12)) >> 16;

    // 6.8 X3 = ((X1 + X2) + 2) / 2^2
    X3 = ((X1 + X2) + 2) >> 2;

    // 6.9 B4 = (AC4 * (X3 + 32768)) / 2^15
    uint32_t B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;

    // 6.10 B7 = ((unsigned long)UP - B3) * (50000 >> oss)
    uint32_t B7 = ((uint32_t)UP - B3) * (50000 >> oss);

    // 6.11 Pressure calculation
    //pressure
    int32_t p;
    if (B7 < 0x80000000)
    {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }

    // 6.12 X1 = (p / 2^8) * (p / 2^8)
    X1 = (p >> 8) * (p >> 8);

    // 6.13 X1 = (X1 * 3038) / 2^16
    X1 = (X1 * 3038) >> 16;

    // 6.14 X2 = (-7357 * p) / 2^16
    X2 = (-7357 * p) >> 16;

    // 6.15 p = p + (X1 + X2 + 3791) / 2^4
    p = p + ((X1 + X2 + 3791) >> 4);

    return (float)p;  // 최종 압력 값 반환
}

void bmp180_test_main(void)
{
    HAL_Init();

    // 1. 보정 값 확인
    if (BMP180_Init() == 0)
    {
        printf("BMP180 init 성공!\n");

/*        printf("AC1: %d, AC2: %d, AC3: %d\n", AC1, AC2, AC3);
        printf("AC4: %u, AC5: %u, AC6: %u\n", AC4, AC5, AC6);
        printf("B1: %d, B2: %d\n", B1, B2);
        printf("MB: %d, MC: %d, MD: %d\n", MB, MC, MD);*/
    }
    else
    {
        printf("BMP180 init 실패! 프로그램 종료.\n");
        while (1); // 오류 발생 시 멈춤
    }

    while (1)
    {

        int32_t UT = BMP180_ReadUncompensatedTemperature();
        int32_t UP = BMP180_ReadUncompensatedPressure();
        //uncompensated check
        printf("UT(raw): %ld, UP(raw): %ld\n", UT, UP);

        // 3. 계산된 온도/기압
        float temp = BMP180_CalculateTemperature(UT);
        int32_t pressure = BMP180_CalculatePressure(UP);

        printf("Temperature: %.2f C\n", temp);
        printf("Pressure: %.2f hPa\n", pressure / 100.0f);
        //uncompensated check
        HAL_Delay(1000);
    }
}
// 오류확인
void I2C_Scan(void)
{
    printf("Scanning I2C devices...\n");
    for (uint8_t i = 1; i < 128; i++)
    {
        if (HAL_I2C_IsDeviceReady(&hi2c1, (i << 1), 3, 5) == HAL_OK)
        {
            printf("Device address: 0x%02X\n", i);
        }
    }
}



