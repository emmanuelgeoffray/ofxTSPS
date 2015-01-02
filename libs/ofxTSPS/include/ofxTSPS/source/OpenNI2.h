//
//  OpenNI2.h
//  openTSPS
//
//  Created by BRenfer on 7/19/13.
//
//

#pragma once


#include "ofxTSPS/source/Source.h"
#include "ofxNI2.h"

namespace ofxTSPS {
    class OpenNI2 : public Source, public ofxNI2::DepthStream {
    public:
        
        OpenNI2() : Source(), ofxNI2::DepthStream()
        {
            type = CAMERA_OPENNI;
            bCanTrackHaar = false;
            bDepthSetup = false;
            bDoProcessFrame = false;
            device = NULL;
            nearClipping = 0;
            farClipping = -1; // will be reset to max value
        }
        
        ~OpenNI2(){
            if ( device != NULL ){
                device->exit();
                delete device;
            }
        }
        
        // core
        bool available(){
            if ( device == NULL ){
                device = new ofxNI2::Device;
                device->setup();
            }
            return (device->listDevices() >= 1);
        }
        
        int numAvailable(){
            if ( device == NULL ){
                device = new ofxNI2::Device;
                device->setup();
            }
            return device->listDevices();
        };
        
        void update(){
            device->update();
            if ( !bDoProcessFrame ){
                bDoProcessFrame = isFrameNew();
            }
            updateTextureIfNeeded();
#ifdef TARGET_OSX
            publishToSyphon( getTextureReference() );
#endif
        }
        
        unsigned char * getPixels(){
            return getPixelsRef().getPixels();
        }
        
        ofPixels & getPixelsRef(){
            static ofPixels retPix;
            depthRemapToRange(ofxNI2::DepthStream::getPixelsRef(), retPix, nearClipping, farClipping, true);
            return retPix;
        }
        
        bool doProcessFrame(){
            bool bReturn = bDoProcessFrame;
            if ( bDoProcessFrame ) bDoProcessFrame = false;
            return bReturn;
        }
        
        // fixed invert...
        inline void depthRemapToRange(const ofShortPixels &src, ofPixels &dst, int nearval, int farval, int invert)
        {
            int N = src.getWidth() * src.getHeight();
            dst.allocate(src.getWidth(), src.getHeight(), 1);
            
            const unsigned short *src_ptr = src.getPixels();
            unsigned char *dst_ptr = dst.getPixels();
            
//            float inv_range = 1. / (far - near);
            
            if (invert)
                std::swap(nearval, farval);
            
            for (int i = 0; i < N; i++)
            {
                unsigned short C = *src_ptr;
                *dst_ptr = C == 0 ? 0 : ofMap(C, nearval, farval, 0, 255, true);
                src_ptr++;
                dst_ptr++;
            }
        }
        
        bool openSource( int width, int height, string etc="" ){
            // setup device?
            if ( device == NULL ){
                device = new ofxNI2::Device;
                //bIsOpen = device->setup();
                device->setup();
                /*
				if ( !bIsOpen ){
					return false;
				}
        */
            }
            
            // only try to attach device once
            if ( !bDepthSetup ){
                bIsOpen = setup(*device);
                if ( farClipping == -1 ) farClipping = stream.getMaxPixelValue();
//                setSize(320, 240);
                setFps(30);
                bDepthSetup  = bIsOpen;
            } else {
                bIsOpen = true;
            }
            
            if (bIsOpen)
            {
                start();
            }
            return bIsOpen;
        }
        
        void closeSource(){
            stream.stop();
            bIsOpen = false;
        }
        
        // Be careful, might be null!
        ofxNI2::Device * getDevice(){
            return device;
        }
        
        void setNearClipping( int nearval ){
            nearClipping = max(0,nearval);
        }
        
        void setFarClipping( int farval ){
            farClipping = min( farval, bIsOpen ? stream.getMaxPixelValue() : 10000 );
        }
        
    private:
        ofxNI2::Device *device;
        bool bDepthSetup, bDoProcessFrame;
        int nearClipping, farClipping;
    };
}
