#include "include/scroller.hpp"
#include "include/camera.hpp"

namespace iwb {

    Scroller::Scroller(Presentation* prs, Handler* hndl) {
        this->prs = prs;
        this->hndl = hndl;
        imgArraySize = 0;
        imgArray = NULL;
        currentImg = 0;
        initialize();
    }

    Scroller::~Scroller() {
        freeFileNameArray();
        freeButtons();
    }

    void Scroller::freeFileNameArray(){
        for (int i = 0; i < imgArraySize; i++) {
            free(imgArray[i]);
        }
        free(imgArray);
        imgArray = NULL;
        imgArraySize = 0;
        currentImg = 0;
    }

    void Scroller::freeButtons() {
        for (int i = 0; i < 5; i++) {
            if (buttons[i] != NULL) {
                deleteButton(buttons[i]);
            }
        }
    }

    void Scroller::deleteButton(Touchable* button) {
        prs->removeComponent(button);
        delete(button);
    }
    
    int Scroller::loadFileNames(){

        /* TODO:
         * reimpliment using scandir
         * make directory name variable
         */
        //check in case of loadFileNames was already called
        if(imgArray != NULL)
            freeFileNameArray();
        DIR *dir;
        struct dirent *ent;
        dir = opendir ("tmp/1");
        if (dir != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                    imgArraySize++;
            }
            rewinddir(dir);
            imgArraySize = imgArraySize-2; //because we do not need to count . and ..
            imgArray = (char**)malloc(imgArraySize * sizeof(char*));
            for (int i = 0; i < imgArraySize; i++) {
                ent = readdir (dir);
                if(!ent){
                    break;
                }
                if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")){
                   imgArray[i] = (char*)malloc(strlen( ent->d_name) + 1);
                   strcpy(imgArray[i], ent->d_name);
                } else {
                    i--;
                }
            }
            closedir (dir);
        } else {
            /* could not open directory */
            return EXIT_FAILURE;
        }
        return 0;
    }

    void Scroller::initialize() {

        for (int i = 0; i < 5; i++) {
            buttons[i] = NULL;
        }
                
        CvPoint projectorUL[5];
        CvPoint projectorBR[5];
        CvPoint cameraUL[5];
        CvPoint cameraBR[5];

        CvPoint upperLeft = cvPoint(0, 0);
        CvPoint lowerRight = cvPoint(800, 200);

        int scrollerWidth = lowerRight.x - upperLeft.x;
        int scrollerHeight = lowerRight.y - upperLeft.y;

        projectorUL[LEFT_ARROW] = cvPoint(
                upperLeft.x + round(0.05 * scrollerWidth),
                upperLeft.y + round(0.25 * scrollerHeight)
                );
        projectorBR[LEFT_ARROW] = cvPoint(
                upperLeft.x + round(0.12 * scrollerWidth),
                upperLeft.y + round(0.75 * scrollerHeight)
                );

        projectorUL[LEFT_IMAGE] = cvPoint(
                upperLeft.x + round(0.17 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[LEFT_IMAGE] = cvPoint(
                upperLeft.x + round(0.34 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[MIDDLE_IMAGE] = cvPoint(
                upperLeft.x + round(0.41 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[MIDDLE_IMAGE] = cvPoint(
                upperLeft.x + round(0.58 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[RIGHT_IMAGE] = cvPoint(
                upperLeft.x + round(0.65 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[RIGHT_IMAGE] = cvPoint(
                upperLeft.x + round(0.82 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[RIGHT_ARROW] = cvPoint(
                upperLeft.x + round(0.88 * scrollerWidth),
                upperLeft.y + round(0.25 * scrollerHeight)
                );
        projectorBR[RIGHT_ARROW] = cvPoint(
                upperLeft.x + round(0.95 * scrollerWidth),
                upperLeft.y + round(0.75 * scrollerHeight)
                );
        
        Camera* camera = Camera::getInstance();
        int screenWidth = camera->getWidth();
        int screenHeight = camera->getHeight();
        float leftOffset = camera->getLeftOffset();
        float rightOffset = camera->getRightOffset();
        float topOffset = camera->getRightOffset();
        float bottomOffset = camera->getBottomOffset();

        cameraUL[LEFT_ARROW] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[LEFT_ARROW].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[LEFT_ARROW].y
                );
        cameraBR[LEFT_ARROW] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[LEFT_ARROW].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[LEFT_ARROW].y
                );

        cameraUL[LEFT_IMAGE] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[LEFT_IMAGE].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[LEFT_IMAGE].y
                );
        cameraBR[LEFT_IMAGE] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[LEFT_IMAGE].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[LEFT_IMAGE].y
                );

        cameraUL[MIDDLE_IMAGE] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[MIDDLE_IMAGE].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[MIDDLE_IMAGE].y
                );
        cameraBR[MIDDLE_IMAGE] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[MIDDLE_IMAGE].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[MIDDLE_IMAGE].y
                );

        cameraUL[RIGHT_IMAGE] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[RIGHT_IMAGE].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[RIGHT_IMAGE].y
                );
        cameraBR[RIGHT_IMAGE] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[RIGHT_IMAGE].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[RIGHT_IMAGE].y
                );

        cameraUL[RIGHT_ARROW] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[RIGHT_ARROW].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[RIGHT_ARROW].y
                );
        cameraBR[RIGHT_ARROW] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[RIGHT_ARROW].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[RIGHT_ARROW].y
                );

        const char* paths[MIDDLE_IMAGE] = {"res/left.jpg", "res/right.jpg"};
//        paths[LEFT_ARROW] = "res/left.jpg";
//        paths[LEFT_IMAGE] = "res/right.jpg";
        // TODO: add image paths
        buttons[LEFT_ARROW] = new Touchable(paths[0], projectorUL[LEFT_ARROW], projectorBR[LEFT_ARROW], cameraUL[LEFT_ARROW], cameraBR[LEFT_ARROW]);
        prs->addComponent(buttons[LEFT_ARROW]);

        buttons[RIGHT_ARROW] = new Touchable(paths[1], projectorUL[RIGHT_ARROW], projectorBR[RIGHT_ARROW], cameraUL[RIGHT_ARROW], cameraBR[RIGHT_ARROW]);
        prs->addComponent(buttons[RIGHT_ARROW]);

//        for (int i = 0; i < 2; i++) {
//            printf("%s\n", paths[i]);
//            buttons[i] = new Touchable(paths[i], projectorUL[i], projectorBR[i], cameraUL[i], cameraBR[i]);
//            prs->addComponent(buttons[i]);
//
//        }
        loadFileNames();

        // FIXME: this code will crash if there are less than three images in the folder
        char filepath[80];
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[0]);
        buttons[LEFT_IMAGE] = new Touchable(filepath, projectorUL[LEFT_IMAGE], projectorBR[LEFT_IMAGE], cameraUL[LEFT_IMAGE], cameraBR[LEFT_IMAGE]);
        prs->addComponent(buttons[LEFT_IMAGE]);

        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[1]);
        buttons[MIDDLE_IMAGE] = new Touchable(filepath, projectorUL[MIDDLE_IMAGE], projectorBR[MIDDLE_IMAGE], cameraUL[MIDDLE_IMAGE], cameraBR[MIDDLE_IMAGE]);
        prs->addComponent(buttons[MIDDLE_IMAGE]);

        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[2]);
        buttons[RIGHT_IMAGE] = new Touchable(filepath, projectorUL[RIGHT_IMAGE], projectorBR[RIGHT_IMAGE], cameraUL[RIGHT_IMAGE], cameraBR[RIGHT_IMAGE]);
        prs->addComponent(buttons[RIGHT_IMAGE]);

        displayImages();
        
    }

    void Scroller::displayImages() {
        int first = currentImg % imgArraySize;
        int second = (currentImg + 1) % imgArraySize;
        int third = (currentImg + 2) % imgArraySize;

        char filepath[80];
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[first]);
        buttons[LEFT_IMAGE]->setImagePath(filepath);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[second]);
        buttons[MIDDLE_IMAGE]->setImagePath(filepath);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[third]);
        buttons[RIGHT_IMAGE]->setImagePath(filepath);
    }

    void Scroller::shiftLeft() {
        currentImg--;
        if (currentImg < 0) {
            currentImg = imgArraySize - 1;
        }

        displayImages();
    }

    void Scroller::shiftRight() {
        currentImg++;
        if (currentImg > imgArraySize - 1) {
            currentImg = 0;
        }

        displayImages();
    }

    void Scroller::displayConfirmation() {
        if (Confirmation::create(prs, hndl)) {
            printf("DEBUG: confirmation dialog created!\n");
        }
    }
}
