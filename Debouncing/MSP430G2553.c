#include <msp430.h>

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;         // Disable the watchdog timer

	P1DIR |= BIT6;                    // Set P1.6 to output

	P1DIR &= ~BIT3;                   // Set P1.3 to input
	P1REN |= BIT3;                    // Enable internal resistor
	P1OUT |= BIT3;                    // Sets pull-up resistor

	P1IE = BIT3;                      // Enabled interrupt 
	P1IFG &= ~BIT3;                   // Clear interrupt flag
	TA0CCTL0 = CCIE;                  //Enabled Capture Compare Interrupt 

	__enable_interrupt();              // Enable interrupt algorithm

	while(1){};                        // Infinite while loop
}
                             
#pragma vector = PORT1_VECTOR       // Interrupt service routine for Port 1
__interrupt void Port_1(void)
{
    P1OUT ^= BIT6;                  // Toggle P1.6
    P1IE &= ~BIT3;                  // Turn off interrupt for button to prevent bouncing due to unintentional interrupts

    TACCR0 = 10000;                 // Set capture compare register value for timer count (~10ms)
    TA0CTL = TASSEL_2 + MC_1 + TACLR;   // Timer Control: SMCLK (~1 MHz), Up mode, Clear
}

// Interrupt service routine for TimerA0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    TA0CTL = MC_0 + TACLR;          // Timer Control: Stop timer, Clear
    P1IE |= BIT3;                   // Enable interrupt on P1.3 (Button) to detect next interrupt; Occurs after ~10ms in order to prevent bouncing
    P1IFG &= ~BIT3;                 // Clear flags for the button interrupt
}
