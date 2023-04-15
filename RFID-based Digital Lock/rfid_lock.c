#include <16F876A.h>         
                  
                        
#fuses NOWDT, PROTECT, CPD, NOWRT, HS     
#fuses NOBROWNOUT, NOLVP, NODEBUG, PUT      
                                                
                                                     
#use delay (clock = 10MHz)                  
#use RS232(baud = 9600, UART1) 
                                  
                                                                
#include <stdlib.h>                                                                   
#include <lcd420.c>            
#include "sound.c"                                                   
             
                                       
#define EE_Code     0x00002100 
#define EE_ID       0x00002104 
                    
#define ENTER       input(pin_A0)            
#define INC         input(pin_A1)            
#define DEC         input(pin_A2)                                         

#define LED         pin_C0                        
#define LOCK        pin_C1  
                       
#define ID_length	16         
                                                 
                                
#ROM    0x00002100  = {0x00}
#ROM    0x00002101  = {0x00}    
#ROM    0x00002102  = {0x00} 
#ROM    0x00002103  = {0x00}  
                                 
#ROM    0x00002104  = {0x7E}                                              
#ROM    0x00002105  = {0x34}
#ROM    0x00002106  = {0x33}         
#ROM    0x00002107  = {0x30} 
#ROM    0x00002108  = {0x30}
#ROM    0x00002109  = {0x42}            
#ROM    0x0000210A  = {0x37} 
#ROM    0x0000210B  = {0x35}          
#ROM    0x0000210C  = {0x31}     
#ROM    0x0000210D  = {0x44}       
#ROM    0x0000210E  = {0x32}  
#ROM    0x0000210F  = {0x37}         
#ROM    0x00002110  = {0x37}    
#ROM    0x00002111  = {0x32}  
#ROM    0x00002112  = {0x37}             
#ROM    0x00002113  = {0x37}  
                                         
                               
    
const unsigned char symbols[24] =                              
{       
    14, 17, 17, 17, 31, 31, 14, 00,          
    14, 17, 01, 01, 31, 31, 14, 00,
    30, 06, 30, 06, 31, 17, 31, 00                                              
};  

unsigned char card_ID[ID_length];
unsigned char set_ID[ID_length];  
unsigned char set_code[4];
                          
short card_found = FALSE;               
short check_card = FALSE; 

                                                   
void setup(); 
void get_RFID_data();
unsigned char inc_dec(unsigned char x_pos, unsigned char y_pos, signed char value, signed char max, signed char min);
void read_codes();                               
void lcd_symbols();
unsigned char get_input();  
void display_RFID_info();
void setup_init(); 
void check_card_ID();  
short get_password();
void make_password();
void lock_task(); 
void unlock_task();
               
                              
                       
void main()                        
{                                                             
    setup();                  
    read_codes();  
    setup();                             
    while(TRUE)                     
    {                                         
    	if(get_input() == 1)
    	{
    		delay_ms(200);
    		setup_init();
    	}                   
    	else
    	{    
    		get_RFID_data();                                                          
    		if(!card_found)
    		{                           
    			display_RFID_info();            
    		}         
    		if(check_card) 
    		{   
    			check_card_ID();   
    		}                         
    	}
    };                   
}    


void setup()                        
{                                         
    disable_interrupts(global);  
    lock_task(); 
    setup_comparator(NC_NC_NC_NC);         
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);  
    setup_timer_0(0|0|0);
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);
    setup_SPI(SPI_disabled);                       
    setup_CCP1(CCP_off);
    set_timer0(0);
    set_timer1(0);                         
    set_timer2(0);
    lcd_init();                    
    lcd_symbols();
    memset(set_code, 0x00, sizeof(set_code)); 
    memset(card_ID, 0x00, sizeof(card_ID));
	memset(set_ID, 0x00, sizeof(set_ID));  
    delay_ms(200);                                            
} 

                                          
void get_RFID_data()                 
{                             
	unsigned char s = 0;  
           
	if(kbhit() && !card_found)                  
    {                                                                                                   
		memset(card_ID, 0x00, sizeof(card_ID));
        for(s = 0; s < ID_length; s += 1)
        {                                                                          
        	card_id[s] = getc();
        }                                                                 
        lcd_putc("\f");                                                                                                                         
        lcd_gotoxy(4, 1);                        
        lcd_putc("RFID no. :");      
        for(s = 0; s < ID_length; s += 1)
        {                                                               
            if((card_ID[s] >= 48) && (card_ID[s] <= 90))                                                 
            {                  
                lcd_gotoxy((s + 2), 2);
                printf(lcd_putc, "%1c ", card_ID[s]);          
            }                                   
        }                                                                                                                  
        card_found = TRUE;                  
        check_card = TRUE;                   
        generate_tone(1500, 40);                  
        delay_ms(2400);                    
    } 
}

                                                                                
unsigned char inc_dec(unsigned char x_pos, unsigned char y_pos, signed char value, signed char max, signed char min)
{
	while(TRUE)
	{
		lcd_gotoxy(x_pos, y_pos);
		printf(lcd_putc, "%1u ", value);  
	   	
		if(get_input() == 3)
		{
			value -= 1;
		} 
		if(value < min)                  
		{
			value = max;
		} 
		if(get_input() == 2)          
		{
			value += 1;
		} 
		if(value > max)
		{
			value = min;
		} 
		if(get_input() == 1)
		{
			return value;
		} 
	}
}

                                      
void read_codes()                     
{                                        
	unsigned char s = 0;
	memset(set_code, 0x00, sizeof(set_code)); 
    memset(set_ID, 0x00, sizeof(set_ID));   
   	
	for(s = 0; s < 4; s += 1)  
	{   
		set_code[s] = read_EEPROM(EE_Code + s);  
		delay_ms(10);  	    
	}
	for(s = 0; s < ID_length; s += 1)  
	{                                               
		set_ID[s] = read_EEPROM(EE_ID + s); 
		delay_ms(10);  
	}        
}   


void lcd_symbols()                                
{                                 
   unsigned char s = 0;  
   
   lcd_send_byte(0, 0x40);                                   
   for(s = 0; s < sizeof(symbols); s += 1)
   {
        lcd_send_byte(1, symbols[s]);
   }                               
   lcd_send_byte(0, 0x80);
} 


unsigned char get_input()
{
    if(ENTER)                       
    {                                              
        generate_tone(900, 30);  
        return 1; 
    }
    else if(INC)
    {                     
        generate_tone(800, 20);     
        return 2;              
    }
    else if(DEC)
    {                   
        generate_tone(600, 20); 
        return 3;
    }                            
    else
    { 
        return 0;                           
    }
}                   
                      

void display_RFID_info()         
{                                    
	lcd_gotoxy(5, 1);       
	lcd_putc("Locked");    
	lcd_gotoxy(12, 1);          
	lcd_putc(0);                  
	lcd_gotoxy(2, 2);          
	lcd_putc(">Enter = Setup");   
}                      
                                
                              
void setup_init()     
{                               
	short check1 = FALSE;
	short check2 = FALSE;
	unsigned char n = 0;
   	
	lock_task();  
	check1 = get_password();
	                                        
	if(check1)              
	{           
		lcd_putc("\f");
		lcd_gotoxy(1, 1);                                                       
		lcd_putc("Inc> Change Code"); 
		lcd_gotoxy(1, 2);                       
		lcd_putc("Dec> Change Card");  
		while((get_input() != 2) && (get_input() != 3));    
		lcd_putc("\f"); 
		if(get_input() == 2) 
		{      
			check2 = get_password();
			if(check2)
			{                                  
				lcd_putc("\f"); 
				lcd_gotoxy(3, 1);   
				lcd_putc("New Password");    
				delay_ms(600);
				make_password(); 
			}
			else
			{     
				lcd_putc("\f");
				lcd_gotoxy(1, 1);   
				lcd_putc("Password Change");
				lcd_gotoxy(1, 2);   
				lcd_putc("Unsuccessful!");   
				delay_ms(1600);  
				lcd_putc("\f");
			}
		}        
		if(get_input() == 3)    
		{                    
			while(TRUE)
    		{               
    			lcd_gotoxy(1, 1);  
    			lcd_putc("Waiting for card");      
    			lcd_gotoxy(1, 2);               
    			lcd_putc("to be authorized");    
    			get_RFID_data(); 
    			if(card_found)
    			{
    				break;      
    			}
    		}                     
    		check_card = FALSE;       
    		for(n = 0; n < ID_length; n += 1)
    		{     

	    			output_toggle(LED);                              
	    			write_eeprom((EE_ID + n), (card_ID[n]));
	    			delay_ms(40);                  
    		}                    
		}
		output_low(LED);
		read_codes();  
	}                                
	else
	{  
		lcd_putc("\f");
		lcd_gotoxy(6, 1);   
		lcd_putc("Access");  
		lcd_gotoxy(6, 2);   
		lcd_putc("Denied"); 
		delay_ms(2000);  
	}
	card_found = FALSE; 
	check_card = FALSE;               
	lcd_putc("\f"); 
}                        


void check_card_ID()
{
	unsigned char n = 0;
	unsigned char match = 0;   
   	
	read_codes();             
	lcd_putc("\f");
   	
	for(n = 0; n < ID_length; n += 1)
	{                   
		if(set_ID[n] == card_ID[n])               
		{
			match += 1;	   
		}
		else                 
		{
			match = 0;                                    
		}                     
	}         
	if(match == ID_length)
	{
		generate_tone(1200, 40); 
		unlock_task(); 
		lcd_putc("\f");
		lcd_gotoxy(5, 1);
		lcd_putc("Card/Tag");                         
		lcd_gotoxy(5, 2);
		lcd_putc("Accepted");       
		unlock_task(); 
		delay_ms(2000); 
		lcd_putc("\f");
		lcd_gotoxy(4, 1);
		lcd_putc("Unlocked");
		lcd_gotoxy(13, 1);          
		lcd_putc(1);
		n = 30;                   
		while(n != 0)
		{
			lcd_gotoxy(3, 2);
			printf(lcd_putc, "Timeout: %02us", n);
			generate_tone(1600, 10);   
			delay_ms(990); 
			n -= 1;
		}    
	}   
	else
	{   	
		lcd_putc("\f");
		lcd_gotoxy(5, 1);      
		lcd_putc("Card/Tag");
		lcd_gotoxy(4, 2);                
		lcd_putc("Unaccepted"); 
		for(n = 0; n < 4; n += 1)                
        {                   
            generate_tone(2200, 200); 
            output_toggle(LED);
            generate_tone(3200, 200);  
        }                               
		delay_ms(1000);                    
	}                           
	card_found = FALSE;     
	check_card = FALSE;  
	lock_task(); 
	lcd_putc("\f");  
}                                     
                                                      

short get_password()
{     
	unsigned char n = 0;   
	unsigned char match = 0;                                                        
	unsigned char password[4];   
   	
	memset(password, 0x00, sizeof(password));                        
	read_codes();
	                      
	lcd_putc("\f");           
	lcd_gotoxy(4, 1);                  
	lcd_putc("Password");   
	lcd_gotoxy(13, 1);                  
	lcd_putc(2); 
	for(n = 0; n < 4; n += 1)
	{                             
		password[n] = inc_dec((7 + n), 2, password[n], 9, 0);    
		lcd_gotoxy((7 + n), 2); 
		lcd_putc("*");  
		delay_ms(400);
	   	
	}         
   	
	for(n = 0; n < 4; n += 1)
	{   
		if(password[n] == set_code[n]) 
		{
			match += 1;         
		}
		else
		{
			match = 0;
		}
	}
   	
	lcd_putc("\f");  
	if(match == 4)   
	{
		lcd_gotoxy(5, 1);
		lcd_putc("Password");  
		lcd_gotoxy(5, 2);
		lcd_putc("Accepted"); 
		generate_tone(1400, 100);  
		delay_ms(1000);    
		return 1;            
	}                                                          
	else                   
	{   
		lcd_gotoxy(2, 1);
		lcd_putc("Wrong Password"); 
		for(n = 0; n < 4; n += 1)
        {                   
            generate_tone(2500,200); 
            output_toggle(LED);
            generate_tone(3500,200);  
        }    
		lcd_putc("\f"); 
		return 0;
	}
}


void make_password()
{                               
	unsigned char n = 0; 
	unsigned char password[4];
   	
	memset(password, 0x00, sizeof(password));   
	for(n = 0; n < 4; n += 1)
	{                             
		password[n] = inc_dec((7 + n), 2, password[n], 9, 0);
		write_eeprom((EE_code + n), (password[n]));
		delay_ms(600); 
	}                  
	lcd_putc("\f"); 
	lcd_gotoxy(5, 1);                
	lcd_putc("Password"); 
	lcd_gotoxy(5, 2);
	lcd_putc("Changed!");
	delay_ms(1600);
	lcd_putc("\f");        
} 

                                         
void lock_task()
{                      
	output_low(LED);
	output_low(LOCK);       
	generate_tone(600, 10); 
}                          

                        
void unlock_task()
{   
	output_high(LED);
	output_high(LOCK);
	generate_tone(1800, 10); 
}                     
                            
                           
