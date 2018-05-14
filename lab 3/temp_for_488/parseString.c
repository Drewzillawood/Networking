#include "parseString.h"

int main(int argc, char *argv[])
{
    //check that there are 4 arguments and input string is not too long
    // argv[0] is executable file name, argv[1] is input string, argv[2] is CRCgenerator polynomial, argv[3] is a bit rate error between 0.00001 and 1
    if(argc != 4 || strlen(argv[1]) > BUFFER )
    {
        printf("Invalid input!\n");
        exit(0);
    }

    double BER = stringToDouble(argv[3]);
    //make sure BER is within range
    if( BER > 1 || BER < 0.00001){
	printf("bullshit!\n");  	
    }

    printf("%f, %c \n", BER, *argv[3]);








    char* buffer        = malloc(BUFFER);
    char* crcGenerator  = argv[2];

    //save input into buffer as a string
    strcpy(buffer, argv[1]);
    int length = strlen(buffer) + strlen(crcGenerator);
    int crcLength = strlen(crcGenerator);
    //make buffer the right length
    buffer = realloc(buffer, length);
    buffer[length - 1] = '\0';

    //make sure CRC is initialized to 0s
    int i;
    for(i = strlen(buffer); i < length; i++)
    {
        buffer[i] = '0';
    }

    char *newBuffer = malloc(length);
    strcpy(newBuffer, buffer);

    int j = 0;

    while(j < length - crcLength){

        while(newBuffer[j] == '0'){
            j++;
        }
        for(i = 0; i < crcLength; i++)
        {
            if(newBuffer[j] == crcGenerator[i])
            {
                newBuffer[j] = '0';
            }
            else
            {
                newBuffer[j] = '1';
            }
            j++;
        }
    }
}

double stringToDouble(char *inputString)
{
    double bin = 0;
    int length = strlen(inputString);
    int i; //character index
    int j = 1; //decimal  index
    int k = 0; //find decimal point index if it exists
    int l = 0; //final decimal point spot
    
    for(i = length - 1; i >= 0; i--)
    {
	//check if decimal point
	if(inputString[i] == '.'){
		l = k;		
	}
	else{
		k++;	
        	bin += (double) (j * (inputString[i] - 48));
		j = j * 10;
	}
    }
    //shift if decimal point exists
    if (l != 0) {
	bin = bin / pow(10, l);
    }

    return bin;
}

long long stringToDecimal(char *inputString)
{
    long long bin = 0;
    int length = strlen(inputString);
    int i;
    for(i = length - 1; i > 0; i--)
    {
        bin += (long long) pow(2, inputString[i] - 49);
    }
    return bin;
}

char *decimalToString(long long binNum, int actualLength)
{
    long long temp = binNum;

    char *binString = malloc(actualLength + 1);
    int i;
    for(i = actualLength - 1; i >= 0; i--)
    {
        // #define B_TO_A( VAL ) ( ( 0 == VAL ) ? ('0') : ('1') )
        // char set = B_TO_A( temp );
        char set = temp == 0 ? '0' : '1';
        binString[i] = set;
        temp >>= 1;
    }

    binString[actualLength] = '\0';
    return binString;
}
