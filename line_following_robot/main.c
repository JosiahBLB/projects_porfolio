/*
 * Robot_Control.c
 *
 * Author : josiah
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define ledLeftOn		(PORTA |= (1<<4))
#define ledLeftOff		(PORTA &= ~(1<<4))
#define ledRightOn		(PORTC |= (1<<4))
#define ledRightOff		(PORTC &= ~(1<<4))
#define leftControlCW	(PORTA |= (1<<0))
#define leftControlCCW	(PORTA &= ~(1<<0))
#define rightControlCW	(PORTC |= (1<<2))
#define rightControlCCW	(PORTC &= ~(1<<2))

#define leftBumper		(!(PINA & (1<<2)))
#define rightBumper		(!(PINC & (1<<5)))
#define leftSensor		(PINA & (1<<7))
#define centerSensor	(PINA & (1<<6))
#define rightSensor		(PINA & (1<<3))

void setup(void);
void setRightMotor(signed char);
void setLeftMotor(signed char);
void updateLEDs(void);
void linePDControl(void);
void lineLost(void);
void obsticalEvasion(void);

unsigned char lastPosition = 0;	// Position of line before loss.
signed char rightBias = 100;	// Right motor demand for straight line.
signed char leftBias = 100;	// Left motor demand for straight line.
signed char lastError = 0;	// Error from previous loop.
signed char Kp = 0.02;	// Proportional Constant.3
signed char Kd = 12;	// Derivative Constant.20
unsigned int lostlinenumber = 0;	//lost

int main(void)
{
	
	setup();
	_delay_ms(1000);

	while (!(leftBumper && rightBumper));
	
	while (1)
	{
		updateLEDs();
		
		if((leftSensor)||(centerSensor)||(rightSensor))
		{
			lastPosition = ((leftSensor*0)+(centerSensor*100)+(rightSensor*200))/(leftSensor+centerSensor+rightSensor);
			linePDControl();
			lostlinenumber =0;
		}
		
		else
		lineLost();
		obsticalEvasion();	// Poll bumper switches, perform reversing turn if triggered.


	}
	
	
}

void setup(void)
{
	
	DDRA = 0x31;	//0b00110001
	DDRC = 0x15;	//0b00010101

	// Timer setup:
	TCCR0A = 0x03;	//Fast PWM channel A and B, Top = 0xFF.
	TCCR0B = 0x05;	//Pre-scaling = /1024.
	OCR0A = 0;	// Zero Output Compare A.
	OCR0B = 0;	// Zero Output Compare B.
	
}

void setRightMotor(signed char Speed)
{

	if(Speed == 0)
	{
		TCCR0A &= ~(1<<7)&~(1<<6);
		rightControlCCW;
		OCR0A = 0;
	}

	else
	{
		if(Speed < 0)
		{
			TCCR0A |= (1<<7);
			TCCR0A |= (1<<6);
			OCR0A = ((double)(Speed*-1)/100)*255;
			rightControlCW;
		}

		else if(Speed>0)
		{
			TCCR0A |= (1<<7);
			TCCR0A &= ~(1<<6);
			OCR0A = ((double)Speed/100)*255 ;
			rightControlCCW;
		}
	}
	
}

void setLeftMotor(signed char Speed)
{
	
	if(Speed == 0)
	{
		TCCR0A &= ~(1<<4)&~(1<<5);
		leftControlCCW;
		OCR0B = 0;
	}

	else
	{
		if(Speed<0)
		{
			TCCR0A |= (1<<5);
			TCCR0A &= ~(1<<4);
			OCR0B = ((double)(Speed*-1)/100)*255 ;
			leftControlCCW;
		}
		else if(Speed>0)
		{
			TCCR0A |= (1<<5);
			TCCR0A |= (1<<4);
			OCR0B = ((double)Speed/100)*255;
			leftControlCW;
		}
	}
	
}

void updateLEDs(void)
{

	if(leftSensor)
	ledLeftOn;
	else
	ledLeftOff;

	if(rightSensor)
	ledRightOn;
	else
	ledRightOff;

	if(centerSensor)
	{
		ledRightOn;
		ledLeftOn;
	}
}


void linePDControl(void)
{

	char error = (lastPosition - 100);
	char controlOutput = (Kp * error) + (Kd * (error - lastError));
	lastError = error;

	char rightSpeed = rightBias + controlOutput; //could be -
	char leftSpeed = leftBias - controlOutput;	//could be +

	if(rightSpeed>100)
	rightSpeed = 100;
	else if(rightSpeed<-100)
	rightSpeed = -100;

	if(leftSpeed>100)
	leftSpeed = 100;
	else if(leftSpeed<-100)
	leftSpeed = -100;

	setLeftMotor(rightSpeed);
	setRightMotor(leftSpeed);
}

void lineLost(void)
{
	lostlinenumber++;
	if(lastPosition<100)
	{
		setLeftMotor(0);
		setRightMotor(100);
	}

	if(lastPosition>100)
	{
		setLeftMotor(100);
		setRightMotor(0);
	}
	
	if (lostlinenumber >=10000)
	{
		
		setLeftMotor(70);
		setRightMotor(80);
		_delay_ms(400);
		
		lostlinenumber =0;
	}
	
}

void obsticalEvasion(void)
{

	if(leftBumper)
	{
		setLeftMotor(-30);
		setRightMotor(-60);
		_delay_ms(500);
		lastPosition = 100;
		lastError = 0;
	}

	if(rightBumper)
	{
		setLeftMotor(-60);
		setRightMotor(-30);
		_delay_ms(500);
		lastPosition = 100;
		lastError = 0;
	}
	
	if ((leftBumper) && (rightBumper))
	{
		setLeftMotor(-70);
		setRightMotor(-80);
		_delay_ms(300);
		setLeftMotor(-30);
		setRightMotor(-60);
		_delay_ms(300);
		lastPosition = 100;
		lastError = 0;
	}
}


