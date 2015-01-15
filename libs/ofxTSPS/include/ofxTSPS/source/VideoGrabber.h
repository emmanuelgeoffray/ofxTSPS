//
//  VideoGrabber.h
//  openTSPS
//
//  Created by Brett Renfer on 8/7/12.
//
//

#pragma once

#include "ofVideoGrabber.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class VideoGrabber : public Source, public ofVideoGrabber {
    public:
        int imgnb;
        
        VideoGrabber(){
            bCanTrackSkeleton = false;
            setVerbose(false);
            type = CAMERA_VIDEOGRABBER;
        }
        
        int numAvailable(){
            return 1;
        }
        
        bool openSource( int width, int height, string etc="" ){
            setDeviceID(sourceIndex);
            bIsOpen = initGrabber(width,height);
            return bIsOpen;
        }
        
        void update(){
            ofVideoGrabber::update();

            //hackito
            /*
            imgnb++;
            ofPixels mypixels = getPixelsRef();
            mypixels.resize(640,480);
            ofSaveImage(mypixels,"snaps/snap"+ ofToString(imgnb)+".jpg");
            */

#ifdef TARGET_OSX
            if ( bPublishTexture ){
                publishToSyphon( ofVideoGrabber::getTextureReference() );
            }
#endif
        }
        
        bool doProcessFrame(){
            return isFrameNew();
        }
        
        void closeSource(){
            close();
        }
        
    private:
        
        ofPixels grayPixels;
    };
}
