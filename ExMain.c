#include "DSP281x_Device.h"     // DSP281x头文件
#include "DSP281x_Examples.h"   // DSP281x Examples 头文件
#include "config.h"

///////////////////////////////////////////////////////////////////////
// NOTE: 在这里定义全局变量


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

////////////////////////////////////////////////////////////////////////
// NOTE: 需要在中断中完成的代码
interrupt void cpu_timer0_isr(void)
{
	// TODO: 写必要的代码在这里
	StepSec(); // 测试进位功能

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;// 清中断应答信号，准备接收下一次中断
}

interrupt void eva_timer1_isr(void)
{
	// TODO: 写必要的代码在这里

	// 恢复中断
	EvaRegs.EVAIFRA.all = BIT7; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // 允许下次中断
}

// 在合适的时候调整载波周期
interrupt void evb_timer3_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvbRegs.EVBIFRA.all = BIT7; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // 允许下次中断
}

interrupt void eva_CMP1INT_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvaRegs.EVAIFRA.all = BIT1; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // 允许下次中断
}

interrupt void eva_CMP2INT_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvaRegs.EVAIFRA.all = BIT2; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // 允许下次中断
}

interrupt void eva_CMP3INT_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvaRegs.EVAIFRA.all = BIT3; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // 允许下次中断
}

#ifdef _CMP_
interrupt void eva_timer1_cnt_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvaRegs.EVAIFRA.all = BIT8; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // 允许下次中断
}
#endif

interrupt void evb_CMP4INT_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvbRegs.EVBIFRA.all = BIT1; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // 允许下次中断
}

interrupt void evb_CMP5INT_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvbRegs.EVBIFRA.all = BIT2; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // 允许下次中断
}

interrupt void evb_CMP6INT_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvbRegs.EVBIFRA.all = BIT3; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // 允许下次中断
}

#ifdef _CMP_
interrupt void evb_timer3_cnt_isr(void)
{
	// TODO: 写必要的代码在这里


	// 恢复中断
	EvbRegs.EVBIFRA.all = BIT8; // 中断标志清
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // 允许下次中断
}
#endif 
