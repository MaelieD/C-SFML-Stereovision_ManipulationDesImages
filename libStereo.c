#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libStereo.h"

/**
 * Similarity between two points
 * Sum of Squared Differences aka SSD
 * This function calculate the SSD between the pixel (col1, row1) of the image imageArraySrc1 and pixel (col2, row2) of the image imageArraySrc2, considering the neighborhood radius.
 * If the coordinates (col1, row1) or (col2, row2) are too close to the edge (the neighborhood come out of
 * picture) function returns the value INFINITY, a constant defined in math.h
 */
double ssd(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc1[size], unsigned char imageArraySrc2[size], unsigned int col1, unsigned int row1, unsigned int col2, unsigned int row2, unsigned int radius)
{
    unsigned int i, j, i2, j2;
    double result = 0;
    double squared = 0;

    if(col1+radius >= width || col2 + radius >= width || row1+radius >= height || row2 + radius >= height || col1 < radius || col2 < radius || row1 < radius || row2 < radius )
        return INFINITY;
    else{ 
        // browse the neighborhood : radius*2+1 by radius*2+1
        for(j=0,j2 = 0 ; j <=2*radius ;j++,j2++)
        {
            for(i=0, i2=0; i <=2*radius ;i++,i2++)
            {    
                // do the sum of squared fifferences
                squared = (double)imageArraySrc1[((row1 - radius + j)*width+(col1 - radius + i))*4] - (double)imageArraySrc2[((row2 - radius +j2)*width+(col2 - radius + i2))*4];
                result += squared * squared;
            }
        }

        return result; // return the sum
    }
}

/**
 * This function uses the previous function to calculate SSD values between the pixel (col1, row1) of the image imageArraySrc1 and all pixels in the line row2 of the image imageArraySrc2, considering the neighborhood radius. 
 * SSD values obtained are stored in the array fArray.
 */
void computeCorrelation(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc1[size], unsigned char imageArraySrc2[size], unsigned int col1, unsigned int row1, unsigned int radius, double fArray[width])
{
    unsigned int col2 = 0;

    for(col2=0; col2 < width ;col2++)
    {
        fArray[col2] = ssd(height, width, size, imageArraySrc1, imageArraySrc2, col1, row1, col2, row1, radius);
    }
}


/**
 * This function searches the index of the column of minimum correlation table fArray and returns.
 * In case of multiple minima, we take the smallest index.
 */
unsigned int idxMinCorrel(unsigned int size, double fArray[size])
{
    double j = fArray[0];// get the first min
    unsigned int i = 0;
    int idx = 0;

    for(i=0; i<size ;i++)
    {
        if(fArray[i] < j)
        {
            j = fArray[i];// continue the loop
            idx = i;// get index min
        }
    }
    return idx;
}

/**
 * This function uses the previous functions to create the disparity map.
 * The disparity values between the pixels of the left image imageArraySrc1 and the right image imageArraySrc2 are calculated for each pixel using the correlation index SSD with the radius.
 * We do not forget to pay attention to the types of variables.
 */
void doubleDisparityMap(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc1[size], unsigned char imageArraySrc2[size], unsigned int radius, unsigned int step, double map[height*width])
{
    unsigned int i, j;
    double arrayCorrel[width];
    unsigned int column;

    for(i=0; i<width ;i++)
        for(j=0; j<height ;j++)
            map[j*width+i] = INFINITY;

    // The parameter Step defines the density of the map: if it is 1, 2 or 3, the array map will not be the same. 
    // As we can't calculate the SSD to similar edge points, it is useless to go.
    for(i=radius; i<(width-radius) ;i+=step)
    {
        for(j=radius; j<(height-radius) ;j+=step)
        {
            computeCorrelation(height, width, size, imageArraySrc1, imageArraySrc2, i, j, radius, arrayCorrel);
            // we need the index of min to calculate the diparity.
            column = idxMinCorrel(width, arrayCorrel);
            // the disparity is the difference between the two points, ie: d(pt1, pt2) = col1 - col2.
            map[j*width+i] = (int)i - (int)column; 
        }
    }
}

/**
 * This function converts the values of disparity map and fill the table RGBA of the image with grayscale.
 * Pixels whose disparity is smaller than min or greater than max will be black (0,0,0).
 * To convert values from one set to another, we use this formula : iVal = 255*((dVal-dMin)/(dMax-dMin))
 */
void printDisparityMap(unsigned int height, unsigned int width, unsigned int size, double min, double max, double map[height*width], unsigned char imageArrayDest[size])
{
    unsigned int i;
    double iVal; // iVal corresponds to the map value converted to double (between dMin and dMax) to intensity (between 0 and 255)

    // a loop to iterate through the disparity map and make the conversion
    for(i=0; i<width*height ;i++)
    {
        // pixels whose disparity is smaller than min or greater than max
        if(map[i] < min || map[i] > max)
        {
            iVal = 0; // black : (0,0,0)
        }else 
            iVal = 255*((map[i]-min)/(max-min)); // values converted

        // array filled RGBA image with grayscale : R=G=B
        imageArrayDest[i*4] = iVal;
        imageArrayDest[i*4+1] = iVal;
        imageArrayDest[i*4+2] = iVal;
        imageArrayDest[i*4+3] = 255;
    }
}

/* THE END */


