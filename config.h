// 本文件用于修改各种时钟周期

// NOTE 定时周期修改此处
// 频率75/128MHz,20ms对应11719-1,100个点20ms，总计时长为2s，为正弦波周期
// 对于1ms的频率，使用586-1，在内部再次分频可以达到20ms的系数。
#define EVPR (640-1) 
// 设定中断周期为0.1s一次，用于更新显示示数us
#define CPUPR (10000)
// 设定闹钟的提示时长
#define AlermPeriod (10)
// 设定ADC可以正常显示的最低阈值
#define ADCLowest (10)


// 修改时钟的进制
#define hour_set (24)
#define minute_set (30)
#define second_set (20)

// NOTE:这个宏可以方便地计算每一个灯的值
// GpioDataRegs.GPFDAT.all &= LED(j);	// 点亮j灯
// GpioDataRegs.GPFDAT.all |= ~LED(j);	// 熄灭j灯
#define LED(x) (0xFEFF << ((x) - 1))
#define GreenLedOn(x)   GpioDataRegs.GPFDAT.all &= (x)
#define GreenLedOff(x)	GpioDataRegs.GPFDAT.all |= ~(x)

// NOTE:定义和595芯片交流的时候的延时
//#define DELAY 100
#define DELAY (10)

// NOTE: 定义按键消抖的时候的延时
#define DELAY_SHAKE (1000)
// NOTE: 定义ADC的最大值
#define ADC_MAX (0xFFF) // 这是3V时的值

#define _CMP_ // 此处为启用

//按键对应值
#define   K1        0xfeff
#define   K2        0xfdff
#define   K3        0xfbff
#define   K4        0xf7ff
#define   K5        0xefff
#define   K6        0xdfff
#define   K7        0xbfff
#define   K8        0x7fff
#define   K9        0xfeff
#define   K10       0xfdff
#define   K11       0xfbff
#define   K12       0xf7ff
#define   K13       0xefff
#define   K14       0xdfff
#define   K15       0xbfff
#define   K16       0x7fff

extern int led_tab[100];
extern Uint16 Flag_Key;
extern Uint16 KeyReg1;
extern Uint16 KeyReg2;
extern Uint16 LastKey2;
extern Uint16 LEDReg;
extern Uint16 led_cmp[8];
extern unsigned long int  AD1;
extern unsigned long int AD2;  //实际AD值*1000
extern int Flag; // 键盘是否得到了输出
extern int hour; // 这组变量用于定义原先的钟表数值
extern int min;
extern int sec;
extern int vm[8];

interrupt void cpu_timer0_isr(void);
interrupt void eva_timer1_isr(void);
interrupt void evb_timer3_isr(void);

interrupt void eva_CMP1INT_isr(void);
interrupt void eva_CMP2INT_isr(void);
interrupt void eva_CMP3INT_isr(void);
interrupt void evb_CMP4INT_isr(void);
interrupt void evb_CMP5INT_isr(void);
interrupt void evb_CMP6INT_isr(void);

interrupt void evb_timer3_cnt_isr(void);
interrupt void eva_timer1_cnt_isr(void);

void loop(void);
void init_var(void);

void refreashLed(int * VideoMem);
void LEDdisplay(int LED8, int LED7, int LED6, int LED5, int LED4, int LED3, int LED2, int LED1);
void Write_LED(int LEDReg);
void LedOut(Uint16 led);

void gpio_init();
void spi_intial();
void InitEv(void);

void Adc_Init();
void ADC_run(void);
void Adc_PowerUP();

int Keyscan1(void);
int Keyscan2(void);
void KeyFunction1(Uint16 KeyReg1);
void KeyFunction2(Uint16 KeyReg2);
int GetKey();
void Delay(Uint16  data);
void StepSec(void);

void DisplayClock(void);
void DisplayADC(void);
void DisplayID(int id);
