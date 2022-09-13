/*
 * main.c: starter code
 *
 * ECE 266 Lab 2, Fall 2022
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "seg7.h"
#include "launchpad.h"

/*
 * Global variables: In embedded system programming, careful use of global variable is acceptable
 * and usually necessary.
 */

// The state of the 7-segment display. See seg7.h for the definition of Seg7Display.
Seg7Display seg7 = { { 3, 2, 1, 0 }, false }; // initial state is "3210" with colon off

/*
 * Task 1: Update the clock
 *
 * REVISE THE FUNCTION TO MAKE IT RUN A CLOCK
 */
void ClockUpdate(uint32_t time)
{
    // Update the colon_on state, from true to false, or false to true
    if (seg7.colon_on)
        seg7.colon_on = false;
    else
        seg7.colon_on = true;

    // Update the 7-segment
    Seg7Update(&seg7);

    // Schedule a callback after 0.5 seconds
    ScheduleCallback(ClockUpdate, time + 500);
}

/*
 * Task 2: Check the push button.
 *
 * If SW1 is pushed, fast-forward minutes. If SW2 is pushed, fast-forward seconds.
 *
 * REVISE THE FUNCTION TO MAKE THE PUSHBUTTONS WORK AS INTENDED
 */
void CheckPushButton(uint32_t time)
{
    int code = PbRead();
    uint32_t delay = 10;

    switch (code)
    {
    case 1:

        delay = 150;                      // Use an inertia for soft de-bouncing
        break;

    case 2:

        delay = 150;                      // Use an inertia for soft de-bouncing
        break;
    }

    ScheduleCallback(CheckPushButton, time + delay);
}

/*
 * The main function: Initialize Tiva C and schedule callback tasks
 */
int main(void)
{
    LpInit();
    Seg7Init();

    uprintf("%s\n\r", "Lab 2: Wall clock");

    ScheduleCallback(ClockUpdate, 1000);
    ScheduleCallback(CheckPushButton, 1005);

    // Loop forever
    while (true)
    {
        ScheduleExecute();
    }
}