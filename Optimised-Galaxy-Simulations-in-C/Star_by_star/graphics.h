/*
 * File: graphics.h
 * ----------------
 * Contains definitions for the public graphing functions.
 *
 * CME212 Assignment 5
 * Oliver Fringer
 * Stanford University
 *
 */
#ifndef _graphics_h
#define _graphics_h

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void FlushDisplay(void);
void CloseDisplay(void);

/*
 * Function: CheckForQuit
 * Usage: while(!CheckForQuite()) {...
 * -----------------------------------
 * Closes the window when the users hits the q key.
 *
 */
int CheckForQuit(void);

/*
 * Function: Refresh
 * Usage: Refresh();
 * -----------------
 * Copy the pixel buffer to the window and flush the display.  Graphics objects
 * plotted to the window will not appear on the display unless this function is
 * used.
 *
 */
void Refresh(void);

/*
 * Function: ClearScreen
 * Usage: ClearScreen();
 * ---------------------
 * Fills the plotting window with black and effectively clears the
 * plotting screen.
 *
 */
void ClearScreen(void);

void DrawRectangle(float x, float y, float W, float H, float dx, float dy, float color);

/*
 * Function: DrawCircle
 * Usage: DrawCircle(x,y,W,H,r,fillcolor);
 * ---------------------------------------
 * Draws the circle with radius r centered about x,y in the window
 * of width W and height H.  The circle is filled with the grayscale color defined
 * in the range specified in the SetCAxes command.  Since there are 256 colors,
 * if the grayscale axes are set with SetCAxes(.1,.5), then a value of fillcolor 
 * of .25 will given grayscale color number (.25-.1)/.5*NUMCOLORS.  a fillcolor of
 * 0 is white, while 1 is black.
 *
 */
void DrawCircle(float x, float y, float W, float H, float radius, float color);

/*
 * Function: InitializeGraphics
 * Usage: InitializeGraphics(argv[0],800,800);
 * -------------------------------------------
 * Opens up a graphics window with the specified dimensions in pixels for plotting
 * with the upper left hand corner of the graphics window appearing in the 
 * upper left hand corner of the display.
 *
 * The command argument argv[0] or equivalent is used to return errors.  
 *
 */
void InitializeGraphics(char *command, int windowWidth, int windowHeight);


/*
 * Function: SetCAxes
 * Usage: SetCAxes(0,1);
 * ---------------------
 * Sets the grayscale color axes.  See DrawCircle for details.
 * 
 */
void SetCAxes(float cmin, float cmax);

#endif
