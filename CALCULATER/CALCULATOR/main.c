/*
 ============================================================================
 Name        : calculator.c
 Author      : Ahmed Ezzat
 Description : Simple Calculator .
 ============================================================================
 */

/******************************************************************************
 *                               HEADER FILES ✔️
 *******************************************************************************/
#include "common_macros.h"
#include "gpio.h"
#include "lcd.h"
#include "keypad.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                               Definitions  ✔️                                *
 *******************************************************************************/
/* DEFINE YOUR (MAXIMUM) NUM OF YOUR OPERANDS */
#define NUM_OF_OPERANDS  5

/******************************************************************************
 *                            FUNCTIONS PROTOTYPE ✔️
 *******************************************************************************/
void Get_Result();

/******************************************************************************
 *                             GLOBAL VARIABLS ✔️
 *******************************************************************************/

uint16 operand[NUM_OF_OPERANDS]; /* ARRAY CONTAIN NUMBER OF OPERANDS ✔️*/
uint16 operator; /* CONTAIN THE TYPE OF OPERATOR ✔️*/
uint16 result; /* CONTAIN THE FINAL RESULT OF OPERATIONS ✔️*/
uint16 Operand_num = 0; /* COUNTER OF OPERANDS OF THE ARRAY (INDEX) ✔️*/
uint16 Digits_num = 0; /* COUNTER OF DIGITS OF THE OPERAND (INDEX) ✔️*/


int main() {

	/* PERIPHERAL INITIATION ✔️*/
	LCD_init();
	/* LOCAL VARIABLE STORE THE VALUE OF PRESSED KEY ✔️*/
	uint8 Keypad_Value;

	while (1) {
		/* GET THE  VALUE OF PRESSED KEY ✔️*/
		Keypad_Value = KEYPAD_getPressedKey();

		/* IF THE VALUE BETWEEN 0 --> 9 ✔️*/
		if ((Keypad_Value <= 9) && (Keypad_Value >= 0))
		{

			if (Digits_num == 0)
			{
				/* STORE THE VALUE OF PRESSED VALUE IN OPERANDS ARRAY ✔️*/
				operand[Operand_num] = Keypad_Value;
				Digits_num++;
			}
			else
			{

				/* STORE THE VALUE OF PRESSED VALUE IN OPERANDS ARRAY (MORE THAN 1 DIGIT) ✔️*/
				operand[Operand_num] = (operand[Operand_num] * 10) + Keypad_Value;
				Digits_num++;
			}

			/* DISPLAY THE PRESSED KEY ON 2*16 LCD ✔️ */
			LCD_intgerToString(Keypad_Value);

			/* CHECK IF COUNTER=0 ✔️*/
			if (Operand_num == 0)
			{
				/*MAKE THE FINAL RESULT STORES THE FIRST OPERAND (AT BEGINNIG) ✔️ */
				result = operand[0];
			}

		}

		/* IF THE VALUE IS AN OPERATIN FROM ( / , * , + , - ) ✔️
		 * MAKE SURE THAT ( = ) NOT PRESSED --> (=) REFER TO END OF ALL OPERATIONS*/
		else if (Keypad_Value != '=')
		{
			/* CALL THE FUNCTION AFTER EACH OPERATOR TO CALCULATE THE CURRENT RESULT */
			Get_Result();
			/* STORE THE VALUE OF PRESSED KEY IN OPERATOR VARIABLE */
			operator = Keypad_Value;
			/* DISPLAY THE PRESSED KEY ON  2*16 LCD ✔️*/
			LCD_displayCharacter(Keypad_Value);
			/* MAKE SURE TO ENCREMENT COUNTER OF OPERANDS OF THE ARRAY (INDEX) ✔️*/
			Operand_num++;
			/* CLEAR THE COUNTER OF DIGITS AGAIN
			 * TO MAKE THE SECOND OPERAND START FROM BEGINING
			 * (DEFULT = 1 DIGIT) */
			Digits_num = 0;
		}

		/* INPUT TIME
		 * IF ANY KEY PRESSED MORE THAN 300 MS IT WILL BE CONSIDERED AS ANOTHER PRESS ✔️ */
		_delay_ms(300);

		/* CHECK IF THE USER PRESS (=) --> IT MEAN THE END OF ALL OPERATIONS ✔️*/
		if (Keypad_Value == '=')
		{
			/* CALL THE FUNCTION TO CALCULATE THE FINAL RESULT ✔️*/
			Get_Result();
			/* GO TO THE SECOND ROW FIRST COL ✔️*/
			LCD_displayStringRowColumn(1, 0, "RESULT = ");
			/* PRINT THE FINAL RESULT ON  2*16 LCD ✔️*/
			LCD_intgerToString(result);
		}

		/* IF USER PRESS ON/C KEY --> IT WILL RESET EVERY THING AND START OVER AGAIN ✔️*/
		if (Keypad_Value == 13)
		{
			LCD_clearScreen();
			Operand_num = 0;
			Digits_num = 0;
		}

	}
}

/*
 * Description :
 * CALCULATE AND DEFINE THE FINAL RESULT OF ALL OPERATIONS.
 * IF THE PRESSED KEY WASNOT AN OPERATOR , IT WILL NOT DO ANY THING.
 * IT DEFINE THE RESULT AFTER EACH ENTERED NUMBER.
 */
void Get_Result() {

	switch (operator)
	{
	/* SUMMATION OPERATIONS */
	case '+':
		result += operand[Operand_num];
		break;
		/* SUBTRACTION OPERATIONS */
	case '-':
		result -= operand[Operand_num];
		break;
		/* MULTIPLICATION OPERATIONS */
	case '*':
		result *= operand[Operand_num];
		break;
		/* DIVISION OPERATIONS */
	case '/':
		result /= operand[Operand_num];
		break;
	default:
		break;

	}

}
