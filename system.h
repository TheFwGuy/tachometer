/**
 *  @file system.h
 *  @author TheFwGuy
 *  @version 1.0
 *  @date October 2013
 *  @brief Header file for the system.c 
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H

/* definitions */

// POWER MODE
#define MODE_ACTIVE  0
#define MODE_LPM0    1
#define MODE_LPM1    2
#define MODE_LPM2    3
#define MODE_LPM3    4
#define MODE_LPM4    5

/* Generic bit addressing */

#define BIT_0  0x01
#define BIT_1  0x02
#define BIT_2  0x04
#define BIT_3  0x08
#define BIT_4  0x10
#define BIT_5  0x20
#define BIT_6  0x40
#define BIT_7  0x80

#define TMRVALUE        512      /* Timer will generate a interrupt every 1 s */
#define RPM_IN          BIT1     /* Bit used to read Hall sensor */

/*
 *  Function prototypes
 */
void InitFreq(void);
void InitTimer(void);
void InitPeriph(void);

#endif

