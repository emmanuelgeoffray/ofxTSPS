//
//  VideoFile.h
//  openTSPS
//
//  Created by Brett Renfer on 8/7/12.
//
//

#pragma once

#include "ofVideoPlayer.h"
#include "ofxTSPS/source/Source.h"
/*
namespace ofxTSPS {
    class VideoFile : public Source, public ofVideoPlayer {
    public:
        int imgnb;
        VideoFile(){
            type = CAMERA_VIDEOFILE;
        }
        
        bool openSource( int width, int height, string etc="" ){
            tspsWidth = width;
            tspsHeight = height;
            customData = etc;
            bIsOpen = loadMovie( customData );
            setLoopState(OF_LOOP_NORMAL);
            if ( bIsOpen ){
                play();
            }
            return bIsOpen;
        }
        
        void update(){
            ofVideoPlayer::update();
            
            //hackito
            imgnb++;
            ofPixels mypixels = getPixelsRef();
            mypixels.resize(1280, 720);
            ofSaveImage(mypixels,"snaps/snap"+ ofToString(imgnb)+".jpg");

            if ( width != tspsWidth || height != tspsHeight ){
                // is there a better way to do this? probably...
                //resize( tspsWidth, tspsHeight );
            }
#ifdef TARGET_OSX
            if ( bPublishTexture ){
                publishToSyphon( ofVideoPlayer::getTextureReference() );
            }
#endif
        }
        
        void closeSource(){
            stop();
            closeMovie();
            close();
        }
    private:
        
        int tspsWidth, tspsHeight;
        
    };
}
*/
///*
#include "IPVideoGrabber.h"

namespace ofxTSPS {
    class VideoFile : public Source, public ofx::Video::IPVideoGrabber {
    public:
        VideoFile(){
            type = CAMERA_VIDEOFILE;
        }
        
        bool openSource( int width, int height, string etc="" ){
            tspsWidth = width;
            tspsHeight = height;
            customData = etc;
            //ofSetLogLevel(OF_LOG_VERBOSE);
            //bIsOpen = loadMovie( customData );
            bIsOpen = true;
            setURI("http://127.0.0.1:8080/?action=stream");
            setMaxReconnects(ULONG_MAX);
            connect(); // connect immediately
            ofLogNotice("connect");
            return bIsOpen;
        }
        
        void update(){
            IPVideoGrabber::update();
#ifdef TARGET_OSX
            if ( bPublishTexture ){
                publishToSyphon( ofVideoPlayer::getTextureReference() );
            }
#endif
        }
        
        void closeSource(){
            //stop();
            //closeMovie();
            close();
        }
    private:
        
        int tspsWidth, tspsHeight;
        
    };
}
//*/
