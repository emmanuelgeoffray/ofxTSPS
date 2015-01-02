/*
 *  WebSocketSender.cpp
 *  TSPSPeopleVision
 *
 */

#include "ofxTSPS/communication/WebSocketSender.h"
#include "ofxTSPS/Person.h"
#include "ofxTSPS/Scene.h"

namespace ofxTSPS {
    //---------------------------------------------------------------------------
    WebSocketSender::WebSocketSender(){
        //binary = false;
        port = 8887;
        bServerSetup = bClientSetup = false;
        server = NULL;
        client = NULL;
    }
    
    //---------------------------------------------------------------------------
    WebSocketSender::~WebSocketSender(){
        sockets.clear();
        if ( client != NULL ) client->exit();
        if ( server != NULL ) server->exit();
        bServerSetup = bClientSetup = false;
    }
    
    //---------------------------------------------------------------------------
    bool WebSocketSender::setupClient( string _host, int _port, bool bUseSSL, string channel ){
        port = _port;
        host = _host;
        //sockets.clear();
        
        // setup web socket server
        client = new ofxLibwebsockets::Client();
        client->addListener( this );
        ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
        options.host = _host;
        options.port = _port;
        options.bUseSSL = bUseSSL;
        options.channel = channel;
        
        bClientSetup = client->connect( options );
        return bClientSetup;        
    }
    
    //---------------------------------------------------------------------------
    bool WebSocketSender::setupServer( int _port ){
        serverPort = _port;
        
        ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
        options.port     = _port;
        //options.protocol = "tsps-protocol";
        
        // setup web socket server
        server = new ofxLibwebsockets::Server();
        server->addListener( this ); 
        bServerSetup = server->setup(options);
        return bServerSetup;        
    }
    
    
    //---------------------------------------------------------------------------
    string WebSocketSender::getHost(){
        return host;
    }
    
    //---------------------------------------------------------------------------
    int WebSocketSender::getPort(){
        return port;
    }
    
    //---------------------------------------------------------------------------
    int WebSocketSender::getServerPort(){
        return serverPort;
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::closeClient(){
        if ( client != NULL ){
            client->close();
            client = NULL;
        }
        bClientSetup = false;
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::closeServer(){
        if (server != NULL) server->exit();
        bServerSetup = false;
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::send(){
        for (int i=0; i<toSend.size(); i++){
            for (int j=0; j<sockets.size(); j++){
                if ( bServerSetup && server != NULL ) sockets[j]->send(toSend[i].msg); 
            }
            if ( bClientSetup && client != NULL ){
                client->send( toSend[i].msg );
            }
        }
        
        toSend.clear();
    };
    
    //---------------------------------------------------------------------------
    // ADVANCED: APPEND DATA TO WS JSON
    //---------------------------------------------------------------------------
    void WebSocketSender::setAppendData( string data ){
        appendData = data;
        if ( appendData[0] != ',' ){
            ofLog( OF_LOG_WARNING, "You did not add a comma to your appended data. Adding for you, may be invalid JSON!" );
            appendData = "," + appendData;
        }
    }
    
    //---------------------------------------------------------------------------
    // TSPS EVENTS
    //---------------------------------------------------------------------------
    void WebSocketSender::personEntered ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
        toSend.push_back(WebSocketMessage(p->getJSON("personEntered", cameraWidth,cameraHeight,bSendContours, appendData )));
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::personMoved ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
        toSend.push_back(WebSocketMessage(p->getJSON("personMoved",cameraWidth,cameraHeight,bSendContours, appendData )));
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::personUpdated ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){	
        toSend.push_back(WebSocketMessage(p->getJSON("personUpdated", cameraWidth,cameraHeight,bSendContours, appendData )));
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::personWillLeave ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
        toSend.push_back(WebSocketMessage(p->getJSON("personWillLeave", cameraWidth,cameraHeight,bSendContours, appendData )));
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::customEvent( string eventName, string eventData ){
        toSend.push_back(WebSocketMessage("{\"type\":\"customEvent\",\"name\":\"" + eventName + "\", \"data\":\"" + eventData + " \"}"));
    }
    
    //---------------------------------------------------------------------------
    void WebSocketSender::customEvent( string eventName, vector<string>params ){
        string jsonString = "{\"type\":\"customEvent\",\"name\":\"" + eventName + "\", \"data\":{ \"params\":[";
        for ( int i=0; i<params.size(); i++){
            jsonString += "\"" + params[i] + "\"";
            if ( i + 1 < params.size() ){
                jsonString += ",";
            }
        }
        jsonString += "]}}";
        toSend.push_back(WebSocketMessage(jsonString));
    }
    
    //--------------------------------------------------------------
    void WebSocketSender::customEvent( string eventName, map<string,string>params ){
        string jsonString = "{\"type\":\"customEvent\",\"name\":\"" + eventName + "\", \"data\":{";
                
        map<string,string>::iterator it;
        int index = 0;
        for ( it = params.begin(); it != params.end(); it){
            
            jsonString += "\"" + (*it).first + "\":\"" + (*it).second + "\"";
            if ( ++it != params.end() ){
                jsonString += ",";
            }
        }
        jsonString += "}}";
        toSend.push_back(WebSocketMessage(jsonString));
    }
    
    //--------------------------------------------------------------
    void WebSocketSender::sceneUpdated( Scene & s ){
        toSend.push_back(WebSocketMessage(s.getJSONMessge()));
    }
    
    //---------------------------------------------------------------------------
    // WEBSOCKET EVENTS
    //--------------------------------------------------------------
    void WebSocketSender::onConnect(ofxLibwebsockets::Event& args){
        sockets.push_back(&args.conn);
    }
    
    void WebSocketSender::onOpen(ofxLibwebsockets::Event& args) {
        sockets.push_back(&args.conn);
    }
    
    //--------------------------------------------------------------
    void WebSocketSender::onMessage(ofxLibwebsockets::Event& args) {
        // here you can send stuff back from the web if need be
        //args.message
        //cout<<"message? "<<args.message<<endl;
    }
    
    //--------------------------------------------------------------
    void WebSocketSender::onBroadcast(ofxLibwebsockets::Event& args) {
        // here you can send stuff back from the web if need be
        //args.message
        //cout<<"message? "<<args.message<<endl;
    }
    
    //--------------------------------------------------------------
    void WebSocketSender::onClose(ofxLibwebsockets::Event& args) {
        ofLogWarning() << "[ofxTSPS::WebSocketSender] Connection closed" << std::endl;
        for (int i=0; i<sockets.size(); i++){
            if (sockets[i] == &args.conn){
                std::cout << "removing connection " << std::endl;
                sockets.erase( sockets.begin() + i);
                break;
            }
        }
    }
}
