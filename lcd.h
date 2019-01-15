void lcd_cmd(void)
{
    RS_PIN = 0;
    __delay_ms(100);
    lcd_clock_byte(0x80);
    __delay_ms(2);
}

void lcd_toggle()
{
    E_PIN = 1;
    E_PIN = 0;
    __delay_us(100);
}

void lcd_clock_nibble(unsigned char nibble)
{
    D4_PIN = ((nibble & 0b1   ) > 0);
    D5_PIN = ((nibble & 0b10  ) > 0);
    D6_PIN = ((nibble & 0b100 ) > 0);
    D7_PIN = ((nibble & 0b1000) > 0);
    lcd_toggle();
}

void lcd_clock_byte(unsigned char byte)
{
    lcd_clock_nibble(byte>>4);
    __delay_us(50);
    lcd_clock_nibble(byte);
    __delay_us(50);
}

void lcd_init(void)
{
    RS_TRIS = 0;
    E_TRIS=0;
    D4_TRIS=0;
    D5_TRIS=0;
    D6_TRIS=0;
    D7_TRIS=0;
    
    E_PIN = 0;
    RS_PIN = 0;
    __delay_ms(100);
    
    lcd_clock_nibble(0x03);
    __delay_ms(5);
    lcd_clock_nibble(0x03);
    __delay_us(200);
    
    lcd_clock_nibble(0x03);
    __delay_us(200);
    lcd_clock_nibble(0x02);
    __delay_ms(5);
    
    lcd_clock_byte(0x28);
    lcd_clock_byte(0x08);
    lcd_clock_byte(0x0C);
    lcd_clock_byte(0x06);
    lcd_clock_byte(0x01);
    lcd_clock_byte(0x80);
    __delay_ms(5);
}
void lcd_print(char txt[])
{
    unsigned l = strlen(txt);
    RS_PIN = 1;
    unsigned i;
    for(i=0;i!=l;i++)
    {
        lcd_clock_byte(txt[i]);
        
    }
}
void lcd_clr()
{
    RS_TRIS = 0;
    RS_PIN = 0;
    lcd_clock_byte(0x01);
    __delay_ms(10);
}
void lcd_numXY(int row, int col, int num)
{
	char s[20];
	sprintf(s, "%4d", num);

	RS_PIN=0;
	switch(row){
		case 0:
			lcd_clock_byte(0x80+row+col);
			break;
		case 1:
			lcd_clock_byte(0x80+0x40+col);
			break;
		case 2:
			lcd_clock_byte(0x80+0x14+col);
			break;
		case 3:
			lcd_clock_byte(0x80+0x54+col);
			break;
	}
	lcd_print(s);
}

