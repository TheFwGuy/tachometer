/**
 *  @file main.c
 *  @author TheFwGuy
 *  @version 1.0
 *  @date October 2013
 *  @brief Main file for the RPM meter, a simple tachometer for slow/medium electric motors
 *
 *  This is the main entry for a program capable to read the revolution of a rotor in
 *  a electric motor experiment.
 *  A rotor has at least 2 magnets on it, so it is necessary to provide to the program
 *  the number of magnets as parameter.
 *  This program is executed on a Olimex MSP430F169LCD board and compiled using Mspgcc 4.4.3
 *
 *  Pinout :
 *    P1.1   Hall sensor input
 *    P1.4   Joystick direction
 *    P1.5   Joystick direction
 *    P1.6   Joystick direction
 *    P1.7   Joystick direction
 *    P2.0   Joystick pushbutton
 *    P2.1   Status LED - toggle at every Hal sensor signal
 *    P2.2   Status clock - 2 sec. period
 *    P2.3   Debug pin
 */

#include <stdio.h>
#include <string.h>
#include "system.h"
#include "lcd_new.h"
#include <io.h>
#include <signal.h>
/*
 *  Global defines
 */

#define DELAY_1   0

unsigned char i = 0;

// Measurement variables
unsigned char NumMagnets = 2; /* Number of magnets (1 to 8 )*/
unsigned char AcqSecTime = 1; /* Acquisition time in seconds (1 to 10) */

unsigned short Rpm_cnt;	     /* RPM counter */
short Rpm_display;              /* RPM display value */
unsigned char  Rpm_show;       /* Flag to display the result */

// simple delay
void DelayN(unsigned long a) { while (--a!=0); }
void SetParam(void);

/**
 *  @fn Menu
 *  @brief The function display the main menu and waits for a command
 *
 *  The function display the menu and then wait
 *  @param none
 *  @return position
 */
unsigned char Menu(void)
{
   unsigned char locPos = 1;
   unsigned char display = 1;
   char press_up   = 1;
   char press_down = 1;

   //Wait if pushbutton is pressed
   while((P2IN&BIT0) == 0);

   // loop for choose
   do
   {
      /*
       *  Max length
       * "12345678901234"
       */
      if(display)
      {
         LCDClear();
         LCDStr ( 0, (unsigned char *)" Set " );
         LCDStr ( 1, (unsigned char *)" Measure " );
         LCDStr ( locPos-1, (unsigned char *)">" );
         LCDUpdate();
         display = 0;
      }

      // Up
      if((!(P1IN&BIT6))&&(press_up==1))
      {
         locPos--;
         if(locPos<1) locPos = 1;

         press_up = 0;
         display = 1;
      }
      if((P1IN&BIT6)&&(press_up==0))
      {
         press_up = 1;
      }

      // Down
      if((!(P1IN&BIT5))&&(press_down==1))
      {
         locPos++;
         if(locPos>2) locPos = 2;

         press_down = 0;
         display = 1;
      }

      if((P1IN&BIT5)&&(press_down==0))
      {
        press_down = 1;
      }

  } while(P2IN&BIT0);

  return (locPos);
}

/**
 *  @fn Measure
 *  @brief The function measure the RPM
 *
 *  @param none
 *  @return none
 */
void Measure(void)
{
   /*
    *  Max length
    * "12345678901234"
    */
   char tmpBuf[20];
   int rpm;

   LCDClear();
   LCDStr ( 0, (unsigned char *)" Measuring " );
   sprintf(tmpBuf, " Magnets : %u", NumMagnets);
   LCDStr ( 1, (unsigned char *)tmpBuf );
   sprintf(tmpBuf, " Timer (s): %u", AcqSecTime);
   LCDStr ( 2, (unsigned char *)tmpBuf );
   LCDUpdate();

   P2OUT |= BIT3;   // Set debug pin high

   //if joystick is not pressed
   while((P2IN&BIT0) == 1)
   {
      /* Display the RPM here */
      if(Rpm_show == 1)
      {
         rpm = (Rpm_display / NumMagnets) * (60 / AcqSecTime);
         /*
          *  Refresh display value
          */
         sprintf(tmpBuf, " Raw count = %d   ", Rpm_display);
         LCDStr ( 3, (unsigned char *)tmpBuf);
         sprintf(tmpBuf, " RPM = %d   ", rpm);
         LCDStr ( 4, (unsigned char *)tmpBuf);
         LCDUpdate();

         Rpm_show = 0;
      }
   }

   P2OUT &= ~BIT3;   // Set debug pin low

   return;
}

/**
 *  @fn main
 *  @brief Program main entry
 *
 *  @param none
 *  @return none (really :) )
 */
int main( void )
{
   unsigned char menuSelection;

   /**** INITIALIZATION ****/
   WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

   Rpm_cnt       = 0;
   Rpm_display   = 0;
   Rpm_show      = 0;

   // Initialize I/O
   InitPeriph();

   // Frequency
   InitFreq();
   InitTimer();

   // LCD init
   LCDInit();
   LCDContrast(0x45);

   eint();  /* Enable interrupts */

   for(;;)
   {
      // Show main menu
      menuSelection = Menu();

      //Wait if pushbutton is pressed
      while((P2IN&BIT0) == 0);
      DelayN(1000);

      switch(menuSelection)
      {
         default:
            menuSelection = 1;
         case 1:     /* Set */
            SetParam();
            InitTimer();  /* Reinitialize timer for possible new AcqSetTime */
            break;

         case 2:     /* Measure */
            Rpm_display = 0;
            Rpm_cnt     = 0;

            Measure();
            break;
      }
   }
}

/**
 * Timer_A
 * @brief Timer A0 interrupt service routine
 *
 * This function handle the Timer A interrupt, in order to perform
 *  time related operations.
 *
 * The timer is set to generate an interrupt every second.
 *
 * @param none
 * @return None
 */
interrupt(TIMERA0_VECTOR) Timer_A (void)
{
   /*
    *  1 second interrupt expired
    *  Copy the counter in the display value and reset the counter
    *  Set flag for display value
    */
   Rpm_display = Rpm_cnt;
   Rpm_cnt     = 0;
   Rpm_show    = 1;
   P2OUT ^= BIT2;   // toggle status clock
}

/**
 * I/O Port 1
 * @brief I/O port 1 interrupt service routine
 *
 * This function handle the I/O Port 1 interrupt.
 *
 * @param none
 * @return None
 */
interrupt(PORT1_VECTOR) PORT1_ISR(void)
{
   if(P1IFG & RPM_IN)
   {
      /*
       *  Interrupt on Pin 1.1 !
       *  Be sure is not a spike !
       */
      if(P1IN & RPM_IN)
      {
         P2OUT ^= BIT1;   // toggle status LED
         Rpm_cnt++;   /* Increment counter */
      }

      P1IFG &= ~RPM_IN;  /* Reset I/O interrupt on P1.1 */
   }
}

/*
 *  This code is documented using DoxyGen
 *  (http://www.stack.nl/~dimitri/doxygen/index.html)
 */
