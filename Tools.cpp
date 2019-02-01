/*
    Utility functions (e.g., for bit/byte manipulations) used in Y86 simulator.
    
    Author:    
    version:    
*/
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstdint>
#include "Tools.h"

#define NDEBUG

namespace Tools {
    
/*-----------------------------------------------------------------------------------------------
    getBits
	Extracts bits 'low' to 'high' (inclusive) from 'source' and returns the result as
	an 64-bit integer. 
	Example:
	
	getBits(12,15,0x0123456789abcdef);
	
	would return the value:
	
	0x00000000000000c;
	
	(Bits 12-15 of the 64-bit word, counting from the right correspond to hex digit 'c' in the
	source word. Note that any number of bits from 1 to 64 could be specified depending on the values
	of 'low' and 'high'.
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t getBits(unsigned low, unsigned high, uint64_t source)
{
    assert((high >= 0) && (high < 64) && (low <= high));
    unsigned mostSig = 63 - high;
    source = source << mostSig;
    source = source >> (mostSig + low);


    
    return source;
}
/*-----------------------------------------------------------------------------------------------
    setBits
	Sets (to 1) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t setBits(unsigned low, unsigned high, uint64_t source)
{
    assert((high < 64) && (low <= high));
    uint64_t ones = 0xffffffffffffffff;
    unsigned mostSig = 63 - high;
    ones = ones << mostSig;
    ones = ones >> (mostSig + low);
    ones = ones << low;
    source = source | ones;
     
    return source;
}
/*-----------------------------------------------------------------------------------------------
    clearBits
	
	Clears (to 0) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t clearBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));
    uint64_t zeros = 0xffffffffffffffff;
    unsigned mostSig = 63 - high;
    zeros = zeros << mostSig;
    zeros = zeros >> (mostSig + low);
    zeros = zeros << low;
    zeros = ~zeros;
    source = source & zeros;

    return source;
}

/*-----------------------------------------------------------------------------------------------
    assignOneBit
	Sets the bit number 'bitNum' to the binary value (0 or 1) specified by 'bitVal' in
	the source word and returns the resulting word.
	
	'bitNum' must be in the range 0 to 63 (inclusive) and 'bitVal' must be 0 or 1.
------------------------------------------------------------------------------------------------*/
uint64_t assignOneBit(unsigned bitNum, unsigned bitVal, uint64_t source)
{
    assert(bitNum >= 0 && bitNum < 64 && bitVal < 2 && bitVal >= 0);
    uint64_t newBit = 0x0000000000000000;
    if(bitVal == 1)
    {
        newBit = newBit | bitVal;
        newBit = newBit << bitNum;
        source = newBit | source;
     }
     else
     {
         newBit = 0x0000000000000001;
         newBit = newBit << bitNum;
         newBit = ~newBit;
         source = newBit & source;
     }

    
    return source;
}

/*-----------------------------------------------------------------------------------------------
    getByteNumber
	Returns the specified byte number from the source word.
	
	Example:
	getByteNumber(3,0x0011223344556677);
	
	will return the single byte value:
	
	0x44
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint8_t getByteNumber(unsigned byteNum, uint64_t source)
{
    assert(byteNum < 8 && byteNum >= 0);
    unsigned left = 14 - (byteNum*2);
    source = source << (left * 4);
    source = source >> 56; 
    return source;
}
/*-----------------------------------------------------------------------------------------------
    putByteNumber
	Returns the source word after replacing the specified byte (byteNum) with a given 
	byte value (byteVal).
	
	Example:
	putByteNumber(3,0x00,0x0011223344556677);
	
	will return the value:
	
	0x0011223300556677
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint64_t putByteNumber(unsigned byteNum, uint8_t byteVal, uint64_t source)
{
    assert(byteNum < 8 && byteNum >= 0 && byteVal <= 0xff && byteVal >= 0x00);
    unsigned low = byteNum * 8;
    unsigned high = low + 7;
    source = clearBits(low, high, source);
    
    uint64_t longerVal = 0x00 | byteVal;
    longerVal = longerVal << (byteNum * 8);
    source = source | longerVal;

    return source;                  
}
/*-----------------------------------------------------------------------------------------------
    buildWord
	
	Returns a 64 bit word consisting of the 8 parameter bytes, where b0 is the least significant
	and b7 is the most significant byte.
------------------------------------------------------------------------------------------------*/
uint64_t buildWord(unsigned char b0, unsigned char b1,unsigned char b2, unsigned char b3,
                          unsigned char b4, unsigned char b5,unsigned char b6, unsigned char b7)
{
assert((b0 < 0x100) && (b1 < 0x100) && (b2 < 0x100) && (b3 < 0x100) && (b4 < 0x100)
             && (b5 < 0x100) && (b6 < 0x100) && (b7 < 0x100));
    uint64_t source = 0x0;
    source = b7 | source;
    source = b6 | (source << 8); 
    source = b5 | (source << 8);
    source = b4 | (source << 8);
    source = b3 | (source << 8);
    source = b2 | (source << 8);
    source = b1 | (source << 8);
    source = b0 | (source << 8);
     
    return source;                     
}
/*-----------------------------------------------------------------------------------------------
    isNegative

	Returns true if the input parameter is negative when interpreted as a signed value.
------------------------------------------------------------------------------------------------*/
bool isNegative(uint64_t source)
{
    source = source >> 63;
    return source;
}
/*-----------------------------------------------------------------------------------------------
    expandBits
	
	Reads the source and converts it to a character string consisting of 72 bytes--71 characters
	followed by a '\0' character (string terminator).
	Example usage: 
		char buf[72];
		expandBits(0xAABBCCDDEE001122UL,buf);

		Will cause the following string to be written to the buf[] array (not including quotes):
	
	"10101010 10111011 11001100 11011101 11101110 00000000 00010001 00100010"
	
	Note the space after every group of 8 digits except the last. The last byte of
	the string is the '\0' byte, that is not printable but used to indicate the end
	of a C string.
------------------------------------------------------------------------------------------------*/
void expandBits(uint64_t source, char *bits)
{
    /* and bits with zero except bit wanted then add that value to the array*/
    //the left is bits[0]
    assert(bits != NULL);
    uint64_t mask = 0x0000000000000001;
    uint64_t temp = source;
    int bitNum = 0;
    bits[8] = 32;
    bits[17] = 32;
    bits[26] = 32;
    bits[35] = 32;
    bits[44] = 32;
    bits[53] = 32;
    bits[62] = 32;
   
    for(int i = 70; i >= 0; i--)
    {
        if((i != 8) && (i != 17) && (i != 26) && (i != 35) &&
            (i != 44) && (i != 53) && (i != 62))
         {
            temp = temp & mask; //makes mask
            if(temp > 0)
            {
                bits[i] = '1';
            }
            else
            {
                bits[i] = '0';
            }   
            mask = mask << 1;
            bitNum++;
            temp = source;
       }
      
    }
    bits[71] = '\0';
    return;
}
/*-----------------------------------------------------------------------------------------------
    clearBuffer
	Treats pbuf as a pointer to an array of 'size' bytes and clears them all to zero.
------------------------------------------------------------------------------------------------*/
void clearBuffer(char * pbuf, int size)
{
    assert(pbuf != NULL && size >= 0);
    for(int i = 0; i < size; i++)
    {
        pbuf[i] = 0;
    }
    return;
}

} // end namespace Y86
