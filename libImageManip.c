#include <stdio.h>
#include <stdlib.h>

/**
 * This function open, read the file passed as parameter, retrieves the height, width and max value of the image and store them in variables whose addresses are passed as parameters, and then closes the file.
 * If the name does not match any file (fopen returned NULL), the function returns -1.
 * If maxVal is different from 255, the function returns -1.
 * If all is ok, the function returns 0.
 */
int readSizeFromPPM(const char filename[], unsigned int *p_height, unsigned int *p_width, unsigned int *p_maxVal)
{
    int retour;

    /* Files pointers */
    FILE *fpIn = NULL;

    /* Opening file for reading ("r" means "read") */
    fpIn = fopen(filename, "r");
    if(fpIn == NULL)
    {
        printf("Opening of the file %s is impossible\n", filename);
        return -1;
    }

    /* Keyword : P3 */
    retour = fscanf(fpIn, "%*[^\n]\n");
    /* We consider that there is a single commented line, just after the keyword */
    retour = fscanf(fpIn, "%*[^\n]\n");

    retour = fscanf(fpIn, "%d", p_width);

    retour = fscanf(fpIn, "%d", p_height);
    retour = fscanf(fpIn, "%d", p_maxVal);

    if(*p_maxVal != 255)
        return -1;

    /* The function closes the file */
    fclose(fpIn);

    return 0;
}

/**
 * This function open, read the file passed as parameter, pass the information (height, width, etc..) and load lines in the 1D array of color passed as a parameter the image content (ie. pixel values), and closes the file. 
 * If the name doesn't match any file (fopen returned NULL), the function returns -1. 
 * If it's ok, the function returns 0.
 */
int loadImageFromPPM(const char filename[], unsigned int size, unsigned char imageArray[size])
{
    unsigned int i=0;
    /* Value of the scanf return */
    int retour;
    /* Characters read */
    int red, green, blue;

    /* Files pointers */
    FILE *fpIn = NULL;

    /* Opening file for reading ("r" means "read") */
    fpIn = fopen(filename, "r");
    if(fpIn == NULL)
    {
        printf("Opening of the file %s is impossible\n", filename);
        return -1;
    }

    retour = fscanf(fpIn, "%*[^\n]\n"); // Keyword : P3
    retour = fscanf(fpIn, "%*[^\n]\n"); // Comment line
    retour = fscanf(fpIn, "%*[^\n]\n"); // Width / Height
    retour = fscanf(fpIn, "%*[^\n]\n"); // maxVal

    /* Browse all 4 to retrieve RGB and keep a free space for the alpha value */
    for(i=0;i<size;i+=4)
    {
        /* reading characters we need in the file (identical to scanf+FILE*) */
        retour = fscanf(fpIn, "%d %d %d", &red, &green, &blue);
        imageArray[i] = (char) red; // Component Red in first
        imageArray[i+1] = (char) green; // Component Green in the second case
        imageArray[i+2] = (char) blue; // Component Blue in third case

        imageArray[i+3] = 255; // Alpha channel in the fourth case of one-dimensional array
    }

    fclose(fpIn);

    return 0;
}

/**
 * This function write in the file passed as parameter, then writes respecting PPM size the content of the table imageArray, then closes the file. 
 * If the file is not accessible (fopen returned NULL), the function returns -1. 
 * It returns 0 if it's ok.
 */
int saveImageAsciiPPM(const char filename[], unsigned int size, unsigned char imageArray[size], unsigned int height, unsigned int width, unsigned int maxVal)
{
    unsigned int i=0;

    /* Files pointers */
    FILE *fpIn = NULL;

    /* Opening file for writing ("w" means "write") */
    fpIn = fopen(filename, "w");
    if(fpIn == NULL)
    {
        printf("Opening of the file %s is impossible\n", filename);
        return -1;
    }

    /* Writing int the destination file (identical to printf+FILE*) */
    fprintf(fpIn, "P3\n");
    fprintf(fpIn, "#Commentaire\n"); 
    fprintf(fpIn, "%d %d\n", width, height);
    fprintf(fpIn, "%d\n", maxVal);

    for(i=0;i<size;i+=4)
    {
            fprintf(fpIn, "%d %d %d\n", imageArray[i], imageArray[i+1], imageArray[i+2]);
    }

    /* Don't forget to close the file */
    fclose(fpIn);

    return 0;
}

/**
 * This function do an identical copie of the table in the source image to the destination image.
 */
void copyImage(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i;

    for(i=0;i<size;i++)
    {
        imageArrayDest[i] = imageArraySrc[i];
    }

}

/**
 * This function produce from the source image, a destination image grayscale (considering only the brightness rather than color). 
 * A common formula gives the gray level corresponding to red, green and blue components.
 */
void grey256(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i;
    unsigned int grey;

    for(i=0;i<size;i+=4)
    {
        /* Formula : Grey = (Red + Green + Blue)/3 */
        grey = (imageArraySrc[i] + imageArraySrc[i+1] + imageArraySrc[i+2]) / 3;
        /* Save data in the array */
        imageArrayDest[i] = grey;
        imageArrayDest[i+1] = grey;
        imageArrayDest[i+2] = grey;
        imageArrayDest[i+3] = 128;
    }
}

/**
 * This function produce from the source image, a destination image in which each channel is reversed.
 */
void negative(unsigned int maxVal, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i;

    for(i=0;i<size;i+=4)
    {
        /* Formula to negative form : Rdest = maxVal - Rsrc and save data */
        imageArrayDest[i] = maxVal - imageArraySrc[i];
        imageArrayDest[i+1] = maxVal - imageArraySrc[i+1];
        imageArrayDest[i+2] = maxVal - imageArraySrc[i+2];
        imageArrayDest[i+3] = 128;
    }

}

/**
 * Extraction of red, green and blue components.
 * These functions produce from the source image, a destination image grayscale values corresponding to one of the components.
 */
/* Red Component */
void extractRed(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i;

    for(i=0;i<size;i+=4)
    {
        imageArrayDest[i] = imageArraySrc[i];
        imageArrayDest[i+1] = imageArraySrc[i];
        imageArrayDest[i+2] = imageArraySrc[i] ;
        imageArrayDest[i+3] = 128;
    }

    //grey256(size, imageArrayDest, imageArrayDest);
}

/* Green Component */
void extractGreen(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i;

    for(i=0;i<size;i+=4)
    {
        imageArrayDest[i+1] = imageArraySrc[i+1];
        imageArrayDest[i] = imageArraySrc[i+1] ;
        imageArrayDest[i+2] = imageArraySrc[i+1];
        imageArrayDest[i+3] = 128;
    }

    //grey256(size, imageArrayDest, imageArrayDest);
}

/* Blue Component */
void extractBlue(unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i;

    for(i=0;i<size;i+=4)
    {
        imageArrayDest[i+2] = imageArraySrc[i+2];
        imageArrayDest[i+1] = imageArraySrc[i+2];
        imageArrayDest[i] = imageArraySrc[i+2];
        imageArrayDest[i+3] = 128;
    }

    //grey256(size, imageArrayDest, imageArrayDest);
}

/**
 * Horizontal Mirror - Left/Right
 * This function produce from the source image, a destination image corresponding to the reverse of the mediator vertical pixels: the pixel is moved left to right and vice versa.
 */
void mirrorH(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i, j;

    for(j=0;j<height;j++)
    {
        for(i=0;i<width*4;i+=4)
        {
            /* We need to find a strange way to change pixel's place */ 
            /* j*width*4+i on the left side and width*j*4+width*4-4+i on the right side */
            imageArrayDest[j*width*4+i] = imageArraySrc[width*j*4+width*4-4-i];
            imageArrayDest[j*width*4+i+1] = imageArraySrc[width*j*4+width*4-3-i];
            imageArrayDest[j*width*4+i+2] = imageArraySrc[width*j*4+width*4-2-i];
            imageArrayDest[j*width*4+i+3] = imageArraySrc[width*j*4+width*4-1-i];
        }
    }
}

/** 
 * Reverse
 * This function produce from the source image, a destination image corresponding to a rotated 180 degrees relative to the source.
 */
void reverse(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    unsigned int i; //(height-i-1)*width+(width-j-1) > width*i+j
    unsigned int j;


    for(i=height*width*4-1,j=0; j<(height*width*4) ;i-=4,j+=4)
    {
        /* We just need to exchange cases along the diagonal from top left to bottom right corner */
        imageArrayDest[i] = imageArraySrc[j+3];
        imageArrayDest[i-1] = imageArraySrc[j+2];
        imageArrayDest[i-2] = imageArraySrc[j+1];
        imageArrayDest[i-3] = imageArraySrc[j];
    }
}

/**
 * Vertical Mirror - Up/Down
 * This function produce from the source image, a destination image pixels corresponding to the reverse to the horizontal bisector: the pixels from the top is at the bottom and vice versa.
 */
void mirrorV(unsigned int height, unsigned int width, unsigned int size, unsigned char imageArraySrc[size], unsigned char imageArrayDest[size])
{
    //mirrorH(height, width, size, imageArraySrc, imageArrayDest);
    //reverse(height, width, size, imageArrayDest, imageArrayDest);

    unsigned int i, j;

    for(j=0;j<height;j++)
    {
        for(i=0;i<width*4;i+=4)
        {
            imageArrayDest[j*width*4+i] = imageArraySrc[width*(height-j-1)*4+i];
            imageArrayDest[j*width*4+i+1] = imageArraySrc[width*(height-j-1)*4+i+1];
            imageArrayDest[j*width*4+i+2] = imageArraySrc[width*(height-j-1)*4+i+2];
            imageArrayDest[j*width*4+i+3] = imageArraySrc[width*(height-j-1)*4+i+3];
        }
    }
}

/* THE END */

