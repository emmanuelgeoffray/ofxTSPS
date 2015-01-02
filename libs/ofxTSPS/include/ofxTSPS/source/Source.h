//
//  Source.h
//  openTSPS
//
//  Created by Brett Renfer on 7/26/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxTSPS/Constants.h"

#ifdef TARGET_OSX
#include "ofxSyphon.h"
#endif

namespace ofxTSPS {
    class Source :  virtual public ofBaseHasPixels {
    public:
        Source(){
            type = CAMERA_CUSTOM;
            sourceIndex = -1;
            
            // init default settings
            bCanTrackContours       = true;
            bCanTrackHaar           = true;
            bCanTrackOpticalFlow    = true;
            bCanTrackSkeleton       = true;
            bIsOpen                  = false;
            
#ifdef TARGET_OSX
            bPublishTexture         = true;
            setupSyphon();
#endif
        }
        
        // get capabilities
        virtual bool canTrackHaar (){ return bCanTrackHaar; };
        virtual bool canTrackContours (){ return bCanTrackContours; };
        virtual bool canTrackSkeleton (){ return bCanTrackSkeleton; };
        virtual bool canTrackOpticalFlow (){ return bCanTrackOpticalFlow; };
        virtual SourceType getType(){ return type; };
        
        // core
        virtual bool available(){ return true; };
        virtual int  numAvailable(){ return 0; };
        virtual bool openSource( int width, int height, string etc="" ){ return true; };
        virtual void update(){};
        virtual bool doProcessFrame(){ return true; };
        virtual void closeSource(){};;
        
        // methods: settings
        virtual bool setTrackHaar ( bool trackHaar ){
            if ( bCanTrackHaar ){
                bTrackHaar = trackHaar;
            }
            return bTrackHaar;
        };
        
        virtual bool setTrackContours ( bool trackContours ){
            if ( bCanTrackContours ){
                bTrackContours = trackContours;
            }
            return bTrackContours;
        };
        
        virtual bool setTrackSkeleton ( bool trackSkeleton ){
            if ( bCanTrackSkeleton ){
                bTrackSkeleton = trackSkeleton;
            }
            return bTrackSkeleton;
        };
        
        virtual bool setTrackOpticalFlow ( bool trackOpticalFlow ){
            if ( bCanTrackOpticalFlow ){
                bTrackOpticalFlow = trackOpticalFlow;
            }
            return bTrackOpticalFlow;
        };
        
        virtual void setSourceIndex( int which ){
            sourceIndex = which;
        };
        
        virtual string getCustomData(){
            return customData;
        }
        
        virtual bool isOpen(){
            return bIsOpen;
        }
        
        // implemented by 3D-capable sources
        virtual float getDepth( float x, float y ){
            return 0.0f;
        }
        
        // feature: stream via Syphon
#ifdef TARGET_OSX
        virtual void setupSyphon( string name="tspsSyphon" ){
            syphonServer.setName(name);
        };
        
        virtual void setPublishTexture( bool publishTexture=true){
            bPublishTexture = publishTexture;
        }
        
        virtual bool isPublishingTexture(){
            return bPublishTexture;
        }
#endif
        
    protected:
        SourceType  type;
        int         sourceIndex;    // which camera are we opening
        string      customData;     // storage like video file location, etc
        
        bool        bIsOpen;         // is source open yet?
        
        // mirrored by ofxTSPS::Processor
        // capabilities + settings
        bool bCanTrackHaar, bTrackHaar;
        bool bCanTrackContours, bTrackContours;
        bool bCanTrackSkeleton, bTrackSkeleton;
        bool bCanTrackOpticalFlow, bTrackOpticalFlow;
        
        // feature: stream via Syphon
#ifdef TARGET_OSX
        bool            bPublishTexture;
        ofxSyphonServer syphonServer;
        
        
        virtual void publishToSyphon( ofTexture & tex ){
            syphonServer.publishTexture(&tex);
        }
#endif
    };
}
