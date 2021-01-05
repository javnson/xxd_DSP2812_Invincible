#include "DSP281x_Device.h"     // DSP281x头文件
#include "DSP281x_Examples.h"   // DSP281x Examples 头文件
#include "config.h"
// EXAM MANUAL

// 开发工具的使用。硬件连接、上电，
// 项目文件的创建、编译，程序的调试（查看某一个内存地址的内容）。（可以使用自己的电脑，运行之后可以去除USB）

//? DSP外设的使用：
//# -> 绿灯
//# -> 红灯
//# -> CPUtimer的使用
//# -> PIE的使用
//# -> SPI的使用（通常可能没有开时钟（程序走不下去）、

// 准备特定的进位程序
// 准备特定占空比的生成代码
// ADC的使用（主要是排序器的应用，如何设置排序器的方式，选择通道、选择复位，启动、等待、读取、右移4位）。

Uint16 Flag_Key;
Uint16 KeyReg1;
Uint16 KeyReg2;
Uint16 LastKey2;
Uint16 LEDReg;
Uint16 led_cmp[8];
int Flag; // 是否得到了输出

// NOTE: ADC部分
unsigned long int  AD1;
unsigned long int AD2;  //实际AD值*1000

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

// NOTE: 共阳字形码0~f, P，－，L，"灭",0.~9.
Uint16 LEDcode[30] = { 0xc000,0xf900,0xA400,0xB000,0x9900,0x9200,0x8200,0xF800,    //0~F
					0x8000,0x9000,0x8800,0x8300,0xc600,0xa100,0x8600,0x8e00,    //
					0x8c00,0xbf00,0xa700,0xff00, // P, -, L, 灭
					0x4000,0x7900,0x2400,0x3000, 0x1900,0x1200,0x0200,0x7800,0x0000,0x1000  // 0~9
};


//////////////////////////////////////////////////////////////////////
// 初始化例程区
// NOTE: 初始化Event GPtimer
void InitEv(void)
{
	// 初始化与GPtimer相关的端口
	GpioMuxRegs.GPAMUX.all = 0x1;	//PWM1 active
	GpioMuxRegs.GPADIR.all = 0x3F;	//GPIOA_3~5
	GpioDataRegs.GPADAT.all = 0x00;

	/////////////////////////////////////////////////////////////////////////////////////
	//初始化EVA寄存器 Timer1
	EvaRegs.EXTCONA.all = 1;		// 三个中断独立操作,下一个操作的先决条件
	EvaRegs.T1CON.all = 0x17CA;		// 1-连续增计数，7-128分频，（C=8(必要值)+4（使能GPtimer）），A允许比较，立即执行
	EvaRegs.COMCONA.all = 0x82e0;	// 允许比较器，打开比较的三个通道（e）
	EvaRegs.ACTRA.all = 0x01;		// 设置将输出结果输出到CMP1，需要初始化GPIOA！
	EvaRegs.DBTCONA.all = 0;		// 对于死区不做处理

	EvaRegs.CMPR1 = led_cmp[0];
	EvaRegs.CMPR2 = led_cmp[1];
	EvaRegs.CMPR3 = led_cmp[2];

#ifndef _CMP_
	EvaRegs.GPTCONA.all = 0x00;		// NOTE:关闭T1CMPOE
#else
	EvaRegs.GPTCONA.all = 0x01;		// NOTE:启动T1CMPOE
	EvaRegs.T1CMPR = led_cmp[3];    // 比较寄存器
#endif

	EvaRegs.T1CNT = 0x0000;		// 计数器清零
	EvaRegs.T1PR = EVPR;		// 设置定时周期

	EvaRegs.EVAIFRA.all = BIT8; // 中断标志清除

	EvaRegs.EVAIMRA.all = 0x018E;		// 启用四个中断T1PINT,CMP1~3,同时启动CMPR
	EvaRegs.EVAIFRA.all = 0;

	/////////////////////////////////////////////////////////////////////////
	// 初始化EVB寄存器 Timer3
	EvbRegs.EXTCONB.all = 1;

	EvbRegs.T3CON.all = 0x17CA;
	EvbRegs.COMCONB.all = 0x82e0;
	EvbRegs.ACTRB.all = 0x01;
	EvbRegs.DBTCONB.all = 0;

	EvbRegs.CMPR4 = led_cmp[4];
	EvbRegs.CMPR5 = led_cmp[5];
	EvbRegs.CMPR6 = led_cmp[6];

#ifndef _CMP_
	EvbRegs.GPTCONB.all = 0x00;		// NOTE:关闭T3CMPOE
#else
	EvbRegs.GPTCONB.all = 0x01;		// note:启动T3CMPOE
	EvbRegs.T3CMPR = led_cmp[7];
#endif

	EvbRegs.T3CNT = 0x0000;
	EvbRegs.T3PR = EVPR;

	EvbRegs.EVBIFRA.all = BIT8; // 中断标志清除

	EvbRegs.EVBIMRA.all = 0x018E;
	EvbRegs.EVBIFRA.all = 0;

}

// NOTE: SPI初始化子程序
void spi_intial()
{
	SpiaRegs.SPICCR.all = 0x0047;                        // 使SPI处于复位方式, 下降沿, 八位数据
	SpiaRegs.SPICTL.all = 0x0006;                        // 主控模式, 一般时钟模式,使能talk, 关闭SPI中断.
	SpiaRegs.SPIBRR = 0x007F;							 // 配置波特率
	SpiaRegs.SPICCR.all = SpiaRegs.SPICCR.all | 0x0080;  // 退出复位状态

	EALLOW;
	GpioMuxRegs.GPFMUX.all |= 0x000F;   // 设置通用引脚为SPI引脚
	EDIS;
}

Uint16 oldBdir;

// NOTE: IO初始化子程序
void gpio_init()
{
	EALLOW;

	//! 绿灯
	GpioMuxRegs.GPFMUX.all = 0x0000;
	GpioMuxRegs.GPFDIR.all = 0xFFFF;    	// upper byte as output/low byte as input

	/////////////////////////////////////////////////////////////////////////
	// NOTE: GPIOE 控制3:8译码器，有效时输出低电平
	// 0,1: KEYA,KEYB -> 键盘
	// 2,3: LEDA,LEDB -> 红灯
	// 4,5: LCD_CSA, LCD_CSB
	//! 3:8译码器
	GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all & 0xfff8; //将GPIOE0~GPIOE2配置为一般I/O口输出,作138译码
	GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all | 0x0007;

	// NOTE: GPB8-GPB15 -> D0 -> D7(顺序增长)
	// 将GPIOB8~GPIOB15配置为一般I/O口,D0~D7
	GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all & 0x00ff;

	///////////////////////////////////////////////////////////////////////////
	// 数码管需要使用SPI驱动串行输出从而实现功能
	//! 数码管支持SPI
	GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11 = 0;     //GPIOA11设置为一般I/O口
	GpioMuxRegs.GPADIR.bit.GPIOA11 = 1;           //


	/// FIXED： 后面的代码已经弃用
	// 将GPIOB8~GPIOB15配置为一般I / O口, D0~D7
	//GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all & 0x00ff;
	//将GPIOB8~GPIOB15配置为输出,D0~D7
	//需要实现红灯操作之前需要先完成下面的操作
	oldBdir = GpioMuxRegs.GPBDIR.all; // 准备好一个buffer可以方便地完成还原赋值
	//GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all | 0xff00;
	// NOTE: GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all | 0xff00;
	// 还原语句
	// NOTE: GpioMuxRegs.GPBDIR.all = oldBdir;

	//将GPIOE0~GPIOE2配置为一般I/O口输出,作138译码
	//GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all & 0xfff8;
	//GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all | 0x0007;

	/// Ev需要配置的

	EDIS;

	GpioDataRegs.GPADAT.bit.GPIOA11 = 0;    // GPIOA11=0; 该端口为74HC595锁存信号
}

////////////////////////////////////////////////////////////////////////
// ADC模块的使用
// NOTE: ADC相关寄存器初始化
void Adc_Init()
{
	// Configure ADC @SYSCLKOUT = 150Mhz
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;      //双序列/级连选择:双序列工作模式
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;     //连续/并发选择:连续采样方式
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;      //启动-停止/连续转换选择:启动-停止方式
	AdcRegs.ADCTRL1.bit.CPS = 1;           //核时钟预定标器:ADC_CLK=ADCLKPS/2=3.125M
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf;      //采集窗口大小:SH pulse/clock=16
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x2;    //核时钟分频:ADCLKPS=HSPCLK/4=6.25M
	AdcRegs.ADCMAXCONV.all = 0x0000;       //转换通道数:SEQ1序列的通道数为1
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xf; //转换通道选择:ADCINB7
}

// NOTE: ADC模块上电延时
void Adc_PowerUP()
{
	long i;

	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;     //ADC带隙和参考电路加电
	for (i = 0; i < 1000000; i++) {}      //至少5ms延时
	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;         //ADC核模拟电路加电
	for (i = 0; i < 10000; i++) {}        //至少20us延时
}

// NOTE：定义ADC扫描的操作
// 扫描得到的数值存在AD1中
// 扫描得到的数值\times1000保存在AD2里面
void ADC_run(void)
{
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;			// 用S/W方式启动SEQ1转换序列

	while (AdcRegs.ADCST.bit.SEQ1_BSY == 1) {}  // 判序列忙否
	AD1 = AdcRegs.ADCRESULT0 >> 4;
	AD2 = (AD1 * 3 * 1000) / 4095;				// 实际AD值*1000
}


/////////////////////////////////////////////////////////////////////
// 这部分代码实现了**红灯**的输出任务
// TODO: 会使用到GPIOE
// 红灯输出
void LedOut(Uint16 led)
{
	int i;

	EALLOW;
	//将GPIOB8~GPIOB15配置为输出,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all | 0xff00;
	EDIS;

	GpioDataRegs.GPEDAT.all = 0xfffb;    //LEDB置0
	GpioDataRegs.GPBDAT.all = ~led;
	for (i = 0; i < DELAY; i++) {}              //延时
	GpioDataRegs.GPEDAT.all = 0xffff;    //锁存高8位
	GpioDataRegs.GPEDAT.all = 0xfffa;    //LEDA置0
	GpioDataRegs.GPBDAT.all = ~(led << 8);
	for (i = 0; i < DELAY; i++) {}
	GpioDataRegs.GPEDAT.all = 0xffff;    //锁存低8位
}

//////////////////////////////////////////////////////////////////////
// 这部分代码实现了刷新**LED**的功能
// FIXED: 这个函数已经弃用
// 通过SPI向LED发送显示数据
void Write_LED(int LEDReg)
{
	SpiaRegs.SPITXBUF = LEDcode[LEDReg];     //给数码管送数
	while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
	SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;  //读出清标志
}

// FIXED: 这个函数已经弃用
// LED显示子程序
void LEDdisplay(int LED8, int LED7, int LED6, int LED5, int LED4, int LED3, int LED2, int LED1)
{
	GpioDataRegs.GPADAT.bit.GPIOA11 = 0; //给LACK信号一个低电平

	Write_LED(LED8);   //向LED最高位写数据,显示LED8
	Write_LED(LED7);
	Write_LED(LED6);
	Write_LED(LED5);
	Write_LED(LED4);
	Write_LED(LED3);
	Write_LED(LED2);
	Write_LED(LED1);   //向LED最低位写数据,显示LED1

	GpioDataRegs.GPADAT.bit.GPIOA11 = 1; //给LACK信号一个高电平为锁存74HC595
}

// NOTE: 刷新显存例程
// 选定一个现存然后显示，这个函数可以尽可能提高显示速度，减少不必要的开支
void refreashLed(int * VideoMem)
{
	int i;
	GpioDataRegs.GPADAT.bit.GPIOA11 = 0; // 给LACK信号一个低电平

	for (i = 7; i > -1; --i)
	{
		// SpiaRegs.SPITXBUF = LEDcode[LEDReg];     //给数码管送数
		SpiaRegs.SPITXBUF = LEDcode[VideoMem[i]];     //给数码管送数
		while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
		SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;  //读出清标志
	}

	GpioDataRegs.GPADAT.bit.GPIOA11 = 1; //给LACK信号一个高电平为锁存74HC595
}


/////////////////////////////////////////////////////////////////////////
// **键盘**例程
// NOTE: 键扫描子程序K1~K8
int Keyscan1(void)
{
	int i;

	EALLOW;
	//将GPIOB8~GPIOB15 配置为输入,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all & 0x00ff;
	EDIS;

	GpioDataRegs.GPEDAT.all = 0xfff8;    // 选通KEY低8位
	for (i = 0; i < DELAY; i++) {}              //延时

	//判K1~K8是否按下
	if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
	{
		for (i = 0; i < DELAY_SHAKE; i++) {}        //延时消抖
		if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
		{
			KeyReg1 = GpioDataRegs.GPBDAT.all; //读键值
			while ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)  //判K1~K8是否松开
			{
				GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
				for (i = 0; i < DELAY_SHAKE; i++) {}
			}
			return (1);
		}
	}
	return (0);
}

// NOTE: 键扫描子程序K9~K16
int Keyscan2(void)
{
	int i;
	EALLOW;
	// 将GPIOB8~GPIOB15配置为输入,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all & 0x00ff;
	EDIS;

	GpioDataRegs.GPEDAT.all = 0xfff9;     // 选通KEY高8位
	for (i = 0; i < DELAY; i++) {}               // 延时

	if ((GpioDataRegs.GPBDAT.all | 0x00ff) == 0xffff)
	{
		for (i = 0; i < DELAY_SHAKE; i++) {}        // 延时消抖
		if ((GpioDataRegs.GPBDAT.all | 0x00ff) == 0xffff)
		{
			Flag_Key = 0;
		}
	}
	// 判K8~K16是否按下
	if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
	{
		for (i = 0; i < DELAY_SHAKE; i++) {}        //延时消抖
		if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
		{
			KeyReg2 = GpioDataRegs.GPBDAT.all;//读键值
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

			/*while ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff) //判K8~K16是否松开
			{
				GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
				for (i=0; i<1000; i++){}
			}
			return (1);*/
		}
	}
	return (0);
}

// NOTE: 键散转子程序K1~K8
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

// NOTE: 键散转子程序K9~K16
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

// NOTE: 读取键盘的输入
// 当输出为真说明键盘读取到了特定的数值
// 数值保存在全局变量LEDReg中
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
// **其他**功能函数
// NOTE: 延时函数
void Delay(Uint16  data)
{
	Uint16  i;
	for (i = 0; i < data; i++) { ; }
}

int hour; // 这组变量用于定义原先的钟表数值
int min;
int sec;

// 时钟计数函数，通过这个函数可以实现按照特定的进制完成进位任务
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
// NOTE:显示函数
// 显示当前的时钟示数
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

// 显示当前的ADC示数
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

// 显示学号，按照格式318-XXXX，后面四位通过参数传入
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
	InitSysCtrl();			// 系统初始化
	gpio_init();            // GPIO初始化子程序
	spi_intial();           // SPI初始化子程序

	GpioDataRegs.GPFDAT.all = 0x3f00;    // GPIOF8~GPIOF13置高电平,指示灯灭
	LedOut(0xFFFF);         // 16个LED都熄灭

	EALLOW;
	SysCtrlRegs.HISPCP.all = 0x3;  //高速外设时钟HSPCLK=SYSCLKOUT/6=25Mhz
	EDIS;

	////////////////////////////////////////////////////////////////////////
	// 外设管理
	DINT;           // 关闭总中断
	IER = 0x0000;   // 关闭外围中断
	IFR = 0x0000;   // 清中断标志

	Adc_PowerUP();
	Adc_Init();

	////////////////////////////////////////////////////////////////////////////////////////
	// 中断管理
	InitPieCtrl();       // 初始化PIE控制寄存器 DSP281x_PieCtrl.c
	InitPieVectTable();  // 初始化PIE矢量表DSP281x_PieVect.c

	InitEv();		// 初始化事件管理器
	Delay(0xFF);

	EALLOW;                                  // 写EALLOW保护寄存器
	PieVectTable.TINT0 = &cpu_timer0_isr;    // PIE中断矢量表
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

	InitCpuTimers();     // 启用CPU Timer0
	ConfigCpuTimer(&CpuTimer0, 150, CPUPR);
	StartCpuTimer0();

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;		// 使能CPU-Timer0

	PieCtrlRegs.PIEIER2.bit.INTx5 = 1;		// T1C
	PieCtrlRegs.PIEIER2.bit.INTx3 = 1;		// 使能CMP3
	PieCtrlRegs.PIEIER2.bit.INTx2 = 1;		// 使能CMP2
	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;		// 使能CMP1

	PieCtrlRegs.PIEIER4.bit.INTx5 = 1;      // T3C
	PieCtrlRegs.PIEIER4.bit.INTx3 = 1;		// 使能CMP6
	PieCtrlRegs.PIEIER4.bit.INTx2 = 1;		// 使能CMP5
	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;		// 使能CMP4

#ifdef _CMP_
	PieCtrlRegs.PIEIER2.bit.INTx4 = 1;		// 使能T1PINT
	PieCtrlRegs.PIEIER4.bit.INTx4 = 1;		// 使能T3PINT
#endif

	IER |= (M_INT1 | M_INT2 | M_INT4);		// 使能中断
	// 1-系统时钟，2对应于EVATimer1，4对应EVBTimer3


	//////////////////////////////////////////////////////////////////
	// 低功耗模式
#ifdef _IDEL_
	SysCtrlRegs.LPMCR0.all = 0x0; // 设定低功耗模式为Idel
#endif
	///////////////////////////////////////////////////////////////////
	// 全局变量初始化段
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
	// 准备进入主循环
	EINT;   // 开放全局中断
	ERTM;   // Enable Global realtime interrupt DBGM

	for (;;)
	{
		loop(); // 执行主循环

#ifdef _IDEL
		asm(" IDLE"); // 低功耗循环
#endif
	}
}

