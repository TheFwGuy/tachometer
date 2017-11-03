/**
 *  @file system.h
 *  @author TheFwGuy
 *  @version 1.0
 *  @date October 2013
 *  @brief Initialization functions for the Olimex MSP430F169 LCD board 
 */
#include "system.h"
#define __MSP430_HAS_BC2__
#include <msp430x16x.h>

extern unsigned char AcqSecTime;

/**
 *  @fn InitFreq
 *  @brief The function initialize the system clock
 *
 *  The program uses two main clock.
 *  The DCO set for a low/middle range frequency (around 800kHz) and the 32Khz crystal
 *  The DCO clock handles the MCLK and SMCLK and the 32kHz crystal the ACLK
 *
 *  @param none
 *  @return none
 */
void InitFreq(void)
{
   int alert = 0;

   WDTCTL = WDTPW + WDTHOLD;   /* Stop watchdog timer */

   /*
    *  DCO Control Register
    *  MODx = 0, DCOx = 3
    *  description:
    *  modulation off, DCOx and RSELx ~800kHz
    */
    DCOCTL = 0x60;   

   /*
    *  Basic Clock System Control Register 1
    *  RSELx = 7, DIVAx = 3, XTS = 0, XT20FF = 1
    *  description:
    *  XT2 off, ACLK divide by 8
    */
   BCSCTL1 = 0xB7;

   /*
    *  Basic Clock System Control Register 2
    *  DCOR = 0, DIVSx = 0, SELS = 0, DIVMx = 0, SELMx = 0
    *  description:
    *
    */
   BCSCTL2 = 0x00;

   /*
    *  Basic Clock System Control Register 3
    *  register:
    *  description:
    *
    */
   BCSCTL3 = 0x0C;

   /* As suggested by the main MSP430 datasheet */
   do
   {
      IFG1 = 0;
      alert++;
   } while((IFG1 & OFIFG) != 0 && alert < 50000);
}

/**
 *  @fn InitTimer
 *  @brief The function initialize the Timer A0
 *
 *  @param none
 *  @return none
 */
void InitTimer(void)
{
   /* Setting timer A */

   TACTL = TASSEL_1 + ID_3 + MC_1;    /* Uses ACLK, divide by 8, count in up mode */
   TACCTL0 = CCIE;                    /* Use TACCR0 to generate interrupt */
   TACCR0 = TMRVALUE * AcqSecTime;    /* 1s */

   /*  NORMAL MODE */
   TACCTL0 &= ~0x0080;                /* Disable Out0 */
}

/**
 *  @fn InitPeriph
 *  @brief The function initialize the I/O
 *
 *  @param none
 *  @return none
 */
void InitPeriph(void)
{
   /* set first functionality to all ports */
   P1SEL = 0x0;
   P2SEL = 0x0;
   P3SEL = 0x0;

   /* all as input */
   P1DIR = 0x0;
   P2DIR = 0x0;
   P3DIR = 0x0;
   P4DIR = 0x0;
   P5DIR = 0x0;
   P6DIR = 0x0;

   /* Set LED Status pin */
   P2OUT &= ~BIT1;   /* low (ON) */
   P2DIR |= BIT1;    

   /* Set Clock Status pin */
   P2OUT &= ~BIT2;   /* low */
   P2DIR |= BIT2;    

   /* Set Debug pin */
   P2OUT &= ~BIT3;   /* low */
   P2DIR |= BIT3;    

   P1IES &= ~BIT1;   /* Set P1.1 interrupt generation on the low-to-high transition */
   P1IE |= BIT1;     /* Enable interrupt on P1.1 */
}

