//###########################################################################
//
// FILE:	DSP281x_PieCtrl.c
//
// TITLE:	DSP281x Device PIE Control Register Initialization Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Added ENPIE = 0 to the IntPieCtrl function
//      |             |      | Removed ENPIE = 1 from the IntPieCtrl function
//      |             |      | Created EnableInterrupts function
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

//---------------------------------------------------------------------------
// InitPieCtrl: 
//---------------------------------------------------------------------------
// This function initializes the PIE control registers to a known state.
//初始化PIE控制寄存器
void InitPieCtrl(void)
{
    // Disable Interrupts at the CPU level:
    //关掉CPU内核中断
    DINT;

    // Disable the PIE
    //关掉PIE模块
    PieCtrlRegs.PIECRTL.bit.ENPIE = 0;

	// Clear all PIEIER registers:
	//清除所有的PIE中断使能寄存器
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;	
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	// Clear all PIEIFR registers:
	//清除所有的PIE中断标志位
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;	
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;


}	

//---------------------------------------------------------------------------
// EnableInterrupts: 
//---------------------------------------------------------------------------
// This function enables the PIE module and CPU interrupts
//使能PIE模块和CPU中断
void EnableInterrupts()
{

    // Enable the PIE
    //使能PIE模块
    PieCtrlRegs.PIECRTL.bit.ENPIE = 1;
    		
	// Enables PIE to drive a pulse into the CPU 
	//使能PIE模块驱动一个脉冲到CPU内核
	PieCtrlRegs.PIEACK.all = 0xFFFF;  

	// Enable Interrupts at the CPU level 
	//使能CPU内核中断
    EINT;

}


//===========================================================================
// No more.
//===========================================================================
