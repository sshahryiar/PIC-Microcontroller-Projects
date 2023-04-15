/////////////////////////////////////////////////////////////////////////
////                          graphics.c                             ////
////                                                                 ////
////   This file contains functions to draw lines, rectangles, bars, ////
////   circles and text to a display. A function which draws a       ////
////   single pixel must be defined before calling the functions in  ////
////   this file. Call it glcd_pixel(x, y, color) where x is the     ////
////   horizontal coordinate, y is the vertical coordinate, and      ////
////   color is 1 bit to turn the pixel on or off.                   ////
////                                                                 ////
////   * Note: (0, 0) is treated as the upper left corner            ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////  glcd_line(x1, y1, x2, y2, color)                               ////
////     * Draws a line from the first point to the second point     ////
////       with the given color                                      ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_rect(x1, y1, x2, y2, fill, color)                         ////
////     * Draws a rectangle with one corner at point (x1,y1) and    ////
////       the other corner at point (x2,y2)                         ////
////       - fill can be YES or NO                                   ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_bar(x1, y1, x2, y2, width, color)                         ////
////     * Draws a bar (wide line) from the first point to the       ////
////       second point                                              ////
////       - width is the number of pixels wide                      ////
////       - color is ON or OFF                                      ////
////                                                                 ////
////  glcd_circle(x, y, radius, fill, color)                         ////
////     * Draws a circle with center at (x,y)                       ////
////       - fill can be YES or NO                                   ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_text57(x, y, textptr, size, color)                        ////
////     * Write the null terminated text pointed to by textptr with ////
////       the upper left coordinate of the first character at (x,y) ////
////       Characters are 5 pixels wide and 7 pixels tall            ////
////       - size is an integer that scales the size of the text     ////
////       - color is ON or OFF                                      ////
////     * Note - This function wraps characters to the next line    ////
////              use #define GLCD_WIDTH to specify a display width  ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2004 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
#ifndef GRAPHICS_DRAWING_FUNCTIONS
#define GRAPHICS_DRAWING_FUNCTIONS
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
#ifndef ON
#define ON  1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif
/////////////////////////////////////////////////////////////////////////


const int8 TEXT35[95][5]={
   0b00000000,    0b00000000,    0b00000000,    0b00000000,    0b00000000, //SPACE
   0b01000100,    0b01000100,    0b01000100,    0b00000000,    0b01000100, //!
   0b10101010,    0b10101010,    0b00000000,    0b00000000,    0b00000000, //"
   0b10101010,    0b11101110,    0b10101010,    0b11101110,    0b10101010, //#
   0b01100110,    0b11001100,    0b11001100,    0b01100110,    0b11101110, //$
   0b10101010,    0b00100010,    0b01000100,    0b10001000,    0b10101010, //%
   0b01000100,    0b10101010,    0b01000100,    0b10101010,    0b01100110, //&
   0b01000100,    0b01000100,    0b00000000,    0b00000000,    0b00000000, //'
   0b01000100,    0b10001000,    0b10001000,    0b10001000,    0b01000100, //(
   0b01000100,    0b00100010,    0b00100010,    0b00100010,    0b01000100, //)
   0b00000000,    0b10101010,    0b01000100,    0b10101010,    0b00000000, //*
   0b00000000,    0b01000100,    0b11101110,    0b01000100,    0b00000000, //+
   0b00000000,    0b00000000,    0b00000000,    0b01000100,    0b10001000, //,
   0b00000000,    0b00000000,    0b11101110,    0b00000000,    0b00000000, //-
   0b00000000,    0b00000000,    0b00000000,    0b00000000,    0b01000100, //.
   0b00100010,    0b00100010,    0b01000100,    0b10001000,    0b10001000, ///
   0b11101110,    0b10101010,    0b10101010,    0b10101010,    0b11101110, //0
    0b01000100,    0b11001100,    0b01000100,    0b01000100,    0b11101110, //1
    0b11101110,    0b00100010,    0b11101110,    0b10001000,    0b11101110, //2
    0b11101110,    0b00100010,    0b11101110,    0b00100010,    0b11101110, //3
    0b10101010,    0b10101010,    0b11101110,    0b00100010,    0b00100010, //4
    0b11101110,    0b10001000,    0b11101110,    0b00100010,    0b11101110, //5
    0b11001100,    0b10001000,    0b11101110,    0b10101010,    0b11101110, //6
    0b11101110,    0b00100010,    0b01000100,    0b10001000,    0b10001000, //7
    0b11101110,    0b10101010,    0b11101110,    0b10101010,    0b11101110, //8
    0b11101110,    0b10101010,    0b11101110,    0b00100010,    0b01100110, //9
    0b00000000,    0b01000100,    0b00000000,    0b01000100,    0b00000000, //:
    0b00000000,    0b01000100,    0b00000000,    0b01000100,    0b10001000, //;
    0b00100010,    0b01000100,    0b10001000,    0b01000100,    0b00100010, //<
    0b00000000,    0b11101110,    0b00000000,    0b11101110,    0b00000000, //=
    0b10001000,    0b01000100,    0b00100010,    0b01000100,    0b10001000, //>
    0b11001100,    0b00100010,    0b01100110,    0b00000000,    0b01000100, //?
    0b01000100,    0b10101010,    0b11101110,    0b10001000,    0b01100110, //@
    0b11101110,    0b10101010,    0b11101110,    0b10101010,    0b10101010, //A
    0b11001100,    0b10101010,    0b11101110,    0b10101010,    0b11001100, //B
    0b11101110,    0b10001000,    0b10001000,    0b10001000,    0b11101110, //C
    0b11001100,    0b10101010,    0b10101010,    0b10101010,    0b11001100, //D
    0b11101110,    0b10001000,    0b11101110,    0b10001000,    0b11101110, //E
    0b11101110,    0b10001000,    0b11101110,    0b10001000,    0b10001000, //F
    0b11101110,    0b10001000,    0b10001000,    0b10101010,    0b11101110, //G
    0b10101010,    0b10101010,    0b11101110,    0b10101010,    0b10101010, //H
    0b11101110,    0b01000100,    0b01000100,    0b01000100,    0b11101110, //I
    0b00100010,    0b00100010,    0b00100010,    0b10101010,    0b11101110, //J
    0b10001000,    0b10101010,    0b11001100,    0b11001100,    0b10101010, //K
    0b10001000,    0b10001000,    0b10001000,    0b10001000,    0b11101110, //L
    0b10101010,    0b11101110,    0b11101110,    0b10101010,    0b10101010, //M
    0b00000000,    0b11001100,    0b10101010,    0b10101010,    0b10101010, //N
    0b01000100,    0b10101010,    0b10101010,    0b10101010,    0b01000100, //O
    0b11101110,    0b10101010,    0b11101110,    0b10001000,    0b10001000, //P
    0b01000100,    0b10101010,    0b10101010,    0b11101110,    0b01100110, //Q
    0b11101110,    0b10101010,    0b11001100,    0b11101110,    0b10101010, //R
    0b11101110,    0b10001000,    0b11101110,    0b00100010,    0b11101110, //S
    0b11101110,    0b01000100,    0b01000100,    0b01000100,    0b01000100, //T
    0b10101010,    0b10101010,    0b10101010,    0b10101010,    0b11101110, //U
    0b10101010,    0b10101010,    0b10101010,    0b10101010,    0b01000100, //V
    0b10101010,    0b10101010,    0b11101110,    0b11101110,    0b10101010, //W
    0b00000000,    0b10101010,    0b01000100,    0b01000100,    0b10101010, //X
    0b10101010,    0b10101010,    0b01000100,    0b01000100,    0b01000100, //Y
    0b11101110,    0b00100010,    0b01000100,    0b10001000,    0b11101110, //Z
    0b11101110,    0b10001000,    0b10001000,    0b10001000,    0b11101110, //[
    0b10001000,    0b10001000,    0b01000100,    0b00100010,    0b00100010, //\
    0b11101110,    0b00100010,    0b00100010,    0b00100010,    0b11101110, //]
    0b01000100,    0b10101010,    0b00000000,    0b00000000,    0b00000000, //^
    0b00000000,    0b00000000,    0b00000000,    0b00000000,    0b11101110, //_
    0b10001000,    0b01000100,    0b00000000,    0b00000000,    0b00000000, //`
    0b00000000, 0b01000100, 0b10101010, 0b10101010, 0b01100110, //a
    0b10001000, 0b11001100, 0b10101010, 0b10101010, 0b11001100, //b
    0b00000000,    0b01100110, 0b10001000, 0b10001000, 0b01100110, //c
    0b00100010,    0b01100110, 0b10101010, 0b10101010, 0b01100110, //d
    0b00000000, 0b01000100, 0b10101010, 0b11001100, 0b01100110, //e
    0b01100110,    0b01000100, 0b11101110, 0b01000100, 0b01000100, //f
    0b00000000, 0b01000100, 0b10101010, 0b01100110, 0b11001100, //g
    0b10001000, 0b11001100, 0b10101010, 0b10101010, 0b10101010, //h
    0b01000100,    0b00000000, 0b01000100, 0b01000100, 0b01000100, //i
    0b01000100, 0b00000000, 0b01000100, 0b01000100, 0b10001000, //j
    0b10001000,    0b10001000, 0b10101010, 0b11001100, 0b10101010, //k
    0b01000100,    0b01000100, 0b01000100, 0b01000100, 0b01000100, //l
    0b00000000,    0b11101110, 0b11101110, 0b10101010, 0b10101010, //m
    0b00000000,    0b11001100, 0b10101010, 0b10101010, 0b10101010, //n
    0b00000000,    0b01000100, 0b10101010, 0b10101010, 0b01000100, //o
    0b00000000,    0b11001100, 0b10101010, 0b11001100, 0b10001000, //p
    0b00000000,    0b01100110, 0b10101010, 0b01100110, 0b00100010, //q
    0b00000000,    0b10001000, 0b11101110, 0b10001000, 0b10001000, //r
    0b00000000, 0b01100110, 0b11001100, 0b00100010, 0b11001100, //s
    0b01000100,    0b11101110, 0b01000100, 0b01000100, 0b01000100, //t
    0b00000000, 0b10101010, 0b10101010, 0b10101010, 0b01000100, //u
    0b00000000, 0b10101010, 0b10101010, 0b01000100, 0b01000100, //v
    0b00000000,    0b10101010, 0b10101010, 0b11101110, 0b10101010, //w
    0b00000000,    0b10101010, 0b01000100, 0b01000100, 0b10101010, //x
    0b00000000,    0b10101010, 0b10101010, 0b01100110, 0b11001100, //y
    0b00000000, 0b11101110, 0b01100110, 0b11001100, 0b11101110, //z
    0b00100010,    0b01000100,    0b11001100,    0b01000100,    0b00100010, //{
    0b01000100,    0b01000100,    0b01000100,    0b01000100,    0b01000100, //|
    0b10001000,    0b01000100,    0b01100110,    0b01000100,    0b10001000, //}
    0b00000000,    0b11001100,    0b10101010,    0b00000000,    0b00000000  //~
};

/////////////////////////////////////////////////////////////////////////
//// Defines a 5x7 font
/////////////////////////////////////////////////////////////////////////
const int8 FONT[51][5] ={0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
                         0x00, 0x00, 0x5F, 0x00, 0x00, // !
                         0x00, 0x03, 0x00, 0x03, 0x00, // "
                         0x14, 0x3E, 0x14, 0x3E, 0x14, // #
                         0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
                         0x43, 0x33, 0x08, 0x66, 0x61, // %
                         0x36, 0x49, 0x55, 0x22, 0x50, // &
                         0x00, 0x05, 0x03, 0x00, 0x00, // '
                         0x00, 0x1C, 0x22, 0x41, 0x00, // (
                         0x00, 0x41, 0x22, 0x1C, 0x00, // )
                         0x14, 0x08, 0x3E, 0x08, 0x14, // *
                         0x08, 0x08, 0x3E, 0x08, 0x08, // +
                         0x00, 0x50, 0x30, 0x00, 0x00, // ,
                         0x08, 0x08, 0x08, 0x08, 0x08, // -
                         0x00, 0x60, 0x60, 0x00, 0x00, // .
                         0x20, 0x10, 0x08, 0x04, 0x02, // /
                         0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
                         0x00, 0x04, 0x02, 0x7F, 0x00, // 1
                         0x42, 0x61, 0x51, 0x49, 0x46, // 2
                         0x22, 0x41, 0x49, 0x49, 0x36, // 3
                         0x18, 0x14, 0x12, 0x7F, 0x10, // 4
                         0x27, 0x45, 0x45, 0x45, 0x39, // 5
                         0x3E, 0x49, 0x49, 0x49, 0x32, // 6
                         0x01, 0x01, 0x71, 0x09, 0x07, // 7
                         0x36, 0x49, 0x49, 0x49, 0x36, // 8
                         0x26, 0x49, 0x49, 0x49, 0x3E, // 9
                         0x00, 0x36, 0x36, 0x00, 0x00, // :
                         0x00, 0x56, 0x36, 0x00, 0x00, // ;
                         0x08, 0x14, 0x22, 0x41, 0x00, // <
                         0x14, 0x14, 0x14, 0x14, 0x14, // =
                         0x00, 0x41, 0x22, 0x14, 0x08, // >
                         0x02, 0x01, 0x51, 0x09, 0x06, // ?
                         0x3E, 0x41, 0x59, 0x55, 0x5E, // @
                         0x7E, 0x09, 0x09, 0x09, 0x7E, // A
                         0x7F, 0x49, 0x49, 0x49, 0x36, // B
                         0x3E, 0x41, 0x41, 0x41, 0x22, // C
                         0x7F, 0x41, 0x41, 0x41, 0x3E, // D
                         0x7F, 0x49, 0x49, 0x49, 0x41, // E
                         0x7F, 0x09, 0x09, 0x09, 0x01, // F
                         0x3E, 0x41, 0x41, 0x49, 0x3A, // G
                         0x7F, 0x08, 0x08, 0x08, 0x7F, // H
                         0x00, 0x41, 0x7F, 0x41, 0x00, // I
                         0x30, 0x40, 0x40, 0x40, 0x3F, // J
                         0x7F, 0x08, 0x14, 0x22, 0x41, // K
                         0x7F, 0x40, 0x40, 0x40, 0x40, // L
                         0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
                         0x7F, 0x02, 0x04, 0x08, 0x7F, // N
                         0x3E, 0x41, 0x41, 0x41, 0x3E, // O
                         0x7F, 0x09, 0x09, 0x09, 0x06, // P
                         0x1E, 0x21, 0x21, 0x21, 0x5E, // Q
                         0x7F, 0x09, 0x09, 0x09, 0x76};// R

const int8 FONT2[44][5]={0x26, 0x49, 0x49, 0x49, 0x32, // S
                         0x01, 0x01, 0x7F, 0x01, 0x01, // T
                         0x3F, 0x40, 0x40, 0x40, 0x3F, // U
                         0x1F, 0x20, 0x40, 0x20, 0x1F, // V
                         0x7F, 0x20, 0x10, 0x20, 0x7F, // W
                         0x41, 0x22, 0x1C, 0x22, 0x41, // X
                         0x07, 0x08, 0x70, 0x08, 0x07, // Y
                         0x61, 0x51, 0x49, 0x45, 0x43, // Z
                         0x00, 0x7F, 0x41, 0x00, 0x00, // [
                         0x02, 0x04, 0x08, 0x10, 0x20, // \
                         0x00, 0x00, 0x41, 0x7F, 0x00, // ]
                         0x04, 0x02, 0x01, 0x02, 0x04, // ^
                         0x40, 0x40, 0x40, 0x40, 0x40, // _
                         0x00, 0x01, 0x02, 0x04, 0x00, // `
                         0x20, 0x54, 0x54, 0x54, 0x78, // a
                         0x7F, 0x44, 0x44, 0x44, 0x38, // b
                         0x38, 0x44, 0x44, 0x44, 0x44, // c
                         0x38, 0x44, 0x44, 0x44, 0x7F, // d
                         0x38, 0x54, 0x54, 0x54, 0x18, // e
                         0x04, 0x04, 0x7E, 0x05, 0x05, // f
                         0x08, 0x54, 0x54, 0x54, 0x3C, // g
                         0x7F, 0x08, 0x04, 0x04, 0x78, // h
                         0x00, 0x44, 0x7D, 0x40, 0x00, // i
                         0x20, 0x40, 0x44, 0x3D, 0x00, // j
                         0x7F, 0x10, 0x28, 0x44, 0x00, // k
                         0x00, 0x41, 0x7F, 0x40, 0x00, // l
                         0x7C, 0x04, 0x78, 0x04, 0x78, // m
                         0x7C, 0x08, 0x04, 0x04, 0x78, // n
                         0x38, 0x44, 0x44, 0x44, 0x38, // o
                         0x7C, 0x14, 0x14, 0x14, 0x08, // p
                         0x08, 0x14, 0x14, 0x14, 0x7C, // q
                         0x00, 0x7C, 0x08, 0x04, 0x04, // r
                         0x48, 0x54, 0x54, 0x54, 0x20, // s
                         0x04, 0x04, 0x3F, 0x44, 0x44, // t
                         0x3C, 0x40, 0x40, 0x20, 0x7C, // u
                         0x1C, 0x20, 0x40, 0x20, 0x1C, // v
                         0x3C, 0x40, 0x30, 0x40, 0x3C, // w
                         0x44, 0x28, 0x10, 0x28, 0x44, // x
                         0x0C, 0x50, 0x50, 0x50, 0x3C, // y
                         0x44, 0x64, 0x54, 0x4C, 0x44, // z
                         0x00, 0x08, 0x36, 0x41, 0x41, // {
                         0x00, 0x00, 0x7F, 0x00, 0x00, // |
                         0x41, 0x41, 0x36, 0x08, 0x00, // }
                         0x02, 0x01, 0x02, 0x04, 0x02};// ~ 
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a line on a graphic LCD using Bresenham's
//                line drawing algorithm
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                color - ON or OFF
// Dependencies:  glcd_pixel()
/////////////////////////////////////////////////////////////////////////
#ifdef LARGE_LCD
void glcd_line(int16 x1, int16 y1, int16 x2, int16 y2, int1 color)
#else
void glcd_line(int8 x1, int8 y1, int8 x2, int8 y2, int1 color)
#endif
{
   int16        dy, dx;
   signed int8  addx=1, addy=1;
   signed int16 P, diff;

   #ifdef LARGE_LCD
   int16 i=0;
   dx = abs((signed int16)(x2 - x1));
   dy = abs((signed int16)(y2 - y1));
   #else
   int8 i=0;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   #endif

   if(x1 > x2)
      addx = -1;
   if(y1 > y2)
      addy = -1;

   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(; i<=dx; ++i)
      {
         glcd_pixel(x1, y1, color);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(; i<=dy; ++i)
      {
         glcd_pixel(x1, y1, color);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
}


/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a rectangle on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                fill  - YES or NO
//                color - ON or OFF
// Dependencies:  glcd_pixel(), glcd_line()
/////////////////////////////////////////////////////////////////////////
#ifdef LARGE_LCD
void glcd_rect(int16 x1, int16 y1, int16 x2, int16 y2, int1 fill, int1 color)
#else
void glcd_rect(int8 x1, int8 y1, int8 x2, int8 y2, int1 fill, int1 color)
#endif
{
   if(fill)
   {
      #ifdef LARGE_LCD
      int16 i, xmin, xmax, ymin, ymax;
      #else
      int8  i, xmin, xmax, ymin, ymax;
      #endif

      if(x1 < x2)                            //  Find x min and max
      {
         xmin = x1;
         xmax = x2;
      }
      else
      {
         xmin = x2;
         xmax = x1;
      }

      if(y1 < y2)                            // Find the y min and max
      {
         ymin = y1;
         ymax = y2;
      }
      else
      {
         ymin = y2;
         ymax = y1;
      }

      for(; xmin <= xmax; ++xmin)
      {
         for(i=ymin; i<=ymax; ++i)
         {
            glcd_pixel(xmin, i, color);
         }
      }
   }
   else
   {
      glcd_line(x1, y1, x2, y1, color);      // Draw the 4 sides
      glcd_line(x1, y2, x2, y2, color);
      glcd_line(x1, y1, x1, y2, color);
      glcd_line(x2, y1, x2, y2, color);
   }
}

/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a bar (wide line) on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                width  - The number of pixels wide
//                color - ON or OFF
/////////////////////////////////////////////////////////////////////////
#ifdef LARGE_LCD
void glcd_bar(int16 x1, int16 y1, int16 x2, int16 y2, int8 width, int1 color)
#else
void glcd_bar(int8 x1, int8 y1, int8 x2, int8 y2, int8 width, int1 color)
#endif
{
   int8         half_width;
   signed int16 dy, dx;
   signed int8  addx=1, addy=1, j;
   signed int16 P, diff, c1, c2;

   #ifdef LARGE_LCD
   int16 i=0;
   dx = abs((signed int16)(x2 - x1));
   dy = abs((signed int16)(y2 - y1));
   #else
   int8 i=0;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   #endif

   half_width = width/2;
   c1 = -(dx*x1 + dy*y1);
   c2 = -(dx*x2 + dy*y2);

   if(x1 > x2)
   {
      signed int16 temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addx = -1;
   }
   if(y1 > y2)
   {
      signed int16 temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addy = -1;
   }

   if(dx >= dy)
   {
      P = 2*dy - dx;
      diff = P - dx;

      for(i=0; i<=dx; ++i)
      {
         for(j=-half_width; j<half_width+width%2; ++j)
         {
            #ifdef LARGE_LCD
            int16 temp;
            #else
            int8 temp;
            #endif

            temp = dx*x1+dy*(y1+j);    // Use more RAM to increase speed
            if(temp+c1 >= 0 && temp+c2 <=0)
               glcd_pixel(x1, y1+j, color);
         }
         if(P < 0)
         {
            P  += 2*dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;
      diff = P - dy;

      for(i=0; i<=dy; ++i)
      {
         if(P < 0)
         {
            P  += 2*dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
         for(j=-half_width; j<half_width+width%2; ++j)
         {
            #ifdef LARGE_LCD
            int16 temp;
            #else
            int8 temp;
            #endif

            temp = dx*x1+dy*(y1+j);    // Use more RAM to increase speed
            if(temp+c1 >= 0 && temp+c2 <=0)
               glcd_pixel(x1+j, y1, color);
         }
      }
   }
}


/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a circle on a graphic LCD
// Inputs:        (x,y) - the center of the circle
//                radius - the radius of the circle
//                fill - YES or NO
//                color - ON or OFF
/////////////////////////////////////////////////////////////////////////
#ifdef LARGE_LCD
void glcd_circle(int16 x, int16 y, int16 radius, int1 fill, int1 color)
#else
void glcd_circle(int8 x, int8 y, int8 radius, int1 fill, int1 color)
#endif
{
   #ifdef LARGE_LCD
   signed int16 a, b, P;
   #else
   signed int8  a, b, P;
   #endif

   a = 0;
   b = radius;
   P = 1 - radius;

   do
   {
      if(fill)
      {
         glcd_line(x-a, y+b, x+a, y+b, color);
         glcd_line(x-a, y-b, x+a, y-b, color);
         glcd_line(x-b, y+a, x+b, y+a, color);
         glcd_line(x-b, y-a, x+b, y-a, color);
      }
      else
      {
         glcd_pixel(a+x, b+y, color);
         glcd_pixel(b+x, a+y, color);
         glcd_pixel(x-a, b+y, color);
         glcd_pixel(x-b, a+y, color);
         glcd_pixel(b+x, y-a, color);
         glcd_pixel(a+x, y-b, color);
         glcd_pixel(x-a, y-b, color);
         glcd_pixel(x-b, y-a, color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
}


void glcd_text35(int8 x, int8 y, char* textptr, int1 color)
{
   int i, j, k;                           // Loop counters
   BYTE pixelData[5];                     // Stores character data

   for(i=0; textptr[i] != '\0'; ++i, ++x) // Loop through the passed string
   {
      if((textptr[i] >= ' ') && (textptr[i] <= '~'))
         memcpy(pixelData, TEXT35[textptr[i]-' '], 5);
      else
         memcpy(pixelData, TEXT35[0], 5); // Default to space

      if(x+3 >= GLCD_WIDTH)          // Performs character wrapping
      {
         x = 0;                           // Set x at far left position
         y += 5 + 1;                      // Set y at next position down
      }
      for(j=3; j>0; j--, x++)             // Loop through character byte data
      {
         for(k=0; k<5; k++)               // Loop through the vertical pixels
         {
            if(bit_test(pixelData[k], j)) // Check if the pixel should be set
            {
               glcd_pixel(x, y+k, color); // Draws the pixel
            }
         }
      }
   }
}


/////////////////////////////////////////////////////////////////////////
// Purpose:       Write text on a graphic LCD
// Inputs:        (x,y) - The upper left coordinate of the first letter
//                textptr - A pointer to an array of text to display
//                size - The size of the text: 1 = 5x7, 2 = 10x14, ...
//                color - ON or OFF
/////////////////////////////////////////////////////////////////////////
#ifdef LARGE_LCD
void glcd_text57(int16 x, int16 y, char* textptr, int8 size, int1 color)
#else
void glcd_text57(int8 x, int8 y, char* textptr, int8 size, int1 color)
#endif
{
   int8 j, k, l, m;                       // Loop counters
   int8 pixelData[5];                     // Stores character data

   for(; *textptr != '\0'; ++textptr, ++x)// Loop through the passed string
   {
      if(*textptr < 'S') // Checks if the letter is in the first font array
         memcpy(pixelData, FONT[*textptr - ' '], 5);
      else if(*textptr <= '~') // Check if the letter is in the second font array
         memcpy(pixelData, FONT2[*textptr - 'S'], 5);
      else
         memcpy(pixelData, FONT[0], 5);   // Default to space

      // Handles newline and carriage returns
      switch(*textptr)
      {
         case '\n':
            y += 7*size + 1;
            continue;
         case '\r':
            x = 0;
            continue;
      }

      if(x+5*size >= GLCD_WIDTH)          // Performs character wrapping
      {
         x = 0;                           // Set x at far left position
         y += 7*size + 1;                 // Set y at next position down
      }
      for(j=0; j<5; ++j, x+=size)         // Loop through character byte data
      {
         for(k=0; k < 7; ++k)             // Loop through the vertical pixels
         {
            if(bit_test(pixelData[j], k)) // Check if the pixel should be set
            {
               for(l=0; l < size; ++l)    // These two loops change the
               {                          // character's size
                  for(m=0; m < size; ++m)
                  {
                     glcd_pixel(x+m, y+k*size+l, color); // Draws the pixel
                  }
               }
            }
         }
      }
   }
}

#endif
