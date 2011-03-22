/* 
 * File:   presentation.cpp
 * Author: vorago
 * 
 * Created on December 29, 2010, 11:42 PM
 */


#include "include/presentation.hpp"
#include <stdio.h>
//#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>
#include <gdk/gdkwindow.h>


namespace iwb {

    Presentation::Presentation(int width, int height) {
        buffer = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        slide = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        winPresentFrame = "winPresentation";
        cvNamedWindow(winPresentFrame, CV_WINDOW_AUTOSIZE);
        GdkWindow *w = gtk_widget_get_parent_window((GtkWidget*)cvGetWindowHandle(winPresentFrame));
		gdk_window_fullscreen(w); 
        cvShowImage(winPresentFrame, slide);
        screenWidth = width;
        screenHeight = height;
        
    }

    Presentation::~Presentation() {
        cvReleaseImage(&buffer);
        cvReleaseImage(&slide);
        cvDestroyWindow(winPresentFrame);

    }

    void Presentation::putImage(CvPoint upperLeft, CvPoint lowerRight, IplImage* image) {

        int newWidth, newHeight;
        //calculations of new images coodrinates with respect to offset

        upperLeft.x = upperLeft.x - leftOffset*screenWidth;
        upperLeft.y = upperLeft.y - topOffset*screenHeight;
        lowerRight.x = lowerRight.x - rightOffset*screenWidth;
        lowerRight.y = lowerRight.y - bottomOffset*screenHeight;
       int width = lowerRight.x - upperLeft.x;
        int height = lowerRight.y - upperLeft.y;

        /*
         * We have to find the largest possible size bound by given rectangle
         * (upperLeft and lowerRight) but with respect to original image
         * aspect ratio.
         * Do not used at the moment.
         */
        
        float coeffHeight = (float) height / (float) image->height;
        float coeffWidth = (float) width / (float) image->width;
        if (coeffHeight > coeffWidth) {
            newWidth = int(image->width * coeffWidth);
            newHeight = int(image->height * coeffWidth);
        } else {
            newWidth = int(image->width * coeffHeight);
            newHeight = int(image->height * coeffHeight);
        }
        /*
         * TODO:
         * To shrink a photo, it will generally look best with CV_INTER_AREA
         * interpolation, whereas to enlarge an image, it will generally look
         * best with CV_INTER_CUBIC (slow) or CV_INTER_LINEAR (faster but still
         * looks OK).
         */
    
        IplImage* resizedImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        cvResize(image, resizedImage, CV_INTER_CUBIC);
        //Setting area for new image
        cvSetImageROI(buffer, cvRect(upperLeft.x, upperLeft.y, width, height));
        cvZero(buffer);
        cvCopyImage(resizedImage, buffer);
        cvResetImageROI(buffer);

        cvReleaseImage(&resizedImage);
    }

    void Presentation::clearArea(CvPoint upperLeft, CvPoint lowerRight) {
        CvScalar color = CV_RGB(0, 0, 0);
        cvRectangle(buffer, upperLeft, lowerRight, color, CV_FILLED, 0, 0);
    }

     void Presentation::applyBuffer() {
        cvCopy(buffer, slide);
        cvShowImage(winPresentFrame, slide);
    }

     int Presentation::getScreenWidth(){
         return screenWidth;
     }

     int Presentation::getScreenHeight(){
         return screenHeight;
     }

    void Presentation::drawScroller(Presentation *prs, const char *image1,
            const char *image2, const char *image3, IplImage* leftArrow, IplImage *rightArrow){
        char filepath[80];
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", image1);
        IplImage *img1 = cvLoadImage(filepath, CV_LOAD_IMAGE_UNCHANGED);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", image2);
        IplImage *img2 = cvLoadImage(filepath, CV_LOAD_IMAGE_UNCHANGED);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", image3);
        IplImage *img3 = cvLoadImage(filepath, CV_LOAD_IMAGE_UNCHANGED);
        /* Each image occupies 17% of scroller height and 67% of scroller lenght
         * Each arrow occupies 7% of scroller height and 50% of scroller lenght
         * Space between images and images and arrows - 7%
         */
        //calculations of relative coordinates
        putImage(cvPoint(prs->scrollerUL[0].x, prs->scrollerUL[0].y), cvPoint(prs->scrollerBR[0].x, prs->scrollerBR[0].y), leftArrow);
        putImage(cvPoint(prs->scrollerUL[1].x, prs->scrollerUL[1].y), cvPoint(prs->scrollerBR[1].x, prs->scrollerBR[1].y), img1);
         putImage(cvPoint(prs->scrollerUL[2].x, prs->scrollerUL[2].y), cvPoint(prs->scrollerBR[2].x, prs->scrollerBR[2].y), img2);
        putImage(cvPoint(prs->scrollerUL[3].x, prs->scrollerUL[3].y), cvPoint(prs->scrollerBR[3].x, prs->scrollerBR[3].y), img3);
        putImage(cvPoint(prs->scrollerUL[4].x, prs->scrollerUL[4].y), cvPoint(prs->scrollerBR[4].x, prs->scrollerBR[4].y), rightArrow);

        applyBuffer();
     }

    void Presentation::drawConfirmation() {

        IplImage *yesButton = cvLoadImage("res/left.jpg", CV_LOAD_IMAGE_UNCHANGED);
        IplImage *noButton = cvLoadImage("res/right.jpg", CV_LOAD_IMAGE_UNCHANGED);

//        putImage(cvPoint(10, 10), cvPoint(50, 50), yesButton);
//        putImage(cvPoint(60, 10), cvPoint(110, 50), noButton);
//        putImage(cvPoint(scrollerUL[0].x, scrollerUL[0].y), cvPoint(scrollerBR[0].x, scrollerBR[0].y), yesButton);
//        putImage(cvPoint(scrollerUL[1].x, scrollerUL[1].y), cvPoint(scrollerBR[1].x, scrollerBR[1].y), noButton);
        printf("ul: %d,%d , br: %d,%d", confirmationUL[0].x,  confirmationUL[0].y,  confirmationBR[0].x,  confirmationBR[0].y);
        putImage(cvPoint(confirmationUL[0].x, confirmationUL[0].y), cvPoint(confirmationBR[0].x, confirmationBR[0].y), yesButton);
        putImage(cvPoint(confirmationUL[1].x, confirmationUL[1].y), cvPoint(confirmationBR[1].x, confirmationBR[1].y), noButton);


        applyBuffer();
    }

    void Presentation::drawComponents() {
        for (std::list<Drawable*>::iterator component = components.begin(); component != components.end(); component++) {
            (*component)->draw(this);
        }

        applyBuffer();
    }

    void Presentation::addComponent(Drawable* component) {
        components.push_back(component);
    }

    void Presentation::removeComponent(Drawable* component) {
        components.remove(component);
    }

    void Presentation::drawDiff(IplImage* diff) {

            CvPoint o, p;
            o.x = 0;
            o.y = 0;
            p.x = screenWidth;
            p.y = screenHeight;
            putImage(o,p,diff);
            applyBuffer();
    }
}
