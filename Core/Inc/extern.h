
extern uint8_t rx_data;   // uart2 rx byte
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

extern volatile int TIM11_1ms_counter;
extern t_print o_prt;

extern void led_main(void);
extern void button_led_toggle_test(void);
extern void ds1302_main(void);
extern void pc_command_processing(void);
extern void set_rtc(char *date_time);
extern void delay_us(int us);
extern void flash_main();
extern void flash_set_time(void);
extern int get_button( GPIO_TypeDef *GPIO, int GPIO_Pin, int button_num);

extern void dotmatrix_main_test();
extern int dotmatrix_main(void);
extern int dotmatrix_main_func(void);

extern void i2c_lcd_main(void);
extern void i2c_lcd_init(void);
extern void lcd_string(uint8_t *str);
extern void move_cursor(uint8_t row, uint8_t column);
extern void button_pull_up(void);
extern void i2c_lcd_init(void);
extern void buzzer_main();

extern int dotmatrix_name_main(void);
#define BTN_PUPDR 0x4002080C
#define BTN_IDR 0x40020810

extern void servo_motor_main(void);

extern void keypadInit();
extern uint8_t keypadScan();

extern void insert_queue(unsigned char value);
extern unsigned char read_queue();
extern int queue_empty(void);
extern int queue_full(void);
extern int key_cal(void);

//bmp180.c
extern void bmp180_test_main(void);
extern void I2C_Scan(void);


//1602.lcd.c
extern void lcd_1602_main(void);
