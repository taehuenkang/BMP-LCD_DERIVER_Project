# BMP-LCD_DERIVER_Project
STM32_liveCoding
- **데이터시트 기반의 로우레벨 드라이버 설계 능력** 향상
- 보정 수식, 통신 타이밍, 레지스터 구조 등을 직접 구현하여 **하드웨어-소프트웨어 연결 고리 학습**

| 항목 | 내용 |
| --- | --- |
| 프로젝트명 | 1602 LCD 4비트 제어 드라이버 개발 |
| 개발 환경 | STM32F4, HAL Library |
| 인터페이스 | 4-bit Parallel (GPIO 직접 제어) |
| 동작 모드 | 4-bit, 2-line, 5x7 dot |
| 개발 방식 | LCD 데이터시트 기반의 초기화 시퀀스 및 통신 직접 구현 |
| 주요 기능 | 문자열 출력, 커서 제어, 화면 초기화 등 |

| 항목 | 내용 |
| --- | --- |
| 프로젝트명 | BMP180 기압 및 온도 센서 드라이버 개발 |
| 개발 환경 | STM32 HAL (CubeIDE), I²C 통신 |
| 대상 센서 | BMP180 (Bosch) |
| 통신 방식 | I²C (7-bit 주소: 0x77 / 8-bit 주소: 0xEE) |
| 사용 언어 | C |
| 주요 기능 | 보정 데이터 추출, 온도 및 압력 계산, 실시간 표시 함수 구현 |




---

## 🌡️ `BMP_LCD_Driver_Project`

```markdown
# 🌡️ BMP180 + LCD1602 Driver – STM32 HAL 기반 드라이버 설계

An embedded system that reads real-time temperature/pressure from BMP180 and displays it on a 1602 LCD.

---

## 🔧 Setup Overview

| Component  | Detail                        |
|------------|-------------------------------|
| MCU        | STM32F4                       |
| Sensor     | BMP180 (I2C)                  |
| Display    | LCD1602 (HD44780, 4bit GPIO)  |
| Interface  | I2C, GPIO, UART               |
| Toolchain  | STM32CubeIDE + HAL            |
| Language   | C                             |

---

## 📦 Features

- EEPROM 보정값 추출 → Bosch 공식 기반 연산
- 4bit LCD 초기화 시퀀스 직접 구현 (EN, RS, D4~D7)
- UART로 디버깅 메시지 출력
- Logic Analyzer로 타이밍 검증

---
📸 Display Example
makefile
복사
편집
Temp: 22.4 °C
Press: 958.81 hPa
🛠️ Build & Flash
Open project in STM32CubeIDE

Flash to STM32F4 board

Use UART monitor (baud 9600) to check logs


## 🧪 Data Flow

```txt
BMP180 → I2C Read → 보정식 적용 → LCD 출력 (1초 간격)
                         ↓
                    UART Debug Print
