/*
 *  ofxTSPS::Settings.h
 *  TSPSPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 
 Portions of this software are distributed under the GPL and the MIT license.
 
 The MIT License
 
 Copyright (c) 2010 Rockwell Group LLC
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

#pragma  once
#include "ofMain.h"
#include "ofxTSPS/Constants.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class Settings {
    public:
        Settings();
        
        string haarFile;
        
        // TSPS general vars
        bool    bMinimized;
        
        // camera vars
        //bool    bUseKinect;
        
        // video file vars
        string  videoDirectory;
        string  videoFile;
        bool    bReloadVideoFiles;
        
        // image vars
        bool    bFlipX, bFlipY;
        bool    bInvert;
        int     smooth;
        int     highpassBlur;
        int     highpassKernel;
        int     highpassNoise;
        int     highpassAmp;
        int     threshold;	
        bool    bHighpass;
        bool    bAmplify;
        bool    bSmooth;
        
        bool    bLearnBackgroundOnStartup, bStartupBgCaptured;
        int     captureSeconds;
        
        bool    bLearnBackground;
        bool    bBlankBackground;
        
        bool    bAdjustedViewInColor;	
        
        bool    bChromaKey;
        float   red_l;
        float   red_h;
        float   green_l;
        float   green_h;
        float   blue_l;
        float   blue_h;

        // tracking vars
        bool    bLearnBackgroundProgressive;
        float   fLearnRate;
        int     trackType;
        bool    bFindHoles;
        float   minBlob;
        float   maxBlob;
        
        //sensing vars
        bool    bTrackOpticalFlow;
        float   minOpticalFlow;
        float   maxOpticalFlow;
        bool    bDetectHaar;
        float   haarAreaPadding;
        bool    bTrackSkeleton;
        //	float   minHaarArea;
        //	float   maxHaarArea;
        
        // communication vars
        string  oscPrefix;
        bool    bSendOsc;
        bool	bUseLegacyOsc;
        bool    bSendTuio;
        bool    bSendTcp;
        
        string  spacebrewHost;
        bool    bSendSpacebrew;
        
        bool    bSendWebSocketServer;
        bool    bSendWebSocketClient;
        string  webSocketHost;
        int     webSocketPort;
        bool    webSocketUseSSL;
        string  webSocketChannel;
        int     webSocketServerPort;
        
        bool    bSendOscContours;
        string  oscHost;
        int     oscPort;
        string  tuioHost;
        int     tuioPort;
        int     tcpPort;
        
        // scene vars
        bool    bSendScene;
        int     sceneGridX, sceneGridY;
        
        // video grabber
        void setSource( Source * source);
        Source*         getSource();
        SourceType      getInputType();
        int             cameraIndex;
        SourceType      inputType;
        
        //layout vars
        int lastCurrentPanel;
        int currentPanel;
        
        //quad warping
        vector <ofPoint> quadWarpScaled;
        vector <ofPoint> quadWarpOriginal;
        
        //xml file
        vector <string> xmlFiles;
        string currentXmlFile;
        
    private:
        Source*         source;
        
        static bool     instanceFlag;
        static Settings *single;
    };
}
