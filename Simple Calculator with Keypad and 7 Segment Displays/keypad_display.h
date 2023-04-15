#define col1			pin_A1
#define col2			pin_A2
#define col3			pin_A3       
#define col4			pin_A5  
#define col5			pin_E0
#define col6			pin_E1
#define col7			pin_E2                        
#define col8			pin_C0                         
                              
#define sw0				!input(pin_B0) 
#define sw1             !input(pin_B2)       
#define sw2				!input(pin_B4)       
#define sw3				!input(pin_B5)                 
                                
                        
#use fast_io(A)
#use fast_io(B)  
#use fast_io(C)
#use fast_io(D)
#use fast_io(E)                                   

                                     
const unsigned char num[0x0A] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};                    
                                                                                                                   
unsigned char buffer[0x08] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}; 

unsigned char pos = 0x00; 
unsigned char key = 0xFF;

short key_pressed = FALSE;
                           

void segmented_display_init();
void clear_buffer();
