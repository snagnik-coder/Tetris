#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions

//Initializing the global variables
unsigned int high_score=0;
int block=0;
int angle=0;
int a=192;
int b=128;
int xp=15;
int yp=0;
int test=0;
unsigned int score=0;
int filled_0=-1;
int filled_1=-1;
int temp=0;
int temp1=0;
int lfsr=10;
unsigned char sc;

void main(void);
bit game_over(void);
void lfsr_generate(void);
void up(void);
void down(void);
void rotate(void);
void new_block(void);
void print_lcd(void);

//Checking if the lcd is filled
bit game_over(void)
{
    if(filled_0>13||filled_1>13)
    {
        return 1;
    }
    return 0;
}
		
//Generating values for angle and block
void lfsr_generate(void)
{  
    temp = (lfsr&0x8);
    temp1 = ((lfsr&0x1)<<3);
    lfsr = (lfsr>>1);
    lfsr = lfsr+(temp^temp1);
}

//move the block up if possible
void up(void)
{
    if(yp%2 == 0)
        yp = yp + 1;
    test=1;
    print_lcd();
}

//move the block down if possible
void down(void)
{
    if(yp%2 == 1)
        yp = yp - 1;
    test=1;
    print_lcd();
}

//rotate the block if necessary
void rotate(void)
{
	if (block != 0)
	{
		if(yp%2 == 0)
        yp = yp + 1;
    test=1;
		angle = angle + 1;
		
	}
}	

//generating new block with lfsr_generate()
void new_block(void)
{
    lfsr_generate();
    block = lfsr%3;
    angle = lfsr%2;
    xp = 15;
    yp = lfsr%2;
}

//printing to the lcd
void print_lcd(void)
{
	//checking if game is over and displaying score
	if (game_over())
	{
				if(score>=high_score)
            high_score = score;
				lcd_cmd(0x01);   
        msdelay(4);
        lcd_cmd(0x80);
        msdelay(4);
        lcd_write_string("Score:");
        msdelay(4);
        int_to_string(score, &sc);
        lcd_write_string(&sc);
        msdelay(4);
        lcd_cmd(0xC0);
        msdelay(4);
        lcd_write_string("High_Score:");
        msdelay(4);
				int_to_string(high_score, &sc);
        lcd_write_string(&sc);
				msdelay(3000);
				lcd_cmd(0x01);
				filled_0 = -1;
				filled_1 = -1;
				xp = 15;
				score = 0;
				new_block();
	}
	//If the next space is filled
	else if ((xp == -1) || (block == 0 && yp == 0 && filled_0 == xp) || (block == 0 && yp== 1 && filled_1 == xp) || (block == 1 && angle%2 == 0 && yp == 0 && filled_0 == xp) || (block == 1 && angle%2 == 1 &&(filled_0 == xp || filled_1 == xp)) || (block == 2 && (angle % 4 == 0 || angle % 4 == 3) && (filled_0 == xp || filled_1 == xp)) || (block == 2 && angle % 4 == 1 && (filled_1 == xp || filled_0 == xp + 1)) || (block == 2 && angle % 4 == 2 && (filled_1 == xp + 1 || filled_0 == xp)))
	{
		if (block == 0)
		{
			if (yp == 0)
			{
				filled_0 = filled_0 + 1;
			}
			else 
			{
				filled_1 = filled_1 + 1;
			}
		}
		else if (block == 1)
		{
			if (angle%2 == 0)
			{
				if (yp == 0)
				{
					filled_0 = filled_0 + 2;
				}
				else 
				{
					filled_1 = filled_1 + 2;
				}
			}
			else
			{
				filled_0++;
				filled_1++;
				if (filled_0 > filled_1)
				{
					filled_1 = filled_0;
				}
				else
				{
					filled_0 = filled_1;
				}
				
			}
		}
		else
		{
			if (angle%4 == 0)
			{
				filled_0 = filled_0 + 1;
				filled_1 = filled_1 + 2;
			}
			else if (angle%4 == 1)
			{
				if (xp == filled_0)
				{
					filled_0 = filled_0 + 1;
					filled_1 = filled_1 + 2;
				}
				else
				{
					filled_0 = filled_0 + 2;
					filled_1 = filled_1 + 2;
				}
			}
			else if (angle%4 == 2)
			{
				if (xp == filled_1)
				{
					filled_0 = filled_0 + 2;
					filled_1 = filled_1 + 1;
				}
				else
				{
					filled_0 = filled_0 + 2;
					filled_1 = filled_1 + 2;
				}
			}
			else
			{
				filled_0 = filled_0 + 2;
				filled_1 = filled_1 + 1;
			}
		}
		new_block();
	}
	//Clear the previous space of the block and print the new space
	else
	{
		lcd_cmd(a + xp + 1);
		msdelay(4);
		lcd_write_string("  ");
		msdelay(4);
		lcd_cmd(b + xp + 1);
		msdelay(4);
		lcd_write_string("  ");
		if(block == 0)
		{
			if (yp == 0)
			{
				lcd_cmd(a + xp);
				msdelay(4);
				lcd_write_string("*");
				msdelay(4);
			}
			else
			{
				lcd_cmd(b + xp);
				msdelay(4);
				lcd_write_string("*");
				msdelay(4);
			}
		}
		else if(block == 1)
		{
			if (angle%2 == 0)
			{
				if (yp == 0)
				{
					lcd_cmd(a + xp);
					msdelay(4);
					lcd_write_string("**");
					msdelay(4);
				}
				else 
				{
					lcd_cmd(b + xp);
					msdelay(4);
					lcd_write_string("**");
					msdelay(4);
				}
			}
			else
			{
				lcd_cmd(a + xp);
				msdelay(4);
				lcd_write_string("*");
				msdelay(4);
				lcd_cmd(b + xp);
				msdelay(4);
				lcd_write_string("*");
				msdelay(4);
			}
		}
		else
		{
			if (angle%4 == 0)
			{
				lcd_cmd(a + xp);
				msdelay(4);
				lcd_write_string("*");
				msdelay(4);
				lcd_cmd(b + xp);
				msdelay(4);
				lcd_write_string("**");
				msdelay(4);
			}
			else if (angle%4 == 1)
			{
				lcd_cmd(a + xp);
				msdelay(4);
				lcd_write_string(" *");
				msdelay(4);
				lcd_cmd(b + xp);
				msdelay(4);
				lcd_write_string("**");
				msdelay(4);
			}
			else
			{
				lcd_cmd(a + xp);
				msdelay(4);
				lcd_write_string("**");
				msdelay(4);
				lcd_cmd(b + xp);
				msdelay(4);
				lcd_write_string(" *");
				msdelay(4);
			}
		}
	}
}				

void main(void)
{
	unsigned char ch;
	//Call initialization functions
	lcd_init();
	uart_init();

	while(1)
	{
			ch=' ';
		  print_lcd();
			msdelay(1000);
		  score++;
			xp--;
			if(rx_complete==1)
			ch = receive_char();
			switch(ch)
			{
				case 'q':up();
								 
								 break;
				
				case 'a':down();
								 
								 break;
				
				case 'r':rotate();
								break;
							
				default:break;
								 
				
			}
			msdelay(4);		
	}			

}