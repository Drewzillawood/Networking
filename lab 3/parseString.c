#include "parseString.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * @author Drew and Nina
 * 
 * The layout of our project requires three inputs:
 * @param arc 
 *  must be 4 
 * @param argv
 *  must contain the following: [0] executable [1] Input string [2] CRC Generator and [3] for generating a bit rate error
 */
int main(int argc, char *argv[])
{
    //check that there are 4 arguments and input string is not too long
    // argv[0] is executable file name, argv[1] is input string, argv[2] is CRCgenerator polynomial in binary, argv[3] is a bit rate error between 0.00001 and 1
    if(argc != 4 || strlen(argv[1]) > BUFFER)
    {
        printf("Invalid input!\n");
        exit(0);
    }

    double BER = stringToDouble(argv[3]);
    //make sure BER is within range
    if( BER > 1 || BER < 0)
    {
	    printf("Invalid input!\n"); 
        exit(0);
    }

    char* inputData     = malloc(BUFFER);
    char* crcGenerator  = argv[2];

    //save input into a string
    strcpy(inputData, argv[1]);
    int inputLength = strlen(inputData);
    int crcLength = strlen(crcGenerator);
    int codeWordLength = inputLength + crcLength - 1;

    //make buffer the right length
    inputData = realloc(inputData, codeWordLength + 1);
    inputData[codeWordLength] = '\0';

    //make sure CRC is initialized to 0s
    for(int i = inputLength; i < codeWordLength; i++)
        inputData[i] = '0';

    // CRC observation
    printf("Original input:         %.*s : %s\n", inputLength, inputData, crcGenerator);
    char* fullCodeWord = generateCRC(inputData, crcGenerator, codeWordLength, crcLength);
    printf("With CRC:               %.*s : %*.*s\n", inputLength, fullCodeWord, inputLength, crcLength, fullCodeWord + inputLength);
    IntroduceError(fullCodeWord, BER);
    /***********Exercise Two***********************/

    // char* e = "00000001000100000010000100000000";

    /***********************1111111111111111***********************/
    printf("Corrupted codeword:     %.*s : %*.*s\n", inputLength, fullCodeWord, inputLength, crcLength, fullCodeWord + inputLength);

    // CRC verification
    bool checker = checkCRC(fullCodeWord, crcGenerator, codeWordLength, crcLength);
    printf("Does the finished codeword check as being error free? |%s|\n", (checker ? "YES" : "NO"));
    free(fullCodeWord);
    free(inputData);
}

/**
 * Break the BER string argument into a decimal value
 */
double stringToDouble(char *inputString)
{
    double bin = 0;
    int length = strlen(inputString);
    int l = 0; //final decimal point spot
    
    // i - character index
    // j - decimal index
    // k - find decimal point index if it exists
    for(int i = length - 1, j = 1, k = 0; i >= 0; i--)
    {
	    //check if decimal point
	    if(inputString[i] == '.')
        {
		    l = k;		
	    }
	    else
        {
		    k++;	
        	bin += (double) (j * (inputString[i] - 48));
		    j *= 10;
	    }
    }
    //shift if decimal point exists
    if (l != 0) 
	    bin /= pow(10, l);

    return bin;
}

/**
 * Actual assignment goal achieved in here 
 */
char* generateCRC(char* inputData, char* crcGenerator, int inputLength, int crcLength)
{
    // Gather our decimal values to allow for efficient XOR calculations
    long long in2Dec = stringToDecimal(inputData);
    long long originalInputDec = in2Dec;
    long long crc2Dec = stringToDecimal(crcGenerator);
    int differenceInLength = inputLength - crcLength;

    // Do CRC division
    doXOR(&in2Dec, crc2Dec, inputLength, crcLength, differenceInLength);

    // Return string equivalent of our remainder ORed with our original
    return decimalToString(in2Dec | originalInputDec, inputLength);
}

/**
 * XOR process for dividing the input by our generator
 */
void doXOR(long long* in2Dec, long long crc2Dec, int inputLength, int crcLength, int differenceInLength)
{
    for(int i = 0; i < differenceInLength; i++)
    {
        // Retrieve the first bit of both of our binary numbers
        int findInputMSB = *in2Dec >> (inputLength - i - 1);
        int findCRCGeneratorMSB = crc2Dec >> (crcLength - 1);

        // If they are both 1 then we want to XOR and store the result back into in2Dec
        if(findInputMSB & findCRCGeneratorMSB)
            *in2Dec ^= crc2Dec << (differenceInLength - i);
    }
}

/**
 * Verify if codeword is valid
 */ 
bool checkCRC(char* inputData, char* crcGenerator, int inputLength, int crcLength)
{
    // Gather our decimal values to allow for efficient XOR calculations
    long long in2Dec = stringToDecimal(inputData);
    long long originalInputDec = in2Dec;
    long long crc2Dec = stringToDecimal(crcGenerator);
    int differenceInLength = inputLength - crcLength;

    // Do CRC division
    doXOR(&in2Dec, crc2Dec, inputLength, crcLength, differenceInLength);

    return in2Dec == 0;
}

/**
 * Handy function to convert our string into a decimal number 
 */
long long stringToDecimal(char *inputString)
{
    long long bin = 0;
    int length = strlen(inputString);
    for(int i = length - 1; i >= 0; i--)
        bin += (inputString[i] == '1') ? (long long) pow(2, length - i - 1) : 0;
    return bin;
}

/**
 * Handy function to convert our handily converted decimal back to a string 
 */
char *decimalToString(long long binNum, int actualLength)
{
    long long temp = binNum;

    char *binString = malloc(actualLength + 1);
    for(int i = actualLength - 1; i >= 0; i--)
    {
        binString[i] = temp & 1 ? '1' : '0';
        temp >>= 1;
    }

    binString[actualLength] = '\0';
    return binString;
}
