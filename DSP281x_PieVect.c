//###########################################################################
//
// FILE:	DSP281x_PieVect.c
//
// TITLE:	DSP281x Devices PIE Vector Table Initialization Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Changed USER0-USER11 to USER1-USER12 to match the CPU guide
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

//const struct PIE_VECT_TABLE PieVectTableInit = {
  const struct PIE_VECT_TABLE PieVectTableInit = {
      PIE_RESERVED,  // Reserved space
      PIE_RESERVED,  //保留 
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   
      PIE_RESERVED,   


// Non-Peripheral Interrupts
//非外设中断
      INT13_ISR,     // XINT13 or CPU-Timer 1
      INT14_ISR,     // CPU-Timer2
      DATALOG_ISR,   // Datalogging interrupt
      RTOSINT_ISR,   // RTOS interrupt
      EMUINT_ISR,    // Emulation interrupt
      NMI_ISR,       // Non-maskable interrupt
      ILLEGAL_ISR,   // Illegal operation TRAP
      USER1_ISR,     // User Defined trap 1
      USER2_ISR,     // User Defined trap 2
      USER3_ISR,     // User Defined trap 3
      USER4_ISR,     // User Defined trap 4
      USER5_ISR,     // User Defined trap 5
      USER6_ISR,     // User Defined trap 6
      USER7_ISR,     // User Defined trap 7
      USER8_ISR,     // User Defined trap 8
      USER9_ISR,     // User Defined trap 9
      USER10_ISR,    // User Defined trap 10
      USER11_ISR,    // User Defined trap 11
      USER12_ISR,     // User Defined trap 12

// Group 1 PIE Vectors
//第1组PIE中断
      PDPINTA_ISR,   // EV-A
      PDPINTB_ISR,   // EV-B
      rsvd_ISR,
      XINT1_ISR,     
      XINT2_ISR,
      ADCINT_ISR,    // ADC
      TINT0_ISR,     // Timer 0
      WAKEINT_ISR,   // WD

// Group 2 PIE Vectors
//第2组PIE中断
      CMP1INT_ISR,   // EV-A
      CMP2INT_ISR,   // EV-A
      CMP3INT_ISR,   // EV-A
      T1PINT_ISR,    // EV-A
      T1CINT_ISR,    // EV-A
      T1UFINT_ISR,   // EV-A
      T1OFINT_ISR,   // EV-A
      rsvd_ISR,
      
// Group 3 PIE Vectors
//第3组PIE中断
      T2PINT_ISR,    // EV-A
      T2CINT_ISR,    // EV-A
      T2UFINT_ISR,   // EV-A
      T2OFINT_ISR,   // EV-A
      CAPINT1_ISR,   // EV-A
      CAPINT2_ISR,   // EV-A
      CAPINT3_ISR,   // EV-A
      rsvd_ISR,
      
// Group 4 PIE Vectors
//第4组PIE中断
      CMP4INT_ISR,   // EV-B
      CMP5INT_ISR,   // EV-B
      CMP6INT_ISR,   // EV-B
      T3PINT_ISR,    // EV-B
      T3CINT_ISR,    // EV-B
      T3UFINT_ISR,   // EV-B
      T3OFINT_ISR,   // EV-B
      rsvd_ISR,      
     
// Group 5 PIE Vectors
//第5组PIE中断
      T4PINT_ISR,    // EV-B
      T4CINT_ISR,    // EV-B
      T4UFINT_ISR,   // EV-B
      T4OFINT_ISR,   // EV-B
      CAPINT4_ISR,   // EV-B
      CAPINT5_ISR,   // EV-B
      CAPINT6_ISR,   // EV-B
      rsvd_ISR,      

// Group 6 PIE Vectors
//第6组PIE中断
      SPIRXINTA_ISR,   // SPI-A
      SPITXINTA_ISR,   // SPI-A
      rsvd_ISR,
      rsvd_ISR,
      MRINTA_ISR,    // McBSP-A
      MXINTA_ISR,    // McBSP-A
      rsvd_ISR,
      rsvd_ISR,
      
// Group 7 PIE Vectors
//第7组PIE中断
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   

// Group 8 PIE Vectors
//第8组PIE中断
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      
// Group 9 PIE Vectors 
//第9组PIE中断    
      SCIRXINTA_ISR, // SCI-A
      SCITXINTA_ISR, // SCI-A
      SCIRXINTB_ISR, // SCI-B
      SCITXINTB_ISR, // SCI-B
      ECAN0INTA_ISR, // eCAN
      ECAN1INTA_ISR, // eCAN
      rsvd_ISR,   
      rsvd_ISR,   
      
// Group 10 PIE Vectors
//第10组PIE中断
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
            
// Group 11 PIE Vectors
//第11组PIE中断
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   

// Group 12 PIE Vectors
//第12组PIE中断
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
      rsvd_ISR,   
};


//---------------------------------------------------------------------------
// InitPieVectTable: 
//---------------------------------------------------------------------------
// This function initializes the PIE vector table to a known state.
// This function must be executed after boot time.
//初始化PIE中断向量表

void InitPieVectTable(void)
{
	int16	i;
	//取PIE中断服务程序入口地址
	Uint32 *Source = (void *) &PieVectTableInit;
	//取中断向量表的地址
	Uint32 *Dest = (void *) &PieVectTable;
	
		
	EALLOW;	
	for(i=0; i < 128; i++)
		*Dest++ = *Source++;	
	EDIS;

	// Enable the PIE Vector Table
	//使能PIE模块
	PieCtrlRegs.PIECRTL.bit.ENPIE = 1;	
			
}

//===========================================================================
// No more.
//===========================================================================
