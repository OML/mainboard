#include "timer.h"
#include "device.h"

volatile uint32_t rt_timer_val;

void _ISR __attribute__((no_auto_psv)) _T2Interrupt(void)
{
        rt_timer_val++;
        IFS0bits.T2IF = 0;
}

uint32_t rt_timer(void)
{
        return rt_timer_val;
}

void timer_start(void)
{
        T2CONbits.TCKPS = 2; // Prescaler 64
        T2CONbits.TON = 1;
        PR2 = 250; // (16000 / 64)

        IFS0bits.T2IF = 0;
        IEC0bits.T2IE = 1;
}
