/////////////////////////////////////////////////////////////////////////
////                           HDM64GS12.c                           ////
////                                                                 ////
//// This file contains drivers for using a Hantronix HDM64GS12 with ////
//// a KS0108 display controller. The HDM64GS12 is 128 by 64 pixels. ////
//// The driver treats the upper left pixel as (0,0).                ////
////                                                                 ////
//// Use #define FAST_GLCD if the target chip has at least 1k of RAM ////
//// to decrease the time it takes to update the display.            ////
//// glcd_update() must then be called to update the display after   ////
//// changing the pixel information.                                 ////
//// See ex_glcd.c for suggested usage.                              ////
//// See KS0108.c for controlling a single 64 by 64 display          ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// LCD Pin connections:                                            ////
//// (These can be changed as needed in the following defines).      ////
////  * 1: VSS is connected to GND                                   ////
////  * 2: VDD is connected to +5V                                   ////
////  * 3: V0  - LCD operating voltage (Constrast adjustment)        ////
////  * 4: D/I - Data or Instruction is connected to B2              ////
////  * 5: R/W - Read or Write is connected to B4                    ////
////  * 6: Enable is connected to B5                                 ////
////  *7-14: Data Bus 0 to 7 is connected to port d                  ////
////  *15: Chip Select 1 is connected to B0                          ////
////  *16: Chip Select 2 is connected to B1                          ////
////  *17: Reset is connected to C0                                  ////
////  *18: Negative voltage is also connected to the 20k Ohm POT     ////
////  *19: Positive voltage for LED backlight is connected to +5V    ////
////  *20: Negavtive voltage for LED backlight is connected to GND   ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////  glcd_init(mode)                                                ////
////     * Must be called before any other function.                 ////
////       - mode can be ON or OFF to turn the LCD on or off         ////
////                                                                 ////
////  glcd_pixel(x,y,color)                                          ////
////     * Sets the pixel to the given color.                        ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_fillScreen(color)                                         ////
////     * Fills the entire LCD with the given color.                ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_update()                                                  ////
////     * Write the display data stored in RAM to the LCD           ////
////     * Only available if FAST_GLCD is defined                    ////
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



#ifndef HDM64GS12
#define HDM64GS12

#ifndef GLCD_WIDTH
#define GLCD_WIDTH   128
#endif

#ifndef GLCD_CS1
#define GLCD_CS1     PIN_A2   // Chip Selection 1
#endif
                   
#ifndef GLCD_CS2
#define GLCD_CS2     PIN_A1   // Chip Selection 2
#endif               

#ifndef GLCD_DI            
#define GLCD_DI      PIN_A5   // Data or Instruction input
#endif

#ifndef GLCD_RW     
#define GLCD_RW      PIN_A4   // Read/Write
#endif

#ifndef GLCD_E
#define GLCD_E       PIN_A3   // Enable
#endif

#ifndef GLCD_RST
#define GLCD_RST     PIN_A0   // Reset
#endif

#define GLCD_LEFT    0
#define GLCD_RIGHT   1
                            
#ifndef ON
#define ON           1
#endif

#ifndef OFF
#define OFF          0
#endif

/////////////////////////////////////////////////////////////////////////
// Function Prototypes
/////////////////////////////////////////////////////////////////////////
void glcd_init(int1 mode);
void glcd_pixel(int8 x, int8 y, int1 color);
void glcd_fillScreen(int1 color);
void glcd_writeByte(int1 side, BYTE data);
BYTE glcd_readByte(int1 side);
void glcd_update();
/////////////////////////////////////////////////////////////////////////

#ifdef FAST_GLCD
struct
{
   int8 left[512];
   int8 right[512];
} displayData;
#endif


// Purpose:       Initialize the LCD.
//                Call before using any other LCD function.
// Inputs:        OFF - Turns the LCD off
//                ON  - Turns the LCD on
void glcd_init(int1 mode)
{
   // Initialze some pins
   output_high(GLCD_RST);         
   output_low(GLCD_E);
   output_low(GLCD_CS1);
   output_low(GLCD_CS2);

   output_low(GLCD_DI);                 // Set for instruction
   glcd_writeByte(GLCD_LEFT,  0xC0);    // Specify first RAM line at the top
   glcd_writeByte(GLCD_RIGHT, 0xC0);    //   of the screen
   glcd_writeByte(GLCD_LEFT,  0x40);    // Set the column address to 0
   glcd_writeByte(GLCD_RIGHT, 0x40);
   glcd_writeByte(GLCD_LEFT,  0xB8);    // Set the page address to 0
   glcd_writeByte(GLCD_RIGHT, 0xB8);

   if(mode == ON)
   {
      glcd_writeByte(GLCD_LEFT,  0x3F); // Turn the display on
      glcd_writeByte(GLCD_RIGHT, 0x3F);
   }
   else
   {
      glcd_writeByte(GLCD_LEFT,  0x3E); // Turn the display off
      glcd_writeByte(GLCD_RIGHT, 0x3E);
   }

   glcd_fillScreen(OFF);                // Clear the display

   #ifdef FAST_GLCD
   glcd_update();
   #endif
}


// Purpose:    Update the LCD with data from the display arrays
#ifdef FAST_GLCD
void glcd_update()
{
   int8 i, j;
   int8 *p1, *p2;

   p1 = displayData.left;
   p2 = displayData.right;

   // Loop through the vertical pages
   for(i = 0; i < 8; ++i)
   {
      output_low(GLCD_DI);                      // Set for instruction
      glcd_writeByte(GLCD_LEFT, 0x40);          // Set horizontal address to 0
      glcd_writeByte(GLCD_RIGHT, 0x40);
      glcd_writeByte(GLCD_LEFT, i | 0xB8);      // Set page address
      glcd_writeByte(GLCD_RIGHT, i | 0xB8);
      output_high(GLCD_DI);                     // Set for data

      // Loop through the horizontal sections
      for(j = 0; j < 64; ++j)
      {
         glcd_writeByte(GLCD_LEFT, *p1++);      // Turn pixels on or off
         glcd_writeByte(GLCD_RIGHT, *p2++);     // Turn pixels on or off
      }
   }
}
#endif


// Purpose:    Turn a pixel on a graphic LCD on or off
// Inputs:     1) x - the x coordinate of the pixel
//             2) y - the y coordinate of the pixel
//             3) color - ON or OFF
void glcd_pixel(int8 x, int8 y, int1 color)
#ifdef FAST_GLCD
{
   int8* p;
   int16 temp;
   temp =  y/8;
   temp *= 64;
   temp += x;

   if(x > 63)
   {
      p = displayData.right + temp - 64;
   }
   else
   {
      p = displayData.left + temp;
   }

   if(color)
   {
      bit_set(*p, y%8);
   }
   else
   {
      bit_clear(*p, y%8);
   }
}
#else
{
   BYTE data;
   int1 side = GLCD_LEFT;  // Stores which chip to use on the LCD

   if(x > 63)              // Check for first or second display area
   {
      x -= 64;
      side = GLCD_RIGHT;
   }

   output_low(GLCD_DI);                         // Set for instruction
   bit_clear(x,7);                              // Clear the MSB. Part of an instruction code
   bit_set(x,6);                                // Set bit 6. Also part of an instruction code
   glcd_writeByte(side, x);                     // Set the horizontal address
   glcd_writeByte(side, (y/8 & 0xBF) | 0xB8);   // Set the vertical page address
   output_high(GLCD_DI);                        // Set for data
   glcd_readByte(side);                         // Need two reads to get data
   data = glcd_readByte(side);                  //  at new address

   if(color == ON)
      bit_set(data, y%8);        // Turn the pixel on
   else                          // or
      bit_clear(data, y%8);      // turn the pixel off

   output_low(GLCD_DI);          // Set for instruction
   glcd_writeByte(side, x);      // Set the horizontal address
   output_high(GLCD_DI);         // Set for data
   glcd_writeByte(side, data);   // Write the pixel data
}
#endif


// Purpose:    Fill the LCD screen with the passed in color
// Inputs:     ON  - turn all the pixels on
//             OFF - turn all the pixels off
void glcd_fillScreen(int1 color)
#ifdef FAST_GLCD
{
   int8  data;
   int8  *p1, *p2;
   int16 i;

   p1 = displayData.left;         
   p2 = displayData.right;
   data = 0xFF * color;

   for(i=0; i<512; ++i)
   {
      *p1++ = data;
      *p2++ = data;
   }
}
#else
{
   int8 i, j;

   // Loop through the vertical pages
   for(i = 0; i < 8; ++i)
   {
      output_low(GLCD_DI);                      // Set for instruction
      glcd_writeByte(GLCD_LEFT, 0b01000000);    // Set horizontal address to 0
      glcd_writeByte(GLCD_RIGHT, 0b01000000);
      glcd_writeByte(GLCD_LEFT, i | 0b10111000);// Set page address
      glcd_writeByte(GLCD_RIGHT, i | 0b10111000);
      output_high(GLCD_DI);                     // Set for data

      // Loop through the horizontal sections
      for(j = 0; j < 64; ++j)
      {
         glcd_writeByte(GLCD_LEFT, 0xFF*color);  // Turn pixels on or off
         glcd_writeByte(GLCD_RIGHT, 0xFF*color); // Turn pixels on or off
      }
   }
}
#endif


// Purpose:    Write a byte of data to the specified chip
// Inputs:     1) chipSelect - which chip to write the data to
//             2) data - the byte of data to write
void glcd_writeByte(int1 side, BYTE data)
{
   if(side)                   // Choose which side to write to
      output_high(GLCD_CS2);
   else
      output_high(GLCD_CS1);

   output_low(GLCD_RW);       // Set for writing
   output_D(data);            // Put the data on the port
   delay_cycles(1);
   output_high(GLCD_E);       // Pulse the enable pin
   delay_cycles(5);
   output_low(GLCD_E);

   output_low(GLCD_CS1);      // Reset the chip select lines
   output_low(GLCD_CS2);
}


// Purpose:    Reads a byte of data from the specified chip
// Ouputs:     A byte of data read from the chip
BYTE glcd_readByte(int1 side)
{
   BYTE data;                 // Stores the data read from the LCD

   set_tris_D(0xFF);          // Set port d to input
   output_high(GLCD_RW);      // Set for reading
                                                                                                         
   if(side)                   // Choose which side to write to
      output_high(GLCD_CS2);
   else
      output_high(GLCD_CS1);

   delay_cycles(1);
   output_high(GLCD_E);       // Pulse the enable pin
   delay_cycles(4);    
   data = input_D();          // Get the data from the display's output register
   output_low(GLCD_E);

   output_low(GLCD_CS1);      // Reset the chip select lines
   output_low(GLCD_CS2);
   return data;               // Return the read data
}

#endif
