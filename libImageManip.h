#ifndef LIBIMAGEMANIP_H /* To avoid multiple insertion */
#define LIBIMAGEMANIP_H

int readSizeFromPPM(const char filename[], unsigned int *p_height, unsigned int *p_width, unsigned int *p_maxVal);

int loadImageFromPPM(const char filename[], unsigned int size, unsigned char imageArray[size]);

int saveImageAsciiPPM(const char filename[], unsigned int size, unsigned char imageArray[size], unsigned int height, unsigned int width, unsigned int maxVal);

void copyImage(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void grey256(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void negative(int maxVal, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void extractRed(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void extractGreen(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void extractBlue(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void mirrorH(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void reverse(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);

void mirrorV(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size]);



/* ... */

#endif

