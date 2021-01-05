//###########################################################################
//
// FILE:	DSP281x_CpuTimers.c
//
// TITLE:	DSP281x CPU 32-bit Timers Initialization & Support Functions.
//
// NOTES:   CpuTimer1 and CpuTimer2 are reserved for use with DSP BIOS and
//          other realtime operating systems.  
//
//          Do not use these two timers in your application if you ever plan
//          on integrating DSP-BIOS or another realtime OS. 
//
//          For this reason, the code to manipulate these two timers is
//          commented out and not used in these examples.
//           
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H  | Changes since previous version (v.58 Alpha)
//      |             |      | Removed some incorrect parameters in the timer
//      |             |      | setup that are not available on this device
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

struct CPUTIMER_VARS CpuTimer0;

// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
//struct CPUTIMER_VARS CpuTimer1;
//struct CPUTIMER_VARS CpuTimer2;

//---------------------------------------------------------------------------
// InitCpuTimers: 
//---------------------------------------------------------------------------
// This function initializes all three CPU timers to a known state.
//初始化CPU定时器
void InitCpuTimers(void)
{
    // CPU Timer 0
    //定时器0
	// Initialize address pointers to respective timer registers:
	//寄存器地址分配
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	// Initialize timer period to maximum:
	//初始化周期寄存器为最大值0xFFFFFFFF	
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):	
	//设定预定标系数为1，CPU计数时钟为SYSCLKOUT，与主频一致。
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	//停止定时器
	CpuTimer0Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	//将周期寄存器中的值0xFFFFFFFF装载到计数寄存器中。
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	//复位定时器中断次数为0
	CpuTimer0.InterruptCount = 0;	             	
	
	
// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
// Do not use these two timers if you ever plan on integrating 
// DSP-BIOS or another realtime OS. 
//
// For this reason, the code to manipulate these two timers is
// commented out and not used in these examples.

    // Initialize address pointers to respective timer registers:
//	CpuTimer1.RegsAddr = &CpuTimer1Regs;
//	CpuTimer2.RegsAddr = &CpuTimer2Regs;
	// Initialize timer period to maximum:
//	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
//	CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
	// Make sure timers are stopped:
//	CpuTimer1Regs.TCR.bit.TSS = 1;             
//	CpuTimer2Regs.TCR.bit.TSS = 1;             
	// Reload all counter register with period value:
//	CpuTimer1Regs.TCR.bit.TRB = 1;             
//	CpuTimer2Regs.TCR.bit.TRB = 1;             
	// Reset interrupt counters:
//	CpuTimer1.InterruptCount = 0;
//	CpuTimer2.InterruptCount = 0;

}	
	
//---------------------------------------------------------------------------
// ConfigCpuTimer: 
//---------------------------------------------------------------------------
// This function initializes the selected timer to the period specified
// by the "Freq" and "Period" parameters. The "Freq" is entered as "MHz"
// and the period in "uSeconds". The timer is held in the stopped state
// after configuration.
//配置CPU定时器的周期和频率
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;
	
	// Initialize timer period:	
	//设定定时器的周期
	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;

	// Set pre-scale counter to divide by 1 (SYSCLKOUT):	
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;
	
	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer 
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 1;
	Timer->RegsAddr->TCR.bit.FREE = 1;     // Timer Free Run
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
	
	// Reset interrupt counter:
	Timer->InterruptCount = 0;
}

//===========================================================================
// No more.
//===========================================================================
