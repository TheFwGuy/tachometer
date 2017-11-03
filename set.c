/**
 *  @file set.c
 *  @author TheFwGuy
 *  @version 1.0
 *  @date October 2013
 *  @brief Setting menu management for RPM meter 
 *
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

// Measurement variables
extern unsigned char NumMagnets; /* Number of magnets */
extern unsigned char AcqSecTime; /* Acquisition time in seconds */

// simple delay
void DelayN(unsigned long a);

/**
 *  @fn SetParam
 *  @brief The function display the set menu and waits for a command
 *
 *  The function display the menu and then wait
 *  @param none
 *  @return none
 */
void SetParam(void)
{
   unsigned char locPos = 1;
   unsigned char display = 1;
   char press_up   = 1;
   char press_down = 1;
   char press_left = 1;
   char press_right = 1;
   char tmpBuf[20];

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
         sprintf(tmpBuf, " Magnets : %u", NumMagnets);
         LCDStr ( 0, (unsigned char *)tmpBuf );
         sprintf(tmpBuf, " Timer (s): %u", AcqSecTime);
         LCDStr ( 1, (unsigned char *)tmpBuf );
         LCDStr ( 2, (unsigned char *)" Press to exit" );
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

     if(press_down == 1 && press_up == 1)
     {
        // Left (increment value)
        if((!(P1IN&BIT7))&&(press_left==1))
        {
           switch(locPos)
           {
               case 1:  /* Num magnets */
                  if(NumMagnets < 8)
                  {
                     NumMagnets++;
                     display = 1;
                  }
                  break;

               case 2:  /* Timer setting */
                  if(AcqSecTime < 10)
                  {
                     AcqSecTime++;
                     display = 1;
                  }
                  break;

              default:
                 break;
           }
           press_left = 0;
        }
        if((P1IN&BIT7)&&(press_left==0))
        {
           press_left = 1;
        }

        // Right (decrement value)
        if((!(P1IN&BIT4))&&(press_right==1))
        {
           switch(locPos)
           {
               case 1:  /* Num magnets */
                  if(NumMagnets > 1)
                  {
                     NumMagnets--;
                     display = 1;
                  }
                  break;

               case 2:  /* Timer setting */
                  if(AcqSecTime > 1)
                  {
                     AcqSecTime--;
                     display = 1;
                  }
                  break;

              default:
                 break;
           }
           press_right = 0;
        }
        if((P1IN&BIT4)&&(press_right==0))
        {
           press_right = 1;
        }
     }
  } while(P2IN&BIT0);
}


/*
 *  This code is documented using DoxyGen
 *  (http://www.stack.nl/~dimitri/doxygen/index.html)
 */
