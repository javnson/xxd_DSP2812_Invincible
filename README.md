# xxd_DSP2812_Invincible
徐老师的DSP2812期末考试准备代码，这段代码实现了所有的常用功能的初始化功能，并提供了类似于Adurino的接口方便同学们快速Invicible。

**感谢大家的支持，祝大家满绩！**

下面给出了使用方法，请花费10min仔细阅读，并完成工程的建立

完成工程建立之后，没有接触过 **额丢一诺**（张大哥说的，狗头）

## 建立工程

使用CCS v7建立CCS工程选用器件DSP320F2812，时间紧张不过多赘述。

建立Target Config File

删除新建工程自动生成的源文件(.c)和链接命令文件(.cmd)。

  

至此全部准备工作完成。

+ 拷贝必要的文件 

请将MAIN分支所有内容全部下载，并拷贝到新建的工程目录下，全部文件的列表如下所示：

> 2010/10/19  20:53             4,953 DSP281x_CpuTimers.c
> 2010/10/19  20:53            25,974 DSP281x_DefaultIsr.c
> 2010/09/27  12:38             6,980 DSP281x_GlobalVariableDefs.c
> 2010/09/27  12:38             4,973 DSP281x_Headers_nonBIOS.cmd
> 2010/10/19  20:53             1,920 DSP281x_InitPeripherals.c
> 2010/10/19  20:53             3,039 DSP281x_PieCtrl.c
> 2010/10/19  20:53             5,911 DSP281x_PieVect.c
> 2010/10/19  20:53             8,098 DSP281x_SysCtrl.c
> 2020/12/29  17:27            16,210 Exam_pre_1.c
> 2020/12/29  17:25             3,511 ExMain.c
> 2010/09/27  12:38             4,727 F2812_EzDSP_RAM_lnk.cmd

+ 完成include路径配置

在CCS工程目录处右键-属性-Include File Path 选择默认给出的headers文件夹目录。

## 快速入门

### 快速配置

请打开文```config.h```文件，需要关注的内容如下：

``` C++
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

// NOTE:定义和595芯片交流的时候的延时
//#define DELAY 100
#define DELAY (10)

// NOTE: 定义按键消抖的时候的延时
#define DELAY_SHAKE (1000)

// NOTE: 定义ADC的最大值
#define ADC_MAX (0xFFF) // 这是3V时的值

#define _CMP_ // 此处为启用
```

注意：

> 请根据需要调整CPU的时钟周期，单位为us，根据需要设定时钟的进制，根据需要设定整周器中断的中断值，算了我还是不复读了。。。
>
> 详细内容注释已经有非常明确清晰的交代了

### 开始测试

之后打开文件```ExMain.c```文件，可以看到里面有数个函数，以下分别讲解

+ 第一部分：定义全局变量

``` C++
///////////////////////////////////////////////////////////////////////
// NOTE: 在这里定义全局变量

```

算了，我不复读了

+ 第二部分：完成全局变量的初始化过程，以及自己的初始化过程

``` C++
///////////////////////////////////////////////////////////////////////
// NOTE: 用于初始化全局变量
void init_var(void)
{
	// 显示一个学号先
	DisplayID(5060);

    // Green Led Test
    GreenLedOn(LED(1));
    GreenLedOn(LED(2));
    GreenLedOn(LED(3));
    GreenLedOn(LED(4));
    GreenLedOn(LED(5));
    GreenLedOn(LED(6));
    GreenLedOff(LED(6));
    GreenLedOff(LED(5));
    GreenLedOff(LED(4));
    GreenLedOff(LED(3));
    GreenLedOff(LED(2));
    GreenLedOff(LED(1));

	// 整合版本的亮灯程序
	GreenLedOn(LED(1) & LED(2) & LED(3) & LED(4));
	GreenLedOff(LED(1) & LED(2) & LED(3) & LED(4));

    // 红灯
    LedOut(0xF0F0);
    LedOut(0x0F0F);

}
```

上面代码给出了几个示例，可以通过在CCS中通过断点查看上面代码的效果，上面的代码已经于上周完成了测试工作。

函数意义如同字面意义，&翻译成中文”和“即可。

注意，绿灯现在的标号是从左向右从1到7.

红灯点亮函数每一位代表一个灯。

+ 第三部分：完成主循环中必须完成的代码

``` C++
// NOTE: 需要在主循环中完成的代码
void loop(void)
{
	if (GetKey()) // 键盘
	{
		// 在这里判定按键功能
		LedOut(LEDReg);
	}

	ADC_run();
	DisplayADC(); // 显示ADC的示数

	vm[7] = hour / 10;
	vm[6] = hour % 10;
	vm[5] = 0x11;
	vm[4] = min / 10;
	vm[3] = min % 10;
	vm[2] = 0x11;
	vm[1] = sec / 10;
	vm[0] = sec % 10;

	refreashLed(vm);
	// 上面功能可以通过下面的函数实现：
	DisplayClock();

}

```

理论上，需要在主循环完成Key的检测和LED的显示刷新（需要写大程序的时候需要将LED的刷新程序移动到CPU timer的中断中，

关于上面的代码的使用方法可以参考后面的函数说明。

+ 第四部分：在CPU timer中实现CPUtimer中需要实现的功能（记得在config中配置时钟鸭！）

``` C++
interrupt void cpu_timer0_isr(void)
{
	// TODO: 写必要的代码在这里
	StepSec(); // 测试进位功能

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;// 清中断应答信号，准备接收下一次中断
}

```

需要在CPU TImer中执行的代码可以放在这一部分，这里只是简单的完成一次进位的工作。

+ 第五部分（附加部分）：完成整周器中断和比较器中断

这一部分考试的时候99.99%用不到，所以此处不做过多解释，有需要的同学自助观看。

## 库函数说明文档

下面给出了需要使用的库函数的说明，需要考试的同学有字面意义就够了~

### 灯光控制系列

#### 绿灯

``` C++
// NOTE:这个宏可以方便地计算每一个灯的值
// GpioDataRegs.GPFDAT.all &= LED(j);	// 点亮j灯
// GpioDataRegs.GPFDAT.all |= ~LED(j);	// 熄灭j灯
#define LED(x) (0xFEFF << ((x) - 1))
#define GreenLedOn(x)   GpioDataRegs.GPFDAT.all &= (x)
#define GreenLedOff(x)	GpioDataRegs.GPFDAT.all |= ~(x)
```

上面代码给出了实现方式，下面的代码演示了如何使用：

``` C++
// 逐个操作：注意灯的编号是1~6（不是从0开始的，而且是人类的从左向右的顺序，不是板子上阴间的从右向左顺序！！！！）
	GreenLedOn(LED(1)); // 点亮左边第一盏灯
	GreenLedOff(LED(1)); // 灭掉第一盏灯
// 整体操作：一次控制4盏灯
	GreenLedOn(LED(1) & LED(2) & LED(3) & LED(4));
	GreenLedOff(LED(1) & LED(2) & LED(3) & LED(4));
```

#### 红灯

```C++
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

```

上面的代码给出了具体的实现，Examer就别操心了~关心下面的测试代码更要紧

``` C++
  LedOut(0xF0F0); // 1111 0000 1111 0000
  LedOut(0x0F0F); // 0000 1111 0000 1111
```

0就是灭1就是亮，大家试试看就知道了，不会设断点就写在loop里面康康。~~~

#### 数码管控制

``` C++
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
```

基于SPI的数码管控制，原理不详。反正就是传入一个8个int长度的数组它就会自动编码显示就对了！

下面是编码原文，懂的兄弟可以直接填对应的编号，不太明白的兄弟数数，从0开始，数到多少就会显示多少~~~

``` C++
// NOTE: 共阳字形码0~f, P，－，L，"灭",0.~9.
Uint16 LEDcode[30] = { 0xc000,0xf900,0xA400,0xB000,0x9900,0x9200,0x8200,0xF800,    //0~F
					0x8000,0x9000,0x8800,0x8300,0xc600,0xa100,0x8600,0x8e00,    //
					0x8c00,0xbf00,0xa700,0xff00, // P, -, L, 灭
					0x4000,0x7900,0x2400,0x3000, 0x1900,0x1200,0x0200,0x7800,0x0000,0x1000  // 0~9
};
```

程序中给出了几个可以直接使用的函数

``` C++
// 显示时钟
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

```

上面代码的注释其实蛮清楚的~

可以在主程序中直接使用，在使用样例中也有范例。

### ADC

别的不多说，就下面这个函数

``` C++
void ADC_run(void)
{
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;			// 用S/W方式启动SEQ1转换序列

	while (AdcRegs.ADCST.bit.SEQ1_BSY == 1) {}  // 判序列忙否
	AD1 = AdcRegs.ADCRESULT0 >> 4;
	AD2 = (AD1 * 3 * 1000) / 4095;				// 实际AD值*1000
}
```

每执行一次就会刷新一组```AD1```和```AD2```两个全局变量的值，不知道是什么？看下面：

``` C++
// NOTE: ADC部分
unsigned long int AD1;
unsigned long int AD2;  //实际AD值*1000
```

### 键盘功能

``` C++
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
```

这个函数返回的值代表是否读取到了键，有就是1，没有就是0，读取的值放在了全局变量``` LEDReg```中。

为什么是这么阴间的名字，因为这是老师的代码封装了一下，还没来得及~~（尴尬）有空欢迎大家branch~

怎么用？

``` C++
	if (GetKey()) // 键盘
	{
		// 在这里判定按键功能
		LedOut(LEDReg); // 这一行可以换成具体的功能
	}
```

上面的代码示例了基本的使用方法，可以在if里面通过switch判定是不是有什么按键按下了，并响应它。

### 辅助函数

还有点点功能需要赶紧说说

#### Delay

想让学号显示一阵阵，之后永远消失，可以使用delay函数，这个函数可以让程序”暂停“一阵阵（中断还是正常的啊）。让学号多待会儿~

下面是函数原型

``` C++
void Delay(Uint16  data) 
{
	Uint16  i;
	for (i = 0; i < data; i++) { ; }
}
```

#### StepSec

为了方便让大家能够快速实现钟表功能，有如下的定义

``` C++
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
```

上面三个是全局变量，可以在你的程序中直接访问。

**进制**是可以通过**宏**配置的~显示功能参考**数码管控制**章节

# 最重要，看这里！

首先，祝大家**满绩**！

其次，最终要的当然是**广告**啦~

狗头（汪汪汪！）

欢迎大家**打赏**，

! [欢迎打赏](https://github.com/javnson/xxd_DSP2812_Invincible/blob/main/%E6%AC%A2%E8%BF%8E%E6%89%93%E8%B5%8F.jpg)

俺要放一张大大的码在这里！









欢迎联系我交流经验修改bug，via github， via V chat and QQ(3141037347,请备注姓名~) 







寒假有兴趣一起做小C++项目的兄弟，求戳，求戳爆我。

主要实现的功能是一个绘图软件（什么绘图软件呢？）

这个问题问得好！但是，俺要上考场了，所以~~~先不说啦~
