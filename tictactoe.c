#include "TFT.h"
#include "YP.h"
#include "YN.h"
#include "XP.h"
#include "XN.h"
#include "SPI.h"
#include "DC.h"
#include "CS.h"
#include "BCK.h"
#include "AMux.h"
#include "ADC.h"
#include "SPI_SPI_UART.h"

/* Variables needed to detect the position */
uint16 Line, ADC_Resultx, ADC_Resulty,Touch_X, Touch_Y,count=0,ADC_Detect=0;

/* Variables used to define box positions */
uint8 box1=1,box2=1,box3=1,box4=1,box5=1,box6=1,box7=1,box8=1,box9=1;

/* Variables to store ADC values and the number of times finger has been pressed */
uint8 xlsb, xmsb, ylsb, ymsb, maincount; 

/* Variables to compute the winner */
uint8 winx1=0,winx2=0,winx3=0,winx4=0,winx5=0,winx6=0,winx7=0,winx8=0,winx9=0;
uint8 wino1=0,wino2=0,wino3=0,wino4=0,wino5=0,wino6=0,wino7=0,wino8=0,wino9=0;

void TFT_Start(void)
{
    CS_HIGH;
	DC_HIGH;

	CyDelay(500);
	Send_Cmd(0x01);
	CyDelay(200);
	
	Send_Cmd(0xCB);
    Send_Data(0x39);
    Send_Data(0x2C);
    Send_Data(0X00);
    Send_Data(0X34);
    Send_Data(0X02);

    Send_Cmd(0xCF);
    Send_Data(0x00);
    Send_Data(0xC1);
    Send_Data(0X30);
    

    Send_Cmd(0xE8);
    Send_Data(0x85);
    Send_Data(0x00);
    Send_Data(0x78);

    Send_Cmd(0xEA);
    Send_Data(0x64);
    Send_Data(0x03);
    Send_Data(0x12);
    Send_Data(0x81);
   

    Send_Cmd(0xF7);
    Send_Data(0x20);

    Send_Cmd(0xEA);
    Send_Data(0x00);
    Send_Data(0x00);

    Send_Cmd(0xC0);                                                      /* Power control                */
    Send_Data(0x1B);                                                   /* VRH[5:0]                     */

    Send_Cmd(0xC1);                                                      /* Power control                */
    Send_Data(0x10);                                                   /* SAP[2:0];BT[3:0]             */

    Send_Cmd(0xC5);                                                      /* VCM control                  */
    Send_Data(0x3F);
    Send_Data(0x3C);

    Send_Cmd(0xC7);                                                      /* VCM control2                 */
    Send_Data(0XB7);

    Send_Cmd(0x36);                                                      /* Memory Access Control        */
    Send_Data(0x08);

    Send_Cmd(0x3A);
    Send_Data(0x55);

    Send_Cmd(0xB1);
    Send_Data(0x00);
    Send_Data(0x1B);

    Send_Cmd(0xB6);                                                      /* Display Function Control     */
    Send_Data(0x0A);
    Send_Data(0xA2);


    Send_Cmd(0xF2);                                                      /* 3Gamma Function Disable      */
    Send_Data(0x00);

    Send_Cmd(0x26);                                                      /* Gamma curve selected         */
    Send_Data(0x01);

    Send_Cmd(0xE0);                                                      /* Set Gamma                    */
    Send_Data(0x0F);
    Send_Data(0x2A);
    Send_Data(0x28);
    Send_Data(0x08);
    Send_Data(0x0E);
    Send_Data(0x08);
    Send_Data(0x54);
    Send_Data(0XA9);
    Send_Data(0x43);
    Send_Data(0x0A);
    Send_Data(0x0F);
    Send_Data(0x00);
    Send_Data(0x00);
    Send_Data(0x00);
    Send_Data(0x00);

    Send_Cmd(0XE1);                                                      /* Set Gamma                    */
    Send_Data(0x00);
    Send_Data(0x15);
    Send_Data(0x17);
    Send_Data(0x07);
    Send_Data(0x11);
    Send_Data(0x06);
    Send_Data(0x2B);
    Send_Data(0x56);
    Send_Data(0x3C);
    Send_Data(0x05);
    Send_Data(0x10);
    Send_Data(0x0F);
    Send_Data(0x3F);
    Send_Data(0x3F);
    Send_Data(0x0F);
	
	Send_Cmd(0x11);                                                      /* Exit Sleep                   */
    CyDelay(120);
    Send_Cmd(0x29);   
  
    Fill_Screen();
    
    }




void Fill_Screen(void)
{
	uint32 index;
	
	Set_Col(0, 160);
	Set_Page(0, 128);
	Send_Cmd(0x2C);
	
	DC_HIGH;
	CS_LOW;
	
	for(index = 0; index < (160* 128); index++)
	{
       SPI_Transfer(0);
		SPI_Transfer(0);
	}
	CS_HIGH;
}


/* The function Draw_Line(x1, y1, x2,y2, color) draws a line between the point (x1,y1) and (x2,y2) with the defined color */

void Draw_Line( uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 Color )
{
	short dx, dy, sdx, sdy, px, py, dxabs, dyabs, i;
	float slope;

	dx = x2 - x1;
	dy = y2 - y1;
	dxabs = sdx = 0;
	
	if( dx > 0 )
	{
		dxabs = dx;
		sdx = 1;
	}
	else if ( dx < 0 )
	{
		dxabs = -dx;
		sdx = -1;
	}
	
	dyabs = sdy = 0;
	
	if( dy > 0 )
	{
		dyabs = dy;
		sdy = 1;
	}
	else if ( dy < 0 )
	{
		dyabs = -dy;
		sdy = -1;
	}
	if ( dxabs >= dyabs ) /* the line is more horizontal than vertical */
	{
		slope = ( float ) dy / ( float ) dx;
		for ( i = 0; i != dx; i += sdx )
		{
			px = i + x1;
			py = slope * i + y1;
			Set_Pixel( px, py, Color );
		}
	}
	else /* the line is more vertical than horizontal */
	{
		slope = ( float ) dx / ( float ) dy;
		for ( i = 0; i != dy; i += sdy )
		{
			px = slope * i + x1;
			py = i + y1;
			Set_Pixel( px, py, Color );
		}
	}
}

/* The function Draw_H_Line(x,y,length, color) draws a horizontal line from (x,y) to (x+length,y) with the defined colour */

void Draw_H_Line(uint16 Pos_X, uint16 Pos_Y, uint16 Length, uint16 Color)
{
	uint16 index;
	
	Set_Col(Pos_X, Pos_X + Length);
	Set_Page(Pos_Y, Pos_Y);
	Send_Cmd(0x2C);
	
	DC_HIGH;
	CS_LOW;
	
	for(index = 0; index < Length; index++)
	{
		SPI_Transfer(Color >> 8);
		SPI_Transfer(Color & 0xFF);
	}
	
	CS_HIGH;
}

/* The function Draw_V_Line(x,y,length, color) draws a vertical line from (x,y) to (x,y+length) with the defined colour */

void Draw_V_Line(uint16 Pos_X, uint16 Pos_Y, uint16 Length, uint16 Color)
{
	uint16 index;
	Set_Col(Pos_X, Pos_X);
	Set_Page(Pos_Y, Pos_Y + Length);
	Send_Cmd(0x2C);
	
	DC_HIGH;
	CS_LOW;
	
	for(index = 0; index < Length; index++)
	{	
		SPI_Transfer(Color >> 8);
		SPI_Transfer(Color & 0xFF);	
	}
	
	CS_HIGH;
}

/* The function Draw_Circle(x,y,r, color) draws a circle of radius "r" with (x,y) as the centre with the defined colour */

void Draw_Circle(uint16 Pos_X, uint16 Pos_Y, uint16 Radius, uint16 Color)
{
	int16 X, Y, E1, E2;
	
	X = -Radius;
	Y = 0;
	E1 = 2 - (X >> 1);
	
	do
	{
		Set_Pixel(Pos_X - X, Pos_Y - Y, Color);
		Set_Pixel(Pos_X - X, Pos_Y + Y, Color);
		Set_Pixel(Pos_X + X, Pos_Y - Y, Color);
		Set_Pixel(Pos_X + X, Pos_Y + Y, Color);
		
		E2 = E1;
		if(E2 <= Y)
		{
			E1 += ++Y * 2 + 1;
			
			if(-X == Y && E2 <= X)
			{
				E2 = 0;
			}	
		}
		
		if(E2 > X)
		{
			E1 += ++X * 2 + 1;
		}	
	}
	while(X <= 0);
} 

/* The function Set_Col(start,end) defines the boundaries for setting a particular column */
void Set_Col(uint16 Start_Col, uint16 End_Col)
{
	Send_Cmd(0x2A);
	Send_Data16(Start_Col);
	Send_Data16(End_Col);
}

/* The function Set_Page(start,end) defines the boundaries for setting a particular page */
void Set_Page(uint16 Start_Page, uint16 End_Page)
{
	Send_Cmd(0x2B);
	Send_Data16(Start_Page);
	Send_Data16(End_Page);
}

/* The function Goto_XY(X,Y) provides access to row X and column Y */
void Goto_XY(uint16 Pos_X, uint16 Pos_Y)
{
	Set_Col(Pos_X, Pos_X);
	Set_Page(Pos_Y, Pos_Y);
	Send_Cmd(0x2C);
}

/* The function Set_Pixel(uint16 Pos_X, uint16 Pos_Y, uint16 Color) highlights the pixel at row "Pos_X" and column "Pos_Y" with "Color" */
void Set_Pixel(uint16 Pos_X, uint16 Pos_Y, uint16 Color)
{
	Goto_XY(Pos_X, Pos_Y);
	Send_Data16(Color);
}


/* The function Send_Data16(uint16) transfers a 16 bit data to the controller in LCD module through SPI */

void Send_Data16(uint16 dat)
{
	uint8 MSB, LSB;
	
	MSB = dat >> 8;
	LSB = dat & 0xFF;
	
	DC_HIGH;
	CS_LOW;
	
	SPI_Transfer(MSB);
	SPI_Transfer(LSB);
	
	CS_HIGH;
}

/* The function Send_Data(uint8) transfers a data to the controller in LCD module through SPI */

void Send_Data(uint8 dat)
{
	DC_HIGH;
	CS_LOW;
	
	SPI_Transfer(dat);
	
	CS_HIGH;
}

/* The function Send_Cmd(uint8) transfers a command to the controller in LCD module through SPI */

void Send_Cmd(uint8 cmd)
{
	DC_LOW;
	CS_LOW;
	
	SPI_Transfer(cmd);
	
	CS_HIGH;
}

/* The function SPI_Transfer(uint8) transfers the data to the controller in LCD module through SPI */

void SPI_Transfer(uint8 data_tx)
{	
	SPI_SpiUartWriteTxData((uint32)data_tx);

	CyDelayUs(1);
}

/* The function TouchDetect() checks if a finger is present or not */
void TouchDetect(void)
{	XP_SetDriveMode(XP_DM_STRONG);
	CyDelayUs(5);
	XP_SetDriveMode(XP_DM_RES_UP);
	XN_SetDriveMode(XN_DM_ALG_HIZ);
	YP_SetDriveMode(YP_DM_ALG_HIZ);
	YN_SetDriveMode(YN_DM_STRONG);
	AMux_Select(1);
	
	while(1)
	{
		ADC_StartConvert();
		ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
		ADC_StopConvert();
		ADC_Detect = ADC_GetResult16(0);
		if(ADC_Detect<=0x600)
		break;
	}
}

/* The function Touch_Location() determines the position where the finger is pressed. */

void Touch_Location(void)
{
	TouchDetect();
	
	XP_SetDriveMode(XP_DM_STRONG);
	XN_SetDriveMode(XN_DM_STRONG);
	
	
	XP_Write(1);
	XN_Write(0);
	
	YP_SetDriveMode(YP_DM_ALG_HIZ);
	YN_SetDriveMode(YN_DM_ALG_HIZ);
	
	AMux_Select(0);
	
	ADC_StartConvert();
	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
	ADC_Resultx = ADC_GetResult16(0);
	
	
	ADC_StopConvert();
	
	
	
	YP_SetDriveMode(YP_DM_STRONG);
	YN_SetDriveMode(YN_DM_STRONG);
	
	
	YP_Write(1);
	YN_Write(0);
	
	XP_SetDriveMode(XP_DM_ALG_HIZ);
	XN_SetDriveMode(XN_DM_ALG_HIZ);
	
	
	
	AMux_Select(1);
	
	ADC_StartConvert();
	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
	ADC_Resulty = ADC_GetResult16(0);
	ADC_StopConvert();
	
	xlsb=0x00FF&ADC_Resultx;
	xmsb=ADC_Resultx>>8;
	ylsb=0x00FF&ADC_Resulty;
	ymsb=ADC_Resulty>>8;

	DisplayPattern();
}

/* The function DisplayPattern() checks the position where the finger is placed and draws an X or an O in that particular box */

void DisplayPattern()
{
	/* Checks if box no:1 has been pressed */
	if((ADC_Resultx>BOX_1_X_THRES_MIN&&ADC_Resultx<BOX_1_X_THRES_MAX)&&(ADC_Resulty>BOX_1_Y_THRES_MIN&&ADC_Resulty<BOX_1_Y_THRES_MAX)&&box1==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_1_CENTRE_X-15, BOX_1_CENTRE_Y-20, BOX_1_CENTRE_X+15, BOX_1_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_1_CENTRE_X+15, BOX_1_CENTRE_Y-20, BOX_1_CENTRE_X-15, BOX_1_CENTRE_Y+20, WHITE);
			winx1=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_1_CENTRE_X,BOX_1_CENTRE_Y, O_RADIUS, RED);
			wino1=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box1=0;
	}
	/* Checks if box no:2 has been pressed */
	if((ADC_Resultx>BOX_2_X_THRES_MIN&&ADC_Resultx<BOX_2_X_THRES_MAX)&&(ADC_Resulty>BOX_2_Y_THRES_MIN&&ADC_Resulty<BOX_2_Y_THRES_MAX)&&box2==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_2_CENTRE_X-15, BOX_2_CENTRE_Y-20, BOX_2_CENTRE_X+15, BOX_2_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_2_CENTRE_X+15, BOX_2_CENTRE_Y-20, BOX_2_CENTRE_X-15, BOX_2_CENTRE_Y+20, WHITE);
			winx2=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_2_CENTRE_X, BOX_2_CENTRE_Y, O_RADIUS, RED);
			wino2=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box2=0;
	}
	/* Checks if box no:3 has been pressed */
	if((ADC_Resultx>BOX_3_X_THRES_MIN&&ADC_Resultx<BOX_3_X_THRES_MAX)&&(ADC_Resulty>BOX_3_Y_THRES_MIN&&ADC_Resulty<BOX_3_Y_THRES_MAX)&&box3==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_3_CENTRE_X-15, BOX_3_CENTRE_Y-20, BOX_3_CENTRE_X+15, BOX_3_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_3_CENTRE_X+15, BOX_3_CENTRE_Y-20, BOX_3_CENTRE_X-15, BOX_3_CENTRE_Y+20, WHITE);
			winx3=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_3_CENTRE_X,BOX_3_CENTRE_Y, O_RADIUS, RED);
			wino3=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box3=0;
	}
	/* Checks if box no:4 has been pressed */
	if((ADC_Resultx>BOX_4_X_THRES_MIN&&ADC_Resultx<BOX_4_X_THRES_MAX)&&(ADC_Resulty>BOX_4_Y_THRES_MIN&&ADC_Resulty<BOX_4_Y_THRES_MAX)&&box4==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_4_CENTRE_X-15, BOX_4_CENTRE_Y-20, BOX_4_CENTRE_X+15, BOX_4_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_4_CENTRE_X+15, BOX_4_CENTRE_Y-20, BOX_4_CENTRE_X-15, BOX_4_CENTRE_Y+20, WHITE);
			winx4=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_4_CENTRE_X,BOX_4_CENTRE_Y, O_RADIUS, RED);
			wino4=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box4=0;
	}
	/* Checks if box no:5 has been pressed */
	if((ADC_Resultx>BOX_5_X_THRES_MIN&&ADC_Resultx<BOX_5_X_THRES_MAX)&&(ADC_Resulty>BOX_5_Y_THRES_MIN&&ADC_Resulty<BOX_5_Y_THRES_MAX)&&box5==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_5_CENTRE_X-15, BOX_5_CENTRE_Y-20, BOX_5_CENTRE_X+15, BOX_5_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_5_CENTRE_X+15, BOX_5_CENTRE_Y-20, BOX_5_CENTRE_X-15, BOX_5_CENTRE_Y+20, WHITE);
			winx5=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_5_CENTRE_X,BOX_5_CENTRE_Y, O_RADIUS, RED);
			wino5=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box5=0;
	}
	/* Checks if box no:6 has been pressed */
	if((ADC_Resultx>BOX_6_X_THRES_MIN&&ADC_Resultx<BOX_6_X_THRES_MAX)&&(ADC_Resulty>BOX_6_Y_THRES_MIN&&ADC_Resulty<BOX_6_Y_THRES_MAX)&&box6==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_6_CENTRE_X-15, BOX_6_CENTRE_Y-20, BOX_6_CENTRE_X+15, BOX_6_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_6_CENTRE_X+15, BOX_6_CENTRE_Y-20, BOX_6_CENTRE_X-15, BOX_6_CENTRE_Y+20, WHITE);
			winx6=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_6_CENTRE_X,BOX_6_CENTRE_Y, O_RADIUS, RED);
			wino6=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box6=0;
	}
	/* Checks if box no:7 has been pressed */
	if((ADC_Resultx>BOX_7_X_THRES_MIN&&ADC_Resultx<BOX_7_X_THRES_MAX)&&(ADC_Resulty>BOX_7_Y_THRES_MIN&&ADC_Resulty<BOX_7_Y_THRES_MAX)&&box7==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_7_CENTRE_X-15, BOX_7_CENTRE_Y-20, BOX_7_CENTRE_X+15, BOX_7_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_7_CENTRE_X+15, BOX_7_CENTRE_Y-20, BOX_7_CENTRE_X-15, BOX_7_CENTRE_Y+20, WHITE);
			winx7=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_7_CENTRE_X,BOX_7_CENTRE_Y, O_RADIUS, RED);
			wino7=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box7=0;
	}
	/* Checks if box no:8 has been pressed */
	if((ADC_Resultx>BOX_8_X_THRES_MIN&&ADC_Resultx<BOX_8_X_THRES_MAX)&&(ADC_Resulty>BOX_8_Y_THRES_MIN&&ADC_Resulty<BOX_8_Y_THRES_MAX)&&box8==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_8_CENTRE_X-15, BOX_8_CENTRE_Y-20, BOX_8_CENTRE_X+15, BOX_8_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_8_CENTRE_X+15, BOX_8_CENTRE_Y-20, BOX_8_CENTRE_X-15, BOX_8_CENTRE_Y+20, WHITE);
			winx8=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_8_CENTRE_X,BOX_8_CENTRE_Y, O_RADIUS, RED);
			wino8=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box8=0;
	}
	/* Checks if box no:9 has been pressed */
	if((ADC_Resultx>BOX_9_X_THRES_MIN&&ADC_Resultx<BOX_9_X_THRES_MAX)&&(ADC_Resulty>BOX_9_Y_THRES_MIN&&ADC_Resulty<BOX_9_Y_THRES_MAX)&&box9==1)
	{
		if(count%2==0)
		{
			Draw_Line(BOX_9_CENTRE_X-15, BOX_9_CENTRE_Y-20, BOX_9_CENTRE_X+15, BOX_9_CENTRE_Y+20, WHITE);
			Draw_Line(BOX_9_CENTRE_X+15, BOX_9_CENTRE_Y-20, BOX_9_CENTRE_X-15, BOX_9_CENTRE_Y+20, WHITE);
			winx9=1;
			maincount++;
			Check_Winner();
		}
		else
		{
			Draw_Circle(BOX_9_CENTRE_X,BOX_9_CENTRE_Y, O_RADIUS, RED);
			wino9=1;
			maincount++;
			Check_Winner();
		}
		count++;
		box9=0;
	}
}

/*
The function Check_Winner() makes the LCD display the winner of the game.
*/
void Check_Winner()
{
	if((winx1==1&&winx2==1&&winx3==1)||(winx1==1&&winx4==1&&winx7==1)||(winx1==1&&winx5==1&&winx9==1)||(winx4==1&&winx5==1&&winx6==1)||(winx7==1&&winx8==1&&winx9==1)||(winx3==1&&winx6==1&&winx9==1)||(winx2==1&&winx5==1&&winx8==1)||(winx3==1&&winx5==1&&winx7==1))
	{
		
		if(winx1==1&&winx2==1&&winx3==1)
			Draw_V_Line(BOX_1_CENTRE_X, BOX_1_CENTRE_Y, BOX_3_CENTRE_Y-BOX_1_CENTRE_Y, WHITE);
			
		else if(winx1==1&&winx4==1&&winx7==1)
			Draw_H_Line(BOX_7_CENTRE_X, BOX_7_CENTRE_Y, BOX_1_CENTRE_X-BOX_7_CENTRE_X, WHITE);
			
		else if(winx1==1&&winx5==1&&winx9==1)
			Draw_Line(BOX_1_CENTRE_X,BOX_1_CENTRE_Y,BOX_9_CENTRE_X,BOX_9_CENTRE_Y, WHITE);
			
		else if(winx4==1&&winx5==1&&winx6==1)
			Draw_V_Line(BOX_4_CENTRE_X, BOX_4_CENTRE_Y, BOX_6_CENTRE_Y-BOX_4_CENTRE_Y, WHITE);
			
		else if(winx7==1&&winx8==1&&winx9==1)
			Draw_V_Line(BOX_7_CENTRE_X, BOX_7_CENTRE_Y, BOX_9_CENTRE_Y-BOX_7_CENTRE_Y, WHITE);
			
		else if(winx3==1&&winx6==1&&winx9==1)
			Draw_H_Line(BOX_9_CENTRE_X, BOX_9_CENTRE_Y, BOX_3_CENTRE_X-BOX_9_CENTRE_X, WHITE);
			
		else if(winx2==1&&winx5==1&&winx8==1)
			Draw_H_Line(BOX_8_CENTRE_X, BOX_8_CENTRE_Y, BOX_2_CENTRE_X-BOX_8_CENTRE_X, WHITE);
			
		else 
			Draw_Line(BOX_3_CENTRE_X,BOX_3_CENTRE_Y,BOX_7_CENTRE_X,BOX_7_CENTRE_Y,WHITE);
			
		CyDelay(5000);
		Fill_Screen();
		
		/*Displays that X won the game */
		Draw_Line(X_BOUNDARY_LOW, Y_1_BOUNDARY_LOW, X_BOUNDARY_HIGH, Y_1_BOUNDARY_HIGH, GREEN);
		Draw_Line(X_BOUNDARY_HIGH, Y_1_BOUNDARY_LOW, X_BOUNDARY_LOW, Y_1_BOUNDARY_HIGH, GREEN);
		Draw_H_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_HIGH, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, WHITE);
		Draw_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_CENTRE, X_BOUNDARY_HIGH, Y_2_BOUNDARY_HIGH, WHITE);
		Draw_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_CENTRE, X_BOUNDARY_HIGH, Y_2_BOUNDARY_LOW, WHITE);
		Draw_H_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_LOW, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, WHITE);
		Draw_Circle(X_BOUNDARY_CENTRE,Y_3_BOUNDARY_CENTRE-10, Y_3_BOUNDARY_HIGH-Y_3_BOUNDARY_LOW, WHITE);
		Draw_H_Line(X_BOUNDARY_LOW, Y_4_BOUNDARY_HIGH, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, WHITE);
		Draw_Line(X_BOUNDARY_LOW, Y_4_BOUNDARY_HIGH, X_BOUNDARY_HIGH, Y_4_BOUNDARY_LOW, WHITE);
		Draw_H_Line(X_BOUNDARY_LOW, Y_4_BOUNDARY_LOW, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, WHITE);
		
		CyDelay(5000);
		/* Starts a new game */
		CySoftwareReset();
	}
	else if((wino1==1&&wino2==1&&wino3==1)||(wino1==1&&wino4==1&&wino7==1)||(wino1==1&&wino5==1&&wino9==1)||(wino4==1&&wino5==1&&wino6==1)||(wino7==1&&wino8==1&&wino9==1)||(wino3==1&&wino6==1&&wino9==1)||(wino2==1&&wino5==1&&wino8==1)||(wino3==1&&wino5==1&&wino7==1))
	{
		if(wino1==1&&wino2==1&&wino3==1)
			Draw_V_Line(BOX_1_CENTRE_X, BOX_1_CENTRE_Y, BOX_3_CENTRE_Y-BOX_1_CENTRE_Y, WHITE);
			
		else if(wino1==1&&wino4==1&&wino7==1)
			Draw_H_Line(BOX_7_CENTRE_X, BOX_7_CENTRE_Y, BOX_1_CENTRE_X-BOX_7_CENTRE_X, WHITE);
			
		else if(wino1==1&&wino5==1&&wino9==1)
			Draw_Line(BOX_1_CENTRE_X,BOX_1_CENTRE_Y,BOX_9_CENTRE_X,BOX_9_CENTRE_Y, WHITE);
			
		else if(wino4==1&&wino5==1&&wino6==1)
			Draw_V_Line(BOX_4_CENTRE_X, BOX_4_CENTRE_Y, BOX_6_CENTRE_Y-BOX_4_CENTRE_Y, WHITE);
			
		else if(wino7==1&&wino8==1&&wino9==1)
			Draw_V_Line(BOX_7_CENTRE_X, BOX_7_CENTRE_Y, BOX_9_CENTRE_Y-BOX_7_CENTRE_Y, WHITE);
			
		else if(wino3==1&&wino6==1&&wino9==1)
			Draw_H_Line(BOX_9_CENTRE_X, BOX_9_CENTRE_Y, BOX_3_CENTRE_X-BOX_9_CENTRE_X, WHITE);
			
		else if(wino2==1&&wino5==1&&wino8==1)
			Draw_H_Line(BOX_9_CENTRE_X, BOX_9_CENTRE_Y, BOX_3_CENTRE_X-BOX_9_CENTRE_X, WHITE);
			
		else 
			Draw_Line(BOX_3_CENTRE_X,BOX_3_CENTRE_Y,BOX_7_CENTRE_X,BOX_7_CENTRE_Y,WHITE);
		
		
		CyDelay(5000);
		Fill_Screen();
		
		/*Displays that O won the game */
		Draw_Circle(X_BOUNDARY_CENTRE,Y_1_BOUNDARY_CENTRE, Y_1_O_RADIUS, GREEN);
		Draw_H_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_HIGH, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, WHITE);
		Draw_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_CENTRE, X_BOUNDARY_HIGH, Y_2_BOUNDARY_HIGH, WHITE);
		Draw_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_CENTRE, X_BOUNDARY_HIGH, Y_2_BOUNDARY_LOW, WHITE);
		Draw_H_Line(X_BOUNDARY_LOW, Y_2_BOUNDARY_LOW, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, WHITE);
		Draw_Circle(X_BOUNDARY_CENTRE,Y_3_BOUNDARY_CENTRE-10, Y_3_BOUNDARY_HIGH-Y_3_BOUNDARY_LOW, WHITE);
		Draw_H_Line(X_BOUNDARY_LOW, Y_4_BOUNDARY_HIGH, Y_4_BOUNDARY_HIGH-Y_4_BOUNDARY_LOW, WHITE);
		Draw_Line(X_BOUNDARY_LOW, Y_4_BOUNDARY_HIGH, X_BOUNDARY_HIGH, Y_4_BOUNDARY_LOW, WHITE);
		Draw_H_Line(X_BOUNDARY_LOW, Y_4_BOUNDARY_LOW, Y_4_BOUNDARY_HIGH-Y_4_BOUNDARY_LOW, WHITE);
		CyDelay(5000);
		/* Starts a new game */
		CySoftwareReset(); 
	}
	else
	{
		/* If the games ends with a draw */
		if(maincount==NO_OF_BOXES)
		{
			CyDelay(5000);
			Fill_Screen();
			Draw_H_Line(X_BOUNDARY_LOW, Y_LEFT_T, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, RED);
			Draw_V_Line(X_BOUNDARY_LOW, Y_RIGHT_T, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, RED);
			Draw_H_Line(X_BOUNDARY_LOW, Y_LINE_I, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, RED);
			Draw_H_Line(X_BOUNDARY_LOW, Y_HIGH_BOUNDARY_E, X_BOUNDARY_HIGH-X_BOUNDARY_LOW, RED);
			Draw_V_Line(X_BOUNDARY_LOW, Y_LOW_BOUNDARY_E, Y_HIGH_BOUNDARY_E-Y_LOW_BOUNDARY_E, RED);
			Draw_V_Line(X_BOUNDARY_CENTRE, Y_LOW_BOUNDARY_E, Y_HIGH_BOUNDARY_E-Y_LOW_BOUNDARY_E, RED);
			Draw_V_Line(X_BOUNDARY_HIGH, Y_LOW_BOUNDARY_E, Y_HIGH_BOUNDARY_E-Y_LOW_BOUNDARY_E, RED);
			CyDelay(5000);
			/* Starts a new game */
			CySoftwareReset();
		}
	}
}

