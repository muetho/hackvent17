#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define CRC32MASK	0x04C11DB7
#define MAX_UINT32	4294967296
#define PROGRESS_1PERCENT 42949672

/*8-bit reflection*/
uint8_t byte_reflection(uint8_t byte)
{
    int i;
    uint8_t reflected_byte = 0;
    for (i=0; i < 8; i++)
    {
	if ((byte & (1 << i)) != 0)
        {
            reflected_byte |= (uint8_t)(1 << (7 - i));
        }
    }
    return reflected_byte;
}

/*32-bit reflection*/
uint32_t crc_reflection(uint32_t crc)
{
    int i;
    uint32_t reflected_crc = 0;
    for (i=0; i < 32; i++)
    {
	if ((crc & (1 << i)) != 0)
        {
            reflected_crc |= (uint32_t)(1 << (31 - i));
        }
    }
    return reflected_crc;
}

/*crc32 calculation*/
uint32_t crc32(uint32_t input, 
	       uint32_t crc_init, 
	       uint32_t final_xor, 
	       bool input_reflection, 
	       bool mask_invers, 
	       bool res_reflection )
{
    int i,j;
    uint8_t byte;
    uint32_t crc = crc_init; /* CRC value is 32bit */

    for (i=0; i < 4; i++)
    {
	/*get current byte*/
	byte = (uint8_t)(input >> ((3-i)*8) & 0x000000FF);

	/*if needed, reflect current input byte*/
	if(input_reflection)
	{
	    byte = byte_reflection(byte);
	}

	/*move byte into MSB of 32bit CRC*/
        crc ^= (uint32_t)(byte << 24);

	/*CRC calculation*/
        for (j = 0; j < 8; j++)
        {
            if ((crc & 0x80000000) != 0) /* test for MSB = bit 31 */
            {
                if(mask_invers)
		{
                    crc = (uint)((crc << 1) ^ ~CRC32MASK);
		}
		else
		{
                    crc = (uint)((crc << 1) ^ CRC32MASK);
		}
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    /*if needed, reflect final crc*/
    if(res_reflection)
    {
        crc = crc_reflection(crc);
    }

    /*xor with specified value*/
    crc = crc ^ final_xor;

    return crc;
}

void print_ascii(uint8_t flag_num, uint32_t flag)
{
    printf("flag %d: %c%c%c%c\n",flag_num,(flag>>24),(flag>>16),(flag>>8),flag);
}

/*brute force program*/
int main(void)
{
    uint32_t input = 0x00000000;
    uint32_t crc;
    uint32_t progress = 0;
   
    /*challenge flags*/
    uint32_t flag_crc[6] = { 0x69355f71, 0xc2c8c11c, 0xdf45873c, 0x9d26aaff, 0xb1b827f4, 0x97d1acf4};

    /*enter brute force loop*/
    for (int i=0; i<MAX_UINT32; i++)
    {
	/*calculate crc*/
        crc = crc32(input,0xFFFFFFFF,0xFFFFFFFF,true,false,true);

	/*test with give crc of flags*/
	for (int j=0; j<6; j++)
	{
	    if (crc == flag_crc[j])
	    {
                print_ascii(j,input);
	    }
	}

	/*print the progress*/
	if ( (i % PROGRESS_1PERCENT) == 0)
	{
            printf("%d%\n",progress);
	    progress++;
	}

        /*increment input*/
	input++;
    }
}
