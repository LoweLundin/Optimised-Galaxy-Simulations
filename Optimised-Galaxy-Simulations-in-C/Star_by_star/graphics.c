/*
 * File: graphics.c
 * ----------------
 * Contains some simple graphics routines.
 *
 * Obtained from http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/
 * and added simple plotting functions.
 *
 * CME212 Assignment 5
 * Oliver Fringer
 * Stanford University
 * 
 * $Id: graphics.c,v 1.2 2005/03/01 04:09:13 fringer Exp fringer $
 * $Log: graphics.c,v $
 * Revision 1.2  2005/03/01 04:09:13  fringer
 * Fixed bug in DrawCircle and DrawRectangles which returned an out
 * of bounds error on the icolor index when color==caxis[1].  This
 * was fixed by using if(color>=caxis[1]) instead of if(color>caxis[1]).
 * Thanks to Sheng Chen for finding the bug.
 *
 * Revision 1.1  2005/03/01 04:07:26  fringer
 * Initial revision
 *
 *
 */
#include "graphics.h"

#define NUMCOLORS 512

Display *global_display_ptr;

Window win;
Pixmap pixmap;
XEvent report;
GC gc;
unsigned black, white;
unsigned width, height;
unsigned colors[NUMCOLORS];
float caxis[2];

/*
 * function: create_simple_window. Creates a window with a white background
 *           in the given size.
 * input:    display, size of the window (in pixels), and location of the window
 *           (in pixels).
 * output:   the window's ID.
 * notes:    window is created with a black border, 2 pixels wide.
 *           the window is automatically mapped after its creation.
 */
Window create_simple_window(Display* display, int width, int height, int x, int y)
{
  int screen_num = DefaultScreen(display);
  int win_border_width = 2;
  Window win;

  /* create a simple window, as a direct child of the screen's */
  /* root window. Use the screen's black and white colors as   */
  /* the foreground and background colors of the window,       */
  /* respectively. Place the new window's top-left corner at   */
  /* the given 'x,y' coordinates.                              */
  win = XCreateSimpleWindow(display, RootWindow(display, screen_num),
                            x, y, width, height, win_border_width,
                            BlackPixel(display, screen_num),
                            WhitePixel(display, screen_num));

  /* make the window actually appear on the screen. */
  XMapWindow(display, win);

  /* flush all pending requests to the X server. */
  XFlush(display);

  return win;
}

GC create_gc(Display* display, Window win, int reverse_video)
{
  GC gc;				/* handle of newly created GC.  */
  unsigned long valuemask = 0;		/* which values in 'values' to  */
					/* check when creating the GC.  */
  XGCValues values;			/* initial values for the GC.   */
  unsigned int line_width = 1;		/* line width for the GC.       */
  int line_style = LineSolid;		/* style for lines drawing and  */
  int cap_style = CapButt;		/* style of the line's edje and */
  int join_style = JoinBevel;		/*  joined lines.		*/
  int screen_num = DefaultScreen(display);

  gc = XCreateGC(display, win, valuemask, &values);
  if (gc < 0) {
	fprintf(stderr, "XCreateGC: \n");
  }

  /* allocate foreground and background colors for this GC. */
  if (reverse_video) {
    XSetForeground(display, gc, WhitePixel(display, screen_num));
    XSetBackground(display, gc, BlackPixel(display, screen_num));
  }
  else {
    XSetForeground(display, gc, BlackPixel(display, screen_num));
    XSetBackground(display, gc, WhitePixel(display, screen_num));
  }

  /* define the style of lines that will be drawn using this GC. */
  XSetLineAttributes(display, gc,
                     line_width, line_style, cap_style, join_style);

  /* define the fill style for the GC. to be 'solid filling'. */
  XSetFillStyle(display, gc, FillSolid);

  return gc;
}

void InitializeGraphics(char *command, int windowWidth, int windowHeight) {
  int i;
  Screen *screen;
  int screen_num;		/* number of screen to place the window on.  */
  /* height and width of the X display
  unsigned int display_width,
               display_height;
  */
  char *display_name = getenv("DISPLAY");  /* address of the X display.      */
  Colormap screen_colormap;     /* color map to use for allocating colors.   */
  XColor color;

  /* Set the height and width of the window */
  width=windowWidth;
  height=windowHeight;

  /* open connection with the X server. */
  global_display_ptr = XOpenDisplay(display_name);
  if (global_display_ptr == NULL) {
    fprintf(stderr, "%s: cannot connect to X server '%s'\n", command, display_name);
    exit(1);
  }

  /* get the geometry of the default screen for our display. */
  screen = ScreenOfDisplay(global_display_ptr, 0);
  screen_num = XScreenNumberOfScreen(screen);
  /*
  display_width = DisplayWidth(global_display_ptr, screen_num);
  display_height = DisplayHeight(global_display_ptr, screen_num);
  */

  /* create a simple window, as a direct child of the screen's   */
  /* root window. Use the screen's white color as the background */
  /* color of the window. Place the new window's top-left corner */
  /* at the given 'x,y' coordinates.                             */
  win = create_simple_window(global_display_ptr, width, height, 0, 0);
  pixmap = XCreatePixmap(global_display_ptr, win, width, height, DefaultDepthOfScreen(screen));

  /* allocate a new GC (graphics context) for drawing in the window. */
  gc = create_gc(global_display_ptr, win, 0);
  XSync(global_display_ptr, False);

  /* get access to the screen's color map. */
  screen_colormap = DefaultColormap(global_display_ptr, DefaultScreen(global_display_ptr));

  black = BlackPixel(global_display_ptr,screen_num);
  white = WhitePixel(global_display_ptr,screen_num);

  /* Load the jet color map */
  for(i=0;i<NUMCOLORS;i++) {
    color.red=((double)(NUMCOLORS-i)/(double)NUMCOLORS)*0xFFFF;
    color.blue=color.red;
    color.green=color.red;
    XAllocColor(global_display_ptr, screen_colormap,&color);    
    colors[i]=color.pixel;
  }
  SetCAxes(0,1);

  /* Set up the window to wait for a quit signal */
  XSelectInput(global_display_ptr, win, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask );
  XCopyArea(global_display_ptr, pixmap, win, gc, 0, 0, width, height, 0, 0);
  XMaskEvent(global_display_ptr, ExposureMask, &report);
}

void SetCAxes(float cmin, float cmax) {
  caxis[0]=cmin;
  caxis[1]=cmax;
}

int CheckForQuit(void) {
  int keysym;

  if(XCheckMaskEvent(global_display_ptr,KeyPressMask,&report)) {
    switch(keysym=XLookupKeysym(&report.xkey, 0)) {
    case XK_q:
      return 1;
      break;
    default:
      break;
    }
  }
  return 0;
}

void Refresh(void) {
  XCopyArea(global_display_ptr, pixmap, win, gc, 0, 0, width, height, 0, 0);
  XFlush(global_display_ptr);
}

void ClearScreen(void) {
  XSetForeground(global_display_ptr, gc, black);
  XFillRectangle(global_display_ptr, pixmap, gc, 0, 0, width, height);
}

void DrawCircle(float x, float y, float W, float H, float radius, float color) {
  int i=(int)((x-radius)/W*width);
  int j=height-(int)((y+radius)/H*height);
  int arcrad=2*(int)(radius/W*width);
  int icolor;

  if(color>=caxis[1])
    icolor=NUMCOLORS-1;
  else if(color<caxis[0])
    icolor=0;
  else
    icolor=(int)((color-caxis[0])/(caxis[1]-caxis[0])*(float)NUMCOLORS);

  XSetForeground(global_display_ptr, gc, colors[icolor]);
  XFillArc(global_display_ptr, pixmap, gc, i, j, arcrad, arcrad, 0, 64*360);
}

void DrawRectangle(float x, float y, float W, float H, float dx, float dy, float color) {
  int i=(int)(x/W*width);
  int j=height-(int)((y+dy)/H*height);
  int w=(int)(dx/W*width);
  int h=(int)(dy/H*height);
  int icolor;

  if(color>=caxis[1])
    icolor=NUMCOLORS-1;
  else if(color<caxis[0])
    icolor=0;
  else
    icolor=(int)((color-caxis[0])/(caxis[1]-caxis[0])*(float)NUMCOLORS);

  XSetForeground(global_display_ptr, gc, colors[icolor]);
  XDrawRectangle(global_display_ptr, pixmap, gc, i, j, w, h);
}

void FlushDisplay(void) {
  XFlush(global_display_ptr);
}

void CloseDisplay(void) {
  XCloseDisplay(global_display_ptr);
}
