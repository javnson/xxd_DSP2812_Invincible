#include "DSP281x_Device.h"     // DSP281xͷ�ļ�
#include "DSP281x_Examples.h"   // DSP281x Examples ͷ�ļ�
#include "config.h"

///////////////////////////////////////////////////////////////////////
// NOTE: �����ﶨ��ȫ�ֱ���


///////////////////////////////////////////////////////////////////////
// NOTE: ���ڳ�ʼ��ȫ�ֱ���
void init_var(void)
{
	// ��ʾһ��ѧ����
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

	// ���ϰ汾�����Ƴ���
	GreenLedOn(LED(1) & LED(2) & LED(3) & LED(4));
	GreenLedOff(LED(1) & LED(2) & LED(3) & LED(4));

    // ���
    LedOut(0xF0F0);
    LedOut(0x0F0F);

}

// NOTE: ��Ҫ����ѭ������ɵĴ���
void loop(void)
{
	if (GetKey()) // ����
	{
		// �������ж���������
		LedOut(LEDReg);
	}

	ADC_run();
	DisplayADC(); // ��ʾADC��ʾ��

	vm[7] = hour / 10;
	vm[6] = hour % 10;
	vm[5] = 0x11;
	vm[4] = min / 10;
	vm[3] = min % 10;
	vm[2] = 0x11;
	vm[1] = sec / 10;
	vm[0] = sec % 10;

	refreashLed(vm);
	// ���湦�ܿ���ͨ������ĺ���ʵ�֣�
	DisplayClock();

}

////////////////////////////////////////////////////////////////////////
// NOTE: ��Ҫ���ж�����ɵĴ���
interrupt void cpu_timer0_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������
	StepSec(); // ���Խ�λ����

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;// ���ж�Ӧ���źţ�׼��������һ���ж�
}

interrupt void eva_timer1_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������

	// �ָ��ж�
	EvaRegs.EVAIFRA.all = BIT7; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // �����´��ж�
}

// �ں��ʵ�ʱ������ز�����
interrupt void evb_timer3_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvbRegs.EVBIFRA.all = BIT7; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // �����´��ж�
}

interrupt void eva_CMP1INT_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvaRegs.EVAIFRA.all = BIT1; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // �����´��ж�
}

interrupt void eva_CMP2INT_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvaRegs.EVAIFRA.all = BIT2; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // �����´��ж�
}

interrupt void eva_CMP3INT_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvaRegs.EVAIFRA.all = BIT3; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // �����´��ж�
}

#ifdef _CMP_
interrupt void eva_timer1_cnt_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvaRegs.EVAIFRA.all = BIT8; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2; // �����´��ж�
}
#endif

interrupt void evb_CMP4INT_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvbRegs.EVBIFRA.all = BIT1; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // �����´��ж�
}

interrupt void evb_CMP5INT_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvbRegs.EVBIFRA.all = BIT2; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // �����´��ж�
}

interrupt void evb_CMP6INT_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvbRegs.EVBIFRA.all = BIT3; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // �����´��ж�
}

#ifdef _CMP_
interrupt void evb_timer3_cnt_isr(void)
{
	// TODO: д��Ҫ�Ĵ���������


	// �ָ��ж�
	EvbRegs.EVBIFRA.all = BIT8; // �жϱ�־��
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4; // �����´��ж�
}
#endif 
