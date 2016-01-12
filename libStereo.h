#ifndef LIBSTEREO_H /* To avoid multiple insertion */
#define LIBSTEREO_H

double ssd(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc1[size], unsigned char imageArraySrc2[size], unsigned int col1, unsigned int row1, unsigned int col2, unsigned int row2, unsigned int radius);

void computeCorrelation(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc1[size], unsigned char imageArraySrc2[size], unsigned int col1, unsigned int row1, unsigned int radius, double fArray[width]);

unsigned int idxMinCorrel(unsigned int size, double fArray[size]);

void doubleDisparityMap(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc1[size], unsigned char imageArraySrc2[size], unsigned int radius, unsigned int step, double map[height*width]);

void printDisparityMap(unsigned int height, unsigned int width, unsigned int size, double min, double max, double map[height*width], unsigned char imageArrayDest[size]);

/* ... */

#endif

