#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "IntroduceError.c"

/**
 * Must maintain that all arguments are not greater than this size
 */
static const int BUFFER = 1024;

/**
 * Enables interpretation of BER
 */
double stringToDouble(char *inputString);

/**
 * CRC Generation is done here
 */
char* generateCRC(char* inputData, char* crcGenerator, int inputLength, int crcLength);

/**
 * Written just to avoid writing same code twice
 */
void doXOR(long long* in2Dec, long long crc2Dec, int inputLength, int crcLength, int differenceInLength);

/**
 * Verifies if we have a valid codeword
 */
bool checkCRC(char* inputData, char *crcGenerator, int inputLength, int crcLength);

 /**
  * Function to convert a string of chars
  * into a binary number
  */
long long stringToDecimal(char * inputString);

/**
 * Function to convert binary number back into
 * a string
 */
char* decimalToString(long long binNum, int actualLength);

/**
 * Provided Code
 */
void IntroduceError( char *data, double p );
