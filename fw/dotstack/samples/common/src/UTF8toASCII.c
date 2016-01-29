/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include <string.h>

#define MASKBITS                0x3F
#define MASKBYTE                0x80
#define MASK2BYTES              0xC0
#define MASK3BYTES              0xE0
#define MASK4BYTES              0xF0
#define MASK5BYTES              0xF8
#define MASK6BYTES              0xFC

void UTF8toASCII(char* input, char* output, int outputSize)
{
    int inputLen = (int)strlen(input);
	int outputLen = 0;
	int i;
	
	if (!output)
	{
		output = input;
		outputSize = strlen(input) + 1;
	}

    for (i = 0; i < inputLen && outputLen < outputSize - 1;)
    {
        char ch;

        // 1110xxxx 10xxxxxx 10xxxxxx
        if((input[i] & MASK3BYTES) == MASK3BYTES)
        {
            ch = '.';
            i += 3;
        }
        // 110xxxxx 10xxxxxx
        else if((input[i] & MASK2BYTES) == MASK2BYTES)
        {
            ch = '.';
            i += 2;
        }
        // 0xxxxxxx
        else if(input[i] < MASKBYTE)
        {
            ch = input[i];
            i += 1;
        }

        output[outputLen++] = ch;
    }
	
	 output[outputLen] = 0;
}
