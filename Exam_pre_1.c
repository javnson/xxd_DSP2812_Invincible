#include "DSP281x_Device.h"     // DSP281xͷ�ļ�
#include "DSP281x_Examples.h"   // DSP281x Examples ͷ�ļ�
#include "config.h"
// EXAM MANUAL

// �������ߵ�ʹ�á�Ӳ�����ӡ��ϵ磬
// ��Ŀ�ļ��Ĵ��������룬����ĵ��ԣ��鿴ĳһ���ڴ��ַ�����ݣ���������ʹ���Լ��ĵ��ԣ�����֮�����ȥ��USB��

//? DSP�����ʹ�ã�
//# -> �̵�
//# -> ���
//# -> CPUtimer��ʹ��
//# -> PIE��ʹ��
//# -> SPI��ʹ�ã�ͨ������û�п�ʱ�ӣ������߲���ȥ����

// ׼���ض��Ľ�λ����
// ׼���ض�ռ�ձȵ����ɴ���
// ADC��ʹ�ã���Ҫ����������Ӧ�ã���������������ķ�ʽ��ѡ��ͨ����ѡ��λ���������ȴ�����ȡ������4λ����

Uint16 Flag_Key;
Uint16 KeyReg1;
Uint16 KeyReg2;
Uint16 LastKey2;
Uint16 LEDReg;
Uint16 led_cmp[8];
int Flag; // �Ƿ�õ������

// NOTE: ADC����
unsigned long int  AD1;
unsigned long int AD2;  //ʵ��ADֵ*1000

int vm[8];

int led_tab[100] =
{
587,585,583,580,576,571,565,558,549,540,
530,518,506,493,479,465,449,434,417,400,
383,365,347,329,311,293,274,256,238,220,
202,185,168,151,136,120,106,92 ,79 ,67 ,
55 ,45 ,36 ,27 ,20 ,14 ,9  ,5  ,2  ,2  ,
2  ,2  ,2  ,5  ,9  ,14 ,20 ,27 ,36 ,45 ,
55 ,67 ,79 ,92 ,106,120,136,151,168,185,
202,220,238,256,274,292,311,329,347,365,
383,400,417,434,449,465,479,493,506,518,
530,540,549,558,565,571,576,580,583,585
};

// NOTE: ����������0~f, P������L��"��",0.~9.
Uint16 LEDcode[30] = { 0xc000,0xf900,0xA400,0xB000,0x9900,0x9200,0x8200,0xF800,    //0~F
					0x8000,0x9000,0x8800,0x8300,0xc600,0xa100,0x8600,0x8e00,    //
					0x8c00,0xbf00,0xa700,0xff00, // P, -, L, ��
					0x4000,0x7900,0x2400,0x3000, 0x1900,0x1200,0x0200,0x7800,0x0000,0x1000  // 0~9
};


//////////////////////////////////////////////////////////////////////
// ��ʼ��������
// NOTE: ��ʼ��Event GPtimer
void InitEv(void)
{
	// ��ʼ����GPtimer��صĶ˿�
	GpioMuxRegs.GPAMUX.all = 0x1;	//PWM1 active
	GpioMuxRegs.GPADIR.all = 0x3F;	//GPIOA_3~5
	GpioDataRegs.GPADAT.all = 0x00;

	/////////////////////////////////////////////////////////////////////////////////////
	//��ʼ��EVA�Ĵ��� Timer1
	EvaRegs.EXTCONA.all = 1;		// �����ж϶�������,��һ���������Ⱦ�����
	EvaRegs.T1CON.all = 0x17CA;		// 1-������������7-128��Ƶ����C=8(��Ҫֵ)+4��ʹ��GPtimer������A����Ƚϣ�����ִ��
	EvaRegs.COMCONA.all = 0x82e0;	// ����Ƚ������򿪱Ƚϵ�����ͨ����e��
	EvaRegs.ACTRA.all = 0x01;		// ���ý������������CMP1����Ҫ��ʼ��GPIOA��
	EvaRegs.DBTCONA.all = 0;		// ����������������

	EvaRegs.CMPR1 = led_cmp[0];
	EvaRegs.CMPR2 = led_cmp[1];
	EvaRegs.CMPR3 = led_cmp[2];

#ifndef _CMP_
	EvaRegs.GPTCONA.all = 0x00;		// NOTE:�ر�T1CMPOE
#else
	EvaRegs.GPTCONA.all = 0x01;		// NOTE:����T1CMPOE
	EvaRegs.T1CMPR = led_cmp[3];    // �ȽϼĴ���
#endif

	EvaRegs.T1CNT = 0x0000;		// ����������
	EvaRegs.T1PR = EVPR;		// ���ö�ʱ����

	EvaRegs.EVAIFRA.all = BIT8; // �жϱ�־���

	EvaRegs.EVAIMRA.all = 0x018E;		// �����ĸ��ж�T1PINT,CMP1~3,ͬʱ����CMPR
	EvaRegs.EVAIFRA.all = 0;

	/////////////////////////////////////////////////////////////////////////
	// ��ʼ��EVB�Ĵ��� Timer3
	EvbRegs.EXTCONB.all = 1;

	EvbRegs.T3CON.all = 0x17CA;
	EvbRegs.COMCONB.all = 0x82e0;
	EvbRegs.ACTRB.all = 0x01;
	EvbRegs.DBTCONB.all = 0;

	EvbRegs.CMPR4 = led_cmp[4];
	EvbRegs.CMPR5 = led_cmp[5];
	EvbRegs.CMPR6 = led_cmp[6];

#ifndef _CMP_
	EvbRegs.GPTCONB.all = 0x00;		// NOTE:�ر�T3CMPOE
#else
	EvbRegs.GPTCONB.all = 0x01;		// note:����T3CMPOE
	EvbRegs.T3CMPR = led_cmp[7];
#endif

	EvbRegs.T3CNT = 0x0000;
	EvbRegs.T3PR = EVPR;

	EvbRegs.EVBIFRA.all = BIT8; // �жϱ�־���

	EvbRegs.EVBIMRA.all = 0x018E;
	EvbRegs.EVBIFRA.all = 0;

}

// NOTE: SPI��ʼ���ӳ���
void spi_intial()
{
	SpiaRegs.SPICCR.all = 0x0047;                        // ʹSPI���ڸ�λ��ʽ, �½���, ��λ����
	SpiaRegs.SPICTL.all = 0x0006;                        // ����ģʽ, һ��ʱ��ģʽ,ʹ��talk, �ر�SPI�ж�.
	SpiaRegs.SPIBRR = 0x007F;							 // ���ò�����
	SpiaRegs.SPICCR.all = SpiaRegs.SPICCR.all | 0x0080;  // �˳���λ״̬

	EALLOW;
	GpioMuxRegs.GPFMUX.all |= 0x000F;   // ����ͨ������ΪSPI����
	EDIS;
}

Uint16 oldBdir;

// NOTE: IO��ʼ���ӳ���
void gpio_init()
{
	EALLOW;

	//! �̵�
	GpioMuxRegs.GPFMUX.all = 0x0000;
	GpioMuxRegs.GPFDIR.all = 0xFFFF;    	// upper byte as output/low byte as input

	/////////////////////////////////////////////////////////////////////////
	// NOTE: GPIOE ����3:8����������Чʱ����͵�ƽ
	// 0,1: KEYA,KEYB -> ����
	// 2,3: LEDA,LEDB -> ���
	// 4,5: LCD_CSA, LCD_CSB
	//! 3:8������
	GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all & 0xfff8; //��GPIOE0~GPIOE2����Ϊһ��I/O�����,��138����
	GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all | 0x0007;

	// NOTE: GPB8-GPB15 -> D0 -> D7(˳������)
	// ��GPIOB8~GPIOB15����Ϊһ��I/O��,D0~D7
	GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all & 0x00ff;

	///////////////////////////////////////////////////////////////////////////
	// �������Ҫʹ��SPI������������Ӷ�ʵ�ֹ���
	//! �����֧��SPI
	GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11 = 0;     //GPIOA11����Ϊһ��I/O��
	GpioMuxRegs.GPADIR.bit.GPIOA11 = 1;           //


	/// FIXED�� ����Ĵ����Ѿ�����
	// ��GPIOB8~GPIOB15����Ϊһ��I / O��, D0~D7
	//GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all & 0x00ff;
	//��GPIOB8~GPIOB15����Ϊ���,D0~D7
	//��Ҫʵ�ֺ�Ʋ���֮ǰ��Ҫ���������Ĳ���
	oldBdir = GpioMuxRegs.GPBDIR.all; // ׼����һ��buffer���Է������ɻ�ԭ��ֵ
	//GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all | 0xff00;
	// NOTE: GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all | 0xff00;
	// ��ԭ���
	// NOTE: GpioMuxRegs.GPBDIR.all = oldBdir;

	//��GPIOE0~GPIOE2����Ϊһ��I/O�����,��138����
	//GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all & 0xfff8;
	//GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all | 0x0007;

	/// Ev��Ҫ���õ�

	EDIS;

	GpioDataRegs.GPADAT.bit.GPIOA11 = 0;    // GPIOA11=0; �ö˿�Ϊ74HC595�����ź�
}

////////////////////////////////////////////////////////////////////////
// ADCģ���ʹ��
// NOTE: ADC��ؼĴ�����ʼ��
void Adc_Init()
{
	// Configure ADC @SYSCLKOUT = 150Mhz
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;      //˫����/����ѡ��:˫���й���ģʽ
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;     //����/����ѡ��:����������ʽ
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;      //����-ֹͣ/����ת��ѡ��:����-ֹͣ��ʽ
	AdcRegs.ADCTRL1.bit.CPS = 1;           //��ʱ��Ԥ������:ADC_CLK=ADCLKPS/2=3.125M
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf;      //�ɼ����ڴ�С:SH pulse/clock=16
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x2;    //��ʱ�ӷ�Ƶ:ADCLKPS=HSPCLK/4=6.25M
	AdcRegs.ADCMAXCONV.all = 0x0000;       //ת��ͨ����:SEQ1���е�ͨ����Ϊ1
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xf; //ת��ͨ��ѡ��:ADCINB7
}

// NOTE: ADCģ���ϵ���ʱ
void Adc_PowerUP()
{
	long i;

	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;     //ADC��϶�Ͳο���·�ӵ�
	for (i = 0; i < 1000000; i++) {}      //����5ms��ʱ
	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;         //ADC��ģ���·�ӵ�
	for (i = 0; i < 10000; i++) {}        //����20us��ʱ
}

// NOTE������ADCɨ��Ĳ���
// ɨ��õ�����ֵ����AD1��
// ɨ��õ�����ֵ\times1000������AD2����
void ADC_run(void)
{
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;			// ��S/W��ʽ����SEQ1ת������

	while (AdcRegs.ADCST.bit.SEQ1_BSY == 1) {}  // ������æ��
	AD1 = AdcRegs.ADCRESULT0 >> 4;
	AD2 = (AD1 * 3 * 1000) / 4095;				// ʵ��ADֵ*1000
}


/////////////////////////////////////////////////////////////////////
// �ⲿ�ִ���ʵ����**���**���������
// TODO: ��ʹ�õ�GPIOE
// ������
void LedOut(Uint16 led)
{
	int i;

	EALLOW;
	//��GPIOB8~GPIOB15����Ϊ���,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all | 0xff00;
	EDIS;

	GpioDataRegs.GPEDAT.all = 0xfffb;    //LEDB��0
	GpioDataRegs.GPBDAT.all = ~led;
	for (i = 0; i < DELAY; i++) {}              //��ʱ
	GpioDataRegs.GPEDAT.all = 0xffff;    //�����8λ
	GpioDataRegs.GPEDAT.all = 0xfffa;    //LEDA��0
	GpioDataRegs.GPBDAT.all = ~(led << 8);
	for (i = 0; i < DELAY; i++) {}
	GpioDataRegs.GPEDAT.all = 0xffff;    //�����8λ
}

//////////////////////////////////////////////////////////////////////
// �ⲿ�ִ���ʵ����ˢ��**LED**�Ĺ���
// FIXED: ��������Ѿ�����
// ͨ��SPI��LED������ʾ����
void Write_LED(int LEDReg)
{
	SpiaRegs.SPITXBUF = LEDcode[LEDReg];     //�����������
	while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
	SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;  //�������־
}

// FIXED: ��������Ѿ�����
// LED��ʾ�ӳ���
void LEDdisplay(int LED8, int LED7, int LED6, int LED5, int LED4, int LED3, int LED2, int LED1)
{
	GpioDataRegs.GPADAT.bit.GPIOA11 = 0; //��LACK�ź�һ���͵�ƽ

	Write_LED(LED8);   //��LED���λд����,��ʾLED8
	Write_LED(LED7);
	Write_LED(LED6);
	Write_LED(LED5);
	Write_LED(LED4);
	Write_LED(LED3);
	Write_LED(LED2);
	Write_LED(LED1);   //��LED���λд����,��ʾLED1

	GpioDataRegs.GPADAT.bit.GPIOA11 = 1; //��LACK�ź�һ���ߵ�ƽΪ����74HC595
}

// NOTE: ˢ���Դ�����
// ѡ��һ���ִ�Ȼ����ʾ������������Ծ����������ʾ�ٶȣ����ٲ���Ҫ�Ŀ�֧
void refreashLed(int * VideoMem)
{
	int i;
	GpioDataRegs.GPADAT.bit.GPIOA11 = 0; // ��LACK�ź�һ���͵�ƽ

	for (i = 7; i > -1; --i)
	{
		// SpiaRegs.SPITXBUF = LEDcode[LEDReg];     //�����������
		SpiaRegs.SPITXBUF = LEDcode[VideoMem[i]];     //�����������
		while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
		SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;  //�������־
	}

	GpioDataRegs.GPADAT.bit.GPIOA11 = 1; //��LACK�ź�һ���ߵ�ƽΪ����74HC595
}


/////////////////////////////////////////////////////////////////////////
// **����**����
// NOTE: ��ɨ���ӳ���K1~K8
int Keyscan1(void)
{
	int i;

	EALLOW;
	//��GPIOB8~GPIOB15 ����Ϊ����,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all & 0x00ff;
	EDIS;

	GpioDataRegs.GPEDAT.all = 0xfff8;    // ѡͨKEY��8λ
	for (i = 0; i < DELAY; i++) {}              //��ʱ

	//��K1~K8�Ƿ���
	if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
	{
		for (i = 0; i < DELAY_SHAKE; i++) {}        //��ʱ����
		if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
		{
			KeyReg1 = GpioDataRegs.GPBDAT.all; //����ֵ
			while ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)  //��K1~K8�Ƿ��ɿ�
			{
				GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
				for (i = 0; i < DELAY_SHAKE; i++) {}
			}
			return (1);
		}
	}
	return (0);
}

// NOTE: ��ɨ���ӳ���K9~K16
int Keyscan2(void)
{
	int i;
	EALLOW;
	// ��GPIOB8~GPIOB15����Ϊ����,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all & 0x00ff;
	EDIS;

	GpioDataRegs.GPEDAT.all = 0xfff9;     // ѡͨKEY��8λ
	for (i = 0; i < DELAY; i++) {}               // ��ʱ

	if ((GpioDataRegs.GPBDAT.all | 0x00ff) == 0xffff)
	{
		for (i = 0; i < DELAY_SHAKE; i++) {}        // ��ʱ����
		if ((GpioDataRegs.GPBDAT.all | 0x00ff) == 0xffff)
		{
			Flag_Key = 0;
		}
	}
	// ��K8~K16�Ƿ���
	if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
	{
		for (i = 0; i < DELAY_SHAKE; i++) {}        //��ʱ����
		if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
		{
			KeyReg2 = GpioDataRegs.GPBDAT.all;//����ֵ
			if (Flag_Key == 0)
			{
				Flag_Key = 1;
				LastKey2 = KeyReg2;
				return (1);
			}
			if (Flag_Key == 1 && KeyReg2 == LastKey2)
				return (0);
			else
			{
				LastKey2 = KeyReg2;
				return (1);
			}

			/*while ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff) //��K8~K16�Ƿ��ɿ�
			{
				GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
				for (i=0; i<1000; i++){}
			}
			return (1);*/
		}
	}
	return (0);
}

// NOTE: ��ɢת�ӳ���K1~K8
void KeyFunction1(Uint16 KeyReg1)
{
	switch (KeyReg1)
	{
	case  K1: {
		LEDReg = 0x00;
	}
			  break;
	case  K2: {
		LEDReg = 0x01;
	}
			  break;
	case  K3: {
		LEDReg = 0x02;
	}
			  break;
	case  K4: {
		LEDReg = 0x03;
	}
			  break;
	case  K5: {
		LEDReg = 0x04;
	}
			  break;
	case  K6: {
		LEDReg = 0x05;
	}
			  break;
	case  K7: {
		LEDReg = 0x06;
	}
			  break;
	case  K8: {
		LEDReg = 0x07;
	}
			  break;
	default:
		break;
	}
}

// NOTE: ��ɢת�ӳ���K9~K16
void KeyFunction2(Uint16 KeyReg2)
{
	switch (KeyReg2)
	{
	case  K9: {
		LEDReg = 0x08;
	}
			  break;
	case  K10: {
		LEDReg = 0x09;
	}
			   break;
	case  K11: {
		LEDReg = 0x0A;
	}
			   break;
	case  K12: {
		LEDReg = 0x0B;
	}
			   break;
	case  K13: {
		LEDReg = 0x0C;
	}
			   break;
	case  K14: {
		LEDReg = 0x0D;
	}
			   break;
	case  K15: {
		LEDReg = 0x0E;
	}
			   break;
	case  K16: {
		LEDReg = 0x0F;
	}
			   break;
	default:
		break;
	}
}

// NOTE: ��ȡ���̵�����
// �����Ϊ��˵�����̶�ȡ�����ض�����ֵ
// ��ֵ������ȫ�ֱ���LEDReg��
int GetKey()
{
	if (Keyscan2() == 1)
	{
		KeyFunction2(KeyReg2);
		Flag = 1;
	}
	else if (Keyscan1() == 1)
	{
		KeyFunction1(KeyReg1);
		Flag = 1;
	}
	else
		Flag = 0;

	return Flag;
}

//////////////////////////////////////////////////////////////////////////
// **����**���ܺ���
// NOTE: ��ʱ����
void Delay(Uint16  data)
{
	Uint16  i;
	for (i = 0; i < data; i++) { ; }
}

int hour; // ����������ڶ���ԭ�ȵ��ӱ���ֵ
int min;
int sec;

// ʱ�Ӽ���������ͨ�������������ʵ�ְ����ض��Ľ�����ɽ�λ����
void StepSec(void)
{
	if (sec == second_set - 1)
	{
		if (min == minute_set - 1)
		{
			if (hour == hour_set - 1)
			{
				hour = 0;
				min = 0;
				sec = 0;
			}
			else
			{
				hour += 1;
				min = 0;
				sec = 0;

			}
		}
		else
		{
			min += 1;
			sec = 0;
		}
	}
	else
		sec += 1;
}

/////////////////////////////////////////////////////////////////////////
// NOTE:��ʾ����
// ��ʾ��ǰ��ʱ��ʾ��
void DisplayClock(void)
{
	vm[7] = hour / 10;
	vm[6] = hour % 10;
	vm[5] = 0x11;
	vm[4] = min / 10;
	vm[3] = min % 10;
	vm[2] = 0x11;
	vm[1] = sec / 10;
	vm[0] = sec % 10;

	refreashLed(vm);
}

// ��ʾ��ǰ��ADCʾ��
void DisplayADC(void)
{
	LEDdisplay((AD2 / 1000) + 20, (AD2 % 1000) / 100, (AD2 % 100) / 10, 19, AD1 / 1000, (AD1 % 1000) / 100, (AD1 % 100) / 10, AD1 % 10);

	vm[7] = (AD2 / 1000) + 20;
	vm[6] = (AD2 % 1000) / 100;
	vm[5] = (AD2 % 100) / 10;
	vm[4] = 19;
	vm[3] = AD1 / 1000;
	vm[2] = (AD1 % 1000) / 100;
	vm[1] = (AD1 % 100) / 10;
	vm[0] = AD1 % 10;

	refreashLed(vm);
}

// ��ʾѧ�ţ����ո�ʽ318-XXXX��������λͨ����������
void DisplayID(int id)
{
	vm[7] = 3;
	vm[6] = 1;
	vm[5] = 8;
	vm[4] = 0x11;// -
	vm[3] = id / 1000;
	vm[2] = (id % 1000) / 100;
	vm[1] = (id % 100) / 10;
	vm[0] = id % 10;

	refreashLed(vm);
}



void main(void)
{
	InitSysCtrl();			// ϵͳ��ʼ��
	gpio_init();            // GPIO��ʼ���ӳ���
	spi_intial();           // SPI��ʼ���ӳ���

	GpioDataRegs.GPFDAT.all = 0x3f00;    // GPIOF8~GPIOF13�øߵ�ƽ,ָʾ����
	LedOut(0xFFFF);         // 16��LED��Ϩ��

	EALLOW;
	SysCtrlRegs.HISPCP.all = 0x3;  //��������ʱ��HSPCLK=SYSCLKOUT/6=25Mhz
	EDIS;

	////////////////////////////////////////////////////////////////////////
	// �������
	DINT;           // �ر����ж�
	IER = 0x0000;   // �ر���Χ�ж�
	IFR = 0x0000;   // ���жϱ�־

	Adc_PowerUP();
	Adc_Init();

	////////////////////////////////////////////////////////////////////////////////////////
	// �жϹ���
	InitPieCtrl();       // ��ʼ��PIE���ƼĴ��� DSP281x_PieCtrl.c
	InitPieVectTable();  // ��ʼ��PIEʸ����DSP281x_PieVect.c

	InitEv();		// ��ʼ���¼�������
	Delay(0xFF);

	EALLOW;                                  // дEALLOW�����Ĵ���
	PieVectTable.TINT0 = &cpu_timer0_isr;    // PIE�ж�ʸ����
	PieVectTable.T1PINT = &eva_timer1_isr;
	PieVectTable.CMP1INT = &eva_CMP1INT_isr;
	PieVectTable.CMP2INT = &eva_CMP2INT_isr;
	PieVectTable.CMP3INT = &eva_CMP3INT_isr;
	PieVectTable.T3PINT = &evb_timer3_isr;
	PieVectTable.CMP4INT = &evb_CMP4INT_isr;
	PieVectTable.CMP5INT = &evb_CMP5INT_isr;
	PieVectTable.CMP6INT = &evb_CMP6INT_isr;

#ifdef _CMP_
	PieVectTable.T1CINT = &eva_timer1_cnt_isr;
	PieVectTable.T3CINT = &evb_timer3_cnt_isr;
#endif

	EDIS;

	InitCpuTimers();     // ����CPU Timer0
	ConfigCpuTimer(&CpuTimer0, 150, CPUPR);
	StartCpuTimer0();

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;		// ʹ��CPU-Timer0

	PieCtrlRegs.PIEIER2.bit.INTx5 = 1;		// T1C
	PieCtrlRegs.PIEIER2.bit.INTx3 = 1;		// ʹ��CMP3
	PieCtrlRegs.PIEIER2.bit.INTx2 = 1;		// ʹ��CMP2
	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;		// ʹ��CMP1

	PieCtrlRegs.PIEIER4.bit.INTx5 = 1;      // T3C
	PieCtrlRegs.PIEIER4.bit.INTx3 = 1;		// ʹ��CMP6
	PieCtrlRegs.PIEIER4.bit.INTx2 = 1;		// ʹ��CMP5
	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;		// ʹ��CMP4

#ifdef _CMP_
	PieCtrlRegs.PIEIER2.bit.INTx4 = 1;		// ʹ��T1PINT
	PieCtrlRegs.PIEIER4.bit.INTx4 = 1;		// ʹ��T3PINT
#endif

	IER |= (M_INT1 | M_INT2 | M_INT4);		// ʹ���ж�
	// 1-ϵͳʱ�ӣ�2��Ӧ��EVATimer1��4��ӦEVBTimer3


	//////////////////////////////////////////////////////////////////
	// �͹���ģʽ
#ifdef _IDEL_
	SysCtrlRegs.LPMCR0.all = 0x0; // �趨�͹���ģʽΪIdel
#endif
	///////////////////////////////////////////////////////////////////
	// ȫ�ֱ�����ʼ����
	Flag = 0;
	hour = 0;
	min = 0;
	sec = 0;
	Flag_Key =0;
	KeyReg1	 =0;
	KeyReg2	 =0;
	LastKey2 =0;
	LEDReg	 =0;

	init_var();

	///////////////////////////////////////////////////////////
	// ׼��������ѭ��
	EINT;   // ����ȫ���ж�
	ERTM;   // Enable Global realtime interrupt DBGM

	for (;;)
	{
		loop(); // ִ����ѭ��

#ifdef _IDEL
		asm(" IDLE"); // �͹���ѭ��
#endif
	}
}

