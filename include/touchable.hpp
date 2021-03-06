/* 
 * File:   touchable.hpp
 * Author: fishi, cers
 *
 * Created on Štvrtok, 2011, marec 17, 16:01
 */

#ifndef TOUCHABLE_HPP
#define	TOUCHABLE_HPP

#include "drawable.hpp"
namespace iwb {class Touchable;}
#include "handler.hpp"
#include <opencv/cv.h>
#include <boost/function.hpp>

// These values need tweaking
#define STD_THRESHOLD 2
#define INTERACTION_TRIGGER 2000

namespace iwb {
    /**
     * Base method for everything that can be touched on the whiteboard.
     */
    class Touchable : public Drawable {
    protected:
        CvPoint cameraUL;
        CvPoint cameraBR;
        int getCameraWidth();
        int getCameraHeight();
        boost::function<void (void)> action;
        int interaction;
        int threshold;
        Handler *hndl;
        int id;
    public:
        Touchable(char* imagePath, Presentation *prs, Handler *hndl, CvPoint projectorUL, CvPoint projectorBR, boost::function<void (void)> action , int threshold=STD_THRESHOLD);
        ~Touchable();
        void detectTouch(IplImage* mask);
        CvPoint cfppoint(CvPoint p);
    };
}

#endif	/* TOUCHABLE_HPP */

