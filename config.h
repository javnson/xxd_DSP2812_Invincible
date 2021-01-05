// ���ļ������޸ĸ���ʱ������

// NOTE ��ʱ�����޸Ĵ˴�
// Ƶ��75/128MHz,20ms��Ӧ11719-1,100����20ms���ܼ�ʱ��Ϊ2s��Ϊ���Ҳ�����
// ����1ms��Ƶ�ʣ�ʹ��586-1�����ڲ��ٴη�Ƶ���Դﵽ20ms��ϵ����
#define EVPR (640-1) 
// �趨�ж�����Ϊ0.1sһ�Σ����ڸ�����ʾʾ��us
#define CPUPR (10000)
// �趨���ӵ���ʾʱ��
#define AlermPeriod (10)
// �趨ADC����������ʾ�������ֵ
#define ADCLowest (10)


// �޸�ʱ�ӵĽ���
#define hour_set (24)
#define minute_set (30)
#define second_set (20)

// NOTE:�������Է���ؼ���ÿһ���Ƶ�ֵ
// GpioDataRegs.GPFDAT.all &= LED(j);	// ����j��
// GpioDataRegs.GPFDAT.all |= ~LED(j);	// Ϩ��j��
#define LED(x) (0xFEFF << ((x) - 1))
#define GreenLedOn(x)   GpioDataRegs.GPFDAT.all &= (x)
#define GreenLedOff(x)	GpioDataRegs.GPFDAT.all |= ~(x)

// NOTE:�����595оƬ������ʱ�����ʱ
//#define DELAY 100
#define DELAY (10)

// NOTE: ���尴��������ʱ�����ʱ
#define DELAY_SHAKE (1000)
// NOTE: ����ADC�����ֵ
#define ADC_MAX (0xFFF) // ����3Vʱ��ֵ

#define _CMP_ // �˴�Ϊ����

//������Ӧֵ
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
extern unsigned long int AD2;  //ʵ��ADֵ*1000
extern int Flag; // �����Ƿ�õ������
extern int hour; // ����������ڶ���ԭ�ȵ��ӱ���ֵ
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
