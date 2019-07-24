#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#define lcd PORTB
#include<my_library/lcd.h>
#include<iot.h>
#include<string.h>

unsigned char keypad()
{	
	while(1)
	{
		PORTC= 0x0e;
		_delay_ms(5);

		if((PINA&0x01)==0){while(!(PINA&0x01)); return '7';}
		if((PINA&0x02)==0){while(!(PINA&0x02)); return '8';}
		if((PINA&0x04)==0){while(!(PINA&0x04)); return '9';}
		if((PINA&0x08)==0){while(!(PINA&0x08)); return 'A';}
		PORTC= 0x0d;
		_delay_ms(5);
		if((PINA&0x01)==0){while(!(PINA&0x01)); return '4';}
		if((PINA&0x02)==0){while(!(PINA&0x02)); return '5';}
		if((PINA&0x04)==0){while(!(PINA&0x04)); return '6';}
		if((PINA&0x08)==0){while(!(PINA&0x08)); return 'B';}
		PORTC= 0x0b;
		_delay_ms(5);
		if((PINA&0x01)==0){while(!(PINA&0x01)); return '1';}
		if((PINA&0x02)==0){while(!(PINA&0x02)); return '2';}
		if((PINA&0x04)==0){while(!(PINA&0x04)); return '3';}
		if((PINA&0x08)==0){while(!(PINA&0x08)); return 'C';}
		PORTC= 0x07;
		_delay_ms(5);
		if((PINA&0x01)==0){while(!(PINA&0x01)); return '*';}
		if((PINA&0x02)==0){while(!(PINA&0x02)); return '0';}
		if((PINA&0x04)==0){while(!(PINA&0x04)); return '#';}
		if((PINA&0x08)==0){while(!(PINA&0x08)); return 'D';}
	}
}

void SMS()
{

    uart_string("AT+CMGF=1\r\n");
	_delay_ms(1000);
	uart_string("AT+CMGS=\"9821511509\"\r\n");
	_delay_ms(1000);
	uart_string("ALERT!!!\r\n");
	uart_string("Unauthorized activity detected at your home.\r\n");
	_delay_ms(300);
	uart_send(0x1A);
	_delay_ms(300);
}

/*
void server(unsigned int x)
{
	uart_string("AT+CIPCSGP=1,\"www\"\r\n");
	_delay_ms(500);
	uart_string("AT+CIPMUX=0\r\n");
	_delay_ms(500);
	uart_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	_delay_ms(5000);
	uart_string("AT+CIPSEND\r\n");
	_delay_ms(5000);
	uart_string("GET https://api.thingspeak.com/update?api_key=PAD6VYGNQ3CBHMEZ&field1=0");
	uart_send(x+48);
	uart_string("\r\n");
	_delay_ms(300);
	uart_send(0x1A);
	_delay_ms(300);
	uart_string("AT+CIPSHUT\r\n");
	_delay_ms(5000);
}*/

void main()
{
	DDRB=0xff;  //for lcd
	DDRA=0b11100000;  //input pins for keypad and PIR sensor
	DDRC=0xff; //for keypad
	uart_init();
	lcd_init();
	lcd_command(0x80);
	unsigned char pwd[]="1111";   //actual pwd
	unsigned char user[5]={0,0,0,0};		//pwd to be entered by the user
	unsigned char val=0;
	_delay_ms(2000);
	_delay_ms(2000);
//	_delay_ms(2000);
	while(1)
	{
		
		PORTA=0b00001111;
		if((PINA&0x10)==0x10)
		{
	
				
				PORTA=0b10001111;
				lcd_string("Enter password:");
				PORTA=0b10001111;
				for(int i=0;i<4;i++)
				{
					PORTA=0b10001111;
					val=keypad();
					user[i]=val;
					lcd_command(0xc0+i);
					lcd_data('*');
				}
				if(strncmp(user,pwd,4)==0) 
				{
					

					PORTA=0x0f;
					lcd_command(0x01); //clear lcd
					lcd_command(0x80);
					lcd_string("Welcome!");
					//server(0);
				} 
				else if(strncmp(user,pwd,4)!=0)
				{
					PORTA=0x8f;
					SMS();
					lcd_command(0x01);
					lcd_command(0x80);
					lcd_string("Wrong Code");
					//server(1);
					PORTA=0b10001111;
					PORTA=0b10101111;
				}
			 	_delay_ms(5000);
				lcd_command(0x01);  //clear lcd
			
		}
		else if((PINA&0x10)!=0x10)
		{
			PORTA=0b00001111;
			lcd_command(0x01);
		}
	}			
}
