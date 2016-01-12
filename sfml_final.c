/* gcc -Wall -Wextra -O1 -Wuninitialized -g -lm sfml_avance3.c libImageManip.c libStereo.c -lcsfml-system -lcsfml-graphics -lcsfml-window -o sfml_avance3.exe */

#include <SFML/Graphics.h>
#include "libImageManip.h"
#include "libStereo.h"
#include <stdio.h>
#include <math.h>

/**
 * \brief Distord the view of the window to make the image fit
 *
 * \param window the window in which to draw the image
 * \param width the width of the image
 * \param height the height of the image
 */
void adaptWindowViewToImage(sfRenderWindow* window, unsigned int width, unsigned int height) {

    sfFloatRect rect = {0, 0, width, height};
    sfView *view = sfRenderWindow_GetDefaultView(window);
    sfView_SetFromRect(view, rect);

}

/**
 * \brief Draw the RGBA array of an image to the sfRenderWindow
 *
 * \param window the window in which to draw the image
 * \param width the width of the image
 * \param height the height of the image
 * \param size the size of the RGBA array
 * \param imageArray a monodimensional RGBA array to be drawn.
 */
void drawImage(sfRenderWindow* window, unsigned int width, unsigned int height, unsigned int size, unsigned char imageArray[size]) {

    sfImage* image;
    sfSprite* sprite;

    image = sfImage_CreateFromPixels(width, height, imageArray);
    if (image==NULL) {
        printf("Cannot allocate the image\n");
        exit(-1);
    }
    sfImage_SetSmooth(image, 0);
    sprite = sfSprite_Create();
    if (sprite==NULL) {
        printf("Cannot allocate the sprite\n");
        exit(-1);
    }
    sfSprite_SetImage(sprite, image);

    /* Draw the sprite */
    sfRenderWindow_DrawSprite(window, sprite);

    /* Clean memory */
    sfSprite_Destroy(sprite);
    sfImage_Destroy(image);


}

/**
 * \brief Initialize the imageArray with plain color
 *
 * \param size the size of the RGBA array
 * \param imageArrayDest a monodimensional RGBA array to be filled.
 * \param red   the red channel of the filing color.
 * \param green the green channel of the filing color.
 * \param blue  the blue channel of the filing color.
 * \param alpha the alpha channel of the filing color.
 */
void initImage(unsigned int size, unsigned char imageArrayDest[size], unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {

    unsigned int i;
    for (i = 0; i < size; i+=4) {
        imageArrayDest[i+0] = red;
        imageArrayDest[i+1] = green;
        imageArrayDest[i+2] = blue;
        imageArrayDest[i+3] = alpha;
    }
}

/**
 * \brief plot a SSD curve on a render window
 *
 * \param window the window in which to draw the curve
 * \param width the width of the image
 * \param height the height of the image
 * \param fArray a monodimensional SSD array to be drawn.
 */
void plotCurve(sfRenderWindow* window, unsigned int height, unsigned int width, double fArray[width]) {

    sfShape* shape =NULL;
    unsigned int col, imin = 0;
    /* The coordinate of the segment are normalized [min .. max] => [top .. top-span] for readability */
    unsigned int top = height/2, span = 0.4*height;
    double min=INFINITY,max=-INFINITY;

    /* Calculate the min, the min index and the max of the curve */
    for (col = 0 ; col < width ; col++) {
        if (fArray[col]<min ) {
            min = fArray[col];
            imin = col;
        }
        if (fArray[col]>max&& fArray[col] != INFINITY) {
            max = fArray[col];
        }
    }

    /* Draw the curve segments */
    for (col = 0 ; col < width-1 ; col++) {
        /* Create the shape */
        shape = sfShape_CreateLine(col, top - (span*(fArray[col]-min)/(max-min)),col+1, top - (span*(fArray[col+1]-min)/(max-min)), 3, sfYellow, 0, sfYellow);
        /* Draw the shape */
        sfRenderWindow_DrawShape(window, shape);
        /* Clear memory */
        sfShape_Destroy(shape);
    }
    /* Draw a blue circle to mark the minimum values */
    /* Create the shape */
    shape = sfShape_CreateCircle(imin, top, 5, sfBlue, 0, sfBlue);
    /* Draw the shape */
    sfRenderWindow_DrawShape(window, shape);
    /* Clear memory */
    sfShape_Destroy(shape);
}

/* sfml_avance3
 *
 * The given code is here to show you the potentiality of the libgminiigned int maxVal2, unsigned int height2, unsigned int width
 * You should modify it to have a smooth presentation of YOUR work.
 * Ex: Press D => Image desaturate i.e. goes to grey level. :p
 */
int sfml(unsigned int maxVal, unsigned int height, unsigned int width, unsigned int maxVal2, unsigned int height2, unsigned int width2, unsigned int size, unsigned char imageArray[size], unsigned char imageArray2[size], unsigned char imageDest[size]) {

    printf("Les images font %u par %u\n", height, width);

    initImage(size, imageDest, 0, 114, 174, 255);
    
    /* WindowSettings options : Bits of the depth buffer / Bits of the stencil buffer /  Level of antialiasing */
    sfWindowSettings Settings = {24, 8, 0};
    /* VideoMode options: Width / Height / Depth */
    /* We want a window of 400 pixel width keeping the aspect ratio */
    sfVideoMode Mode = {400, 400*height/width, 32};

    /* We want two windows for the original image and the modified images */
    sfRenderWindow* windowLeft, *windowRight, *windowModif, *currentWindow;
    sfShape* shape = NULL;
    /* Event structure (Mouse, Keyboard, etc.) */
    sfEvent event;

    /* Create the main window */
    windowLeft = sfRenderWindow_Create(Mode, "Image originale",  sfResize | sfClose, Settings);
    if (windowLeft == NULL) {
        printf("Cannot create main window\n");
        return EXIT_FAILURE;
    }
    /* Set the position of the main window */
    sfRenderWindow_SetPosition(windowLeft, 0, 50);
    /* Adapt the the window's view to the image => The image will be stretched */
    adaptWindowViewToImage(windowLeft, width, height);

    /* Create the second windows */
    windowRight = sfRenderWindow_Create(Mode, "Image stereo",  sfResize | sfClose, Settings);
    if (windowRight == NULL) {
        printf("Cannot create second window\n");
        return EXIT_FAILURE;
    }
    /* Set the position of the second window */
    sfRenderWindow_SetPosition(windowRight, 400+5, 50);
    /* Adapt the the window's view to the image => The image will be stretched */
    adaptWindowViewToImage(windowRight, width, height);

    /* Create the third windows */
    windowModif = sfRenderWindow_Create(Mode, "Image modifiee",  sfResize | sfClose, Settings);
    if (windowModif == NULL) {
        printf("Cannot create second window\n");
        return EXIT_FAILURE;
    }
    /* Set the position of the second window */
    sfRenderWindow_SetPosition(windowModif, 800+10, 50);
    /* Adapt the the window's view to the image => The image will be stretched */
    adaptWindowViewToImage(windowModif, width, height);

    /* Coordinates of the mouse click converted in the image referential*/
    float clickCol, clickRow;
    /* Coordinates of the left point and of the right point */
    unsigned int leftCol = 0, leftRow = 0, rightCol=0, rightRow = 0;
    /* Radius of the SSD calculus */
    unsigned int radius = 1;
    /* Array containing the SSD for every pixel of a selected line */
    double fArray[width];
    /* Column coordinate of the minimum of SSD */
    unsigned int minIdx = 0;
    /* Disparity array, the same size as the image */
    double fMap[height*width];
    /* Density parameter. */ 
    /* step=1 => full image, step>1 => only one pixels out of step */
    unsigned int step = 2;
    /* min and max value for the image representation of the disparity */
    double dMin=-50, dMax=50;



    /* Print some help message for the user */
    printf("Help : \n");
    printf("\t-Press Escape to quit\n");
    printf("\t-Press P for random color\n");
    printf("\t-Press C for copy\n");
    printf("\t-Press D for desaturate\n");
    printf("\n");
    printf("\t-Press N for negative image\n");
    printf("\t-Press R to extract the red component\n");
    printf("\t-Press G to extract the green component\n");
    printf("\t-Press B to extract the blue component\n");
    printf("\t-Press H for the horizontal mirror\n");
    printf("\t-Press V for the vertical mirror\n");
    printf("\t-Press A for the image reverse\n");
    printf("\n");
    printf("\t-Press S for saving the modified image\n");
    printf("\t-Press L to change radius\n");
    printf("\t-Click on left image to select left point and launch automatic search of the right point.\n");
    printf("\t-Click on right image to select right point.\n");
    printf("\t-Press I to change step\n");
    printf("\t-Press O to change min/max\n");
    printf("\t-Press M to compute disparity map (take some time)\n");
    /* ... add your commmand to the list */

    /* Start the game loop */
    while (sfRenderWindow_IsOpened(windowLeft)) {
        /* Process events */
        while (1) {
            // Get events from the main window
            if (sfRenderWindow_GetEvent(windowLeft, &event)==0) {
                // If no event from the main window, get events from the second window
                if (sfRenderWindow_GetEvent(windowRight, &event) == 0) {
                    // If no event from the main window, get events from the second window
                    if (sfRenderWindow_GetEvent(windowModif, &event) == 0) {
                        // If there is also no event from the second window, break out.
                        break;
                    } else {
                        currentWindow = windowModif; // The event come from windowModif
                    }
                } else {
                    currentWindow = windowRight; // The event come from windowRight
                }
            } else {
                currentWindow = windowLeft; // The event come from windowLeft
            }

            /* If close window event : exit */
            if (event.Type == sfEvtClosed) {
                sfRenderWindow_Close(windowLeft);
            }
            if (event.Type == sfEvtMouseButtonPressed) { /* Mouse click ? */
                if (event.MouseButton.Button == sfButtonLeft) { /* Left click ? */

                    /* Convert coordinate from window referential to image referential */
                    sfRenderWindow_ConvertCoords(currentWindow, event.MouseButton.X, event.MouseButton.Y, &clickCol,&clickRow, NULL);
                    printf("Left click at : %d %d (in the mouse referential) and %d %d (in the image referential)\n", event.MouseButton.X, event.MouseButton.Y, (unsigned int)clickCol, (unsigned int)clickRow);

                    if (currentWindow == windowLeft) { /* Click on left image ? */
                        /* Save current mouse position as left point */
                        leftCol = (unsigned int)clickCol;
                        leftRow = (unsigned int)clickRow;

                        /* Automatic search of similar right point */											
                        /* Compute the correlation for the whole line */												
                        computeCorrelation(height, width, size, imageArray, imageArray2, leftCol, leftRow, 3, fArray);
                        /* Search for the minimum SSD of the line */
                        minIdx = idxMinCorrel(width, fArray);
                        /* We set the coordinate of the point in the right image */
                        rightCol = minIdx;
                        rightRow = leftRow;
                        printf("The minimum SSD (with radius %u) is found at index %d\n", radius, minIdx);
                        /* Note the explicit cast from unsigned to signed so we can have a signed difference */
                        printf("Difference of columns %d\n", ((int)leftCol)-minIdx);
                    } else {
                        if (currentWindow == windowRight) { /* Click on right image ? */
                            /* Save current mouse position as right point */
                            rightCol = (unsigned int)clickCol;
                            rightRow = (unsigned int)clickRow;
                            if ((leftCol != 0 || clickRow !=0)) { /* If left point selected */
                                /* Calculate SSD */
                                printf("SSD between I1(%u,%u) & I2(%u,%u) with radius=%u: %lf\n", leftCol, leftRow, rightCol, rightRow, radius, ssd(height, width, size, imageArray, imageArray2, leftCol, leftRow, rightCol, leftRow, radius));
                            }

                        }
                    }
                }
            }
            if (event.Type == sfEvtKeyPressed) {
                switch (event.Key.Code) {
                    case sfKeyEscape : /* If escape key : exit */
                        sfRenderWindow_Close(windowLeft);
                        break;
                    case sfKeyP: /* Random color for the destination image */
                        initImage(size, imageDest, rand()%255, rand()%255, rand()%255, 255);
                        break;
                    case sfKeyC: /* Copy */
                        copyImage(size, imageArray, imageDest);
                        break;
                    case sfKeyD: /* Desaturate */
                        grey256(size, imageArray, imageDest);
                        break;
                    case sfKeyN: /* Negative */
                        negative(maxVal, size, imageArray, imageDest);
                        break;
                    case sfKeyR: /* Extract Red Component */
                        extractRed(size, imageArray, imageDest);
                        break;
                    case sfKeyG: /* Extract Green Component */
                        extractGreen(size, imageArray, imageDest);
                        break;
                    case sfKeyB: /* Extract Blue Component */
                        extractBlue(size, imageArray, imageDest);
                        break;
                    case sfKeyH: /* Horizontal Mirror */
                        mirrorH(height, width, size, imageArray, imageDest);
                        break;
                    case sfKeyV: /* Vertical Mirror */
                        mirrorV(height, width, size, imageArray, imageDest);
                        break;
                    case sfKeyA: /* Reverse */
                        reverse(height, width, size, imageArray, imageDest);
                        break;

                        /* ... your other functions calls here ... */

                    case sfKeyS: /* Save */
                        saveImageAsciiPPM("save.ppm", size, imageDest, height, width, maxVal);
                        printf("Image modifiée sauvée dans le fichier save.ppm\n");
                        break;
                    case sfKeyL: /* Change radius */
                        printf("Old radius %u. New radius ? ", radius);
                        if (scanf("%u", &radius) != 1) {
                            printf("Input error\n");
                        }
                        break;
                    case sfKeyI: /* Change step */
                        printf("Old step %u. New step ? ", step);
                        if (scanf("%u", &step) != 1) {
                            printf("Input error\n");
                        }
                        break;
                    case sfKeyO: /* Change min/max */
                        printf("Old min %lf. Old max %lf\n", dMin, dMax);
                        printf("New min ? ");
                        if (scanf("%lf", &dMin) != 1) {
                            printf("Input error\n");
                        }
                        printf("New max ? ");
                        if (scanf("%lf", &dMax) != 1) {
                            printf("Input error\n");
                        }
                        printf("Printing disparity image with min=%lf and max=%lf.\n", dMin, dMax);
                        /* Apply the change by recomputing disparity image */
                        printDisparityMap(height, width, size, dMin, dMax,  fMap, imageDest);
                        break;
                    case sfKeyM:
                        printf("Starting computing disparity array with radius=%d and step=%d.\n", radius, step);
                        printf("Change radius by pressing L, step by pressing I.\n");
                        printf("Please wait ... (it may take some time. kill with CTRL+C) ...\n");
                        /* Compute the disparity array */
                        doubleDisparityMap(height, width, size, imageArray, imageArray2, radius, step,  fMap);
                        printf("... done.\n");

                        printf("Printing disparity image with min=%lf and max=%lf.\n", dMin, dMax);
                        printf("Change min/max by pressing O.\n");
                        /* Convert to disparity image */
                        printDisparityMap(height, width, size, dMin, dMax,  fMap, imageDest);
                        break;
                    default:
                        break;
                }
            }
        }

        /* Clear the screen */
        sfRenderWindow_Clear(windowLeft, sfBlack);
        sfRenderWindow_Clear(windowRight, sfBlack);
        sfRenderWindow_Clear(windowModif, sfBlack);

        /* Draw the left image */
        drawImage(windowLeft, width, height, size, imageArray);
        /* Draw a circle on the left image where the user has clicked */
        if (leftCol != 0 || leftRow != 0) {
            shape = sfShape_CreateCircle(leftCol, leftRow, 3, sfRed, 0, sfRed);
            sfRenderWindow_DrawShape(windowLeft, shape);
        }
        /* Draw the right image */
        drawImage(windowRight, width, height, size, imageArray2);
        /* Draw the curve of the SSD calculate */
        if(minIdx != 0) { 
            plotCurve(windowRight, height, width, fArray);
        }
        /* Draw a circle on the right image where the user has clicked */
        if (rightCol != 0 || rightRow != 0) {
            shape = sfShape_CreateCircle(rightCol, rightRow, 3, sfRed, 0, sfRed);
            sfRenderWindow_DrawShape(windowRight, shape);
        }
        /* Draw the epipolar line on the two images */
        if(leftCol != 0 || leftRow != 0) { 
            shape = sfShape_CreateLine(0, leftRow, width, leftRow, 1, sfRed, 0, sfRed);
            sfRenderWindow_DrawShape(windowLeft, shape);
            sfRenderWindow_DrawShape(windowRight, shape);
        }
        /* Draw the modified image */
        drawImage(windowModif, width, height, size, imageDest);

        /* Update the window */
        sfRenderWindow_Display(windowLeft);
        sfRenderWindow_Display(windowRight);
        sfRenderWindow_Display(windowModif);
    }

    /* Cleanup resources */
    sfRenderWindow_Destroy(windowLeft);
    sfRenderWindow_Destroy(windowRight);
    sfRenderWindow_Destroy(windowModif);
    sfShape_Destroy(shape);

    return EXIT_SUCCESS;
}

int main()
{
    /* Files declaration */
    char filename[256] = "cowL.ppm";
    char filename2[256] = "cowR.ppm";
    char filenameOut[] = "copie.ppm";

    /* Choosing the size of the image */
    unsigned int height, width, maxVal, height2, width2, maxVal2;

    /* fonctions sfml_avance3.c */
    if (readSizeFromPPM(filename, &height, &width, &maxVal) != 0) {
        printf("Error reading %s !\n", filename);
        return -1;
    }
    if (readSizeFromPPM(filename2, &height2, &width2, &maxVal2) != 0) {
        printf("Error reading %s !\n", filename2);
        return -1;
    }
    if (height!=height2 || width != width2) {
        printf("Les deux images doivent avoir les mêmes dimensions !\n");
        return -1;
    }

    printf("The image size is %u x %u.\n", height, width);
    printf("The channel max value is %u.\n", maxVal);
    printf("We will need an RGBA array of size %u elements.\n", height*width*4);

    /* Size declaration */
    unsigned int size = height*width*4;
    /* Images declaration */
    unsigned char imageArray[size], imageArray2[size], imageArrayCopy[size];

    /* Loading image from file */
    if (loadImageFromPPM(filename, size, imageArray) != 0) {
        printf("Error loading %s !\n", filename);
        return -1;
    }
    if (loadImageFromPPM(filename2, size, imageArray2) != 0) {
        printf("Error loading %s !\n", filename2);
        return -1;
    }

    /* Copy */
    copyImage(size, imageArray, imageArrayCopy);

    /* Save a copy image */
    if(saveImageAsciiPPM(filenameOut, size, imageArrayCopy, height, width, maxVal) == -1)
    {
        printf("Error loading image from %s !\n", filenameOut);
        return -1;
    }

    /* View */
    sfml(maxVal, height, width, maxVal2, height2, width2, size, imageArray, imageArray2, imageArrayCopy);

    return 0;
}

