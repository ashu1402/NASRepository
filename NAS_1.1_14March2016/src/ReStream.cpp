#include "ReStream.h"

ReStream::ReStream(char * portNumber, const char * ipAddressInfo, bool globalLoop)
{
	try{
		ipAddress = (char *)malloc(sizeof(char) * strlen(ipAddressInfo) );
		sprintf( ipAddress,"%s",ipAddressInfo);
		portNo = (char *)malloc(sizeof(char) * strlen(portNumber) );
		sprintf( portNo ,"%s", portNumber);
		//dBA = databaseAccess;
		isServerAttached = false;
		isLocalStream = globalLoop;
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::ReStreamConstructor",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::ReStreamConstructor",e);
	}
}

ReStream::~ReStream(){
	try{
		if (ipAddress != NULL )
			free ( ipAddress );
		if (portNo != NULL )
			free ( portNo );
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::ReStreamDestructor",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::ReStreamdestructor",e);
	}
}


void ReStream::SetIpAddress(char * ipAddressInfo ){
	try{
		sprintf( ipAddress,"%s",ipAddressInfo);
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::SetIpAddress",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::SetIpAddress",e);
	}
}

char* ReStream::getPortNo()
{
	return portNo;
}



void ReStream::setPortNo(char* portNumber)
{
	try{
		portNo = (char *)malloc(sizeof(char) * strlen(portNumber) );
		sprintf( portNo ,"%s", portNumber);
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::setPortNo",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::setPortNo",e);
	}
}


void ReStream::media_constructed (GstRTSPMediaFactory * factory, GstRTSPMedia * media, gpointer user_data)
{
	if ( ISDEBUG )
		cout<<"media_constructed "<<endl;
}

void ReStream::prepared (GstRTSPMedia *gstrtspmedia, gpointer user_data)
{
	if ( ISDEBUG )
		cout<<"prepared "<<endl;
}

void ReStream::unprepared (GstRTSPMedia *gstrtspmedia, gpointer user_data)
{
	if ( ISDEBUG )
		cout<<"media unprepared "<<endl;
}

void ReStream::new_state (GstRTSPMedia *gstrtspmedia, gpointer user_data)
{
	if ( ISDEBUG )
		cout<<"new_state "<<endl;
}

void ReStream::new_stream (GstRTSPMedia *gstrtspmedia, gpointer user_data)
{
	if ( ISDEBUG )
		cout<<"new_stream "<<endl;
}

void ReStream::removed_stream (GstRTSPMedia *gstrtspmedia, gpointer user_data)
{
	if ( ISDEBUG )
		cout<<"removed_stream"<<endl;
}

void ReStream::media_configure (GstRTSPMediaFactory * factory, GstRTSPMedia * media, gpointer user_data)
{
	GMainLoop *loop = (GMainLoop*)(user_data);
	g_signal_connect (media, "prepared", (GCallback) prepared, loop);
	g_signal_connect (media, "new-state", (GCallback)new_state, loop);
	g_signal_connect (media, "new-stream", (GCallback)new_stream, loop);	
	g_signal_connect (media, "removed-stream", (GCallback)removed_stream, loop);
	g_signal_connect (media, "unprepared", (GCallback)unprepared, loop);
	//gst_rtsp_media_set_eos_shutdown (media,TRUE);	
}

void ReStream::need_data_feature (GstElement * appsrc, guint unused, gpointer user_data)
{
	try{
		struct MyContext* ctx = (struct  MyContext*)user_data;

		if(ctx==NULL){
			cout<<"ctx is NUll";
			return;
		}
		else
			if ( ISDEBUG )
				cout<<"buffer size"<<gst_buffer_get_size( ctx->buffer );

		guint size,depth,height,width,step,channels;
		GstFlowReturn ret;
		IplImage* img = cvCreateImage(cvSize(ctx->width,ctx->height),IPL_DEPTH_8U,3);
		guchar *data1;

		CvRect rect;
		rect.width =  ctx->width;
		rect.height = ctx->height;
		rect.x = 0;
		rect.y = 0;


		pthread_mutex_lock(&ctx->imgLock);
		while(ctx->image == NULL){
			if ( ISDEBUG )
				cout<<"inside lock ! ImgBuffer is Empty. Please push images.\n";
		}
		cvSetImageROI( ctx->image, rect );
		cvCopy(ctx->image, img );
		cvResetImageROI(ctx->image);
		if ( ISDEBUG )
			cout<<"inside lock ! Copying Image to local Buffer.\n";
		pthread_mutex_unlock(&ctx->imgLock);

		if( img == NULL ){
			cout<<"Failed to fetch the image:( \n";
			return;
		}
		else
		{

			height    = img->height;
			width     = img->width;
			step      = img->widthStep;
			channels  = img->nChannels;
			depth     = img->depth;
			data1      = (guchar *)img->imageData;
			size = height*width*channels;

			memcpy( (guchar *)ctx->map.data, data1,  gst_buffer_get_size( ctx->buffer ) );

			GST_BUFFER_PTS (ctx->buffer) = ctx->timestamp;
			GST_BUFFER_DURATION (ctx->buffer) = gst_util_uint64_scale_int (1, GST_SECOND, FPS);
			ctx->timestamp += GST_BUFFER_DURATION (ctx->buffer);

			g_signal_emit_by_name (appsrc, "push-buffer", ctx->buffer, &ret);

			if (ret != GST_FLOW_OK) {
				if ( ISDEBUG )
					cout<<"error pushing image\n";
			}
			else{
				if ( ISDEBUG )
					cout<<"Successfully pushed\n";
				if(img!=NULL)
					cvReleaseImage(&img);
			}
		}
	}
	catch(Exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("ReStream","CV::need_data_feature",e);
	}
	catch(exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("ReStream","STD::need_data_feature",e);
	}
}

void ReStream::unprepared_feature (GstRTSPMedia *gstrtspmedia, MyContext* ctx)
{
	try{
		if ( ISDEBUG )
			cout<<"media unprepared_feature "<<endl;
		//cvReleaseCapture(&ctx->cap);
		gst_buffer_unmap (ctx->buffer, &ctx->map);
		gst_buffer_unref(ctx->buffer);
		if ( ISDEBUG )
			cout<<"Successfully released buffers";
	}
	catch(Exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("ReStream","CV::unprepared_feature",e);
	}
	catch(exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("ReStream","STD::unprepared_feature",e);
	}

}

/* called when a new media pipeline is constructed. We can query the
 * pipeline and configure our appsrc */
void ReStream::media_configure_feature (GstRTSPMediaFactory * factory, GstRTSPMedia * media, gpointer user_data)
{
	try{
		GstElement *element, *appsrc;
		struct MyContext *ctx  = (struct MyContext *)user_data;

		element = gst_rtsp_media_get_element (media);

		appsrc = gst_bin_get_by_name_recurse_up (GST_BIN (element), "mysrc");

		gst_util_set_object_arg (G_OBJECT (appsrc), "format", "time");
		g_object_set (G_OBJECT (appsrc), "caps",
				gst_caps_new_simple ("video/x-raw",
						"format", G_TYPE_STRING, "BGR",
						"width", G_TYPE_INT, ctx->width,
						"height", G_TYPE_INT,ctx->height,
						"framerate", GST_TYPE_FRACTION, FPS, 1, NULL),NULL );


		//ctx->white = FALSE;
		ctx->timestamp = 0;
		ctx->buffer =  gst_buffer_new_allocate (NULL, ctx->height * ctx->width*3 , NULL);
		gst_buffer_map (ctx->buffer, &ctx->map, GST_MAP_WRITE);

		/* make sure ther datais freed when the media is gone */
		//g_object_set_data_full (G_OBJECT (media), "my-extra-data", ctx, (GDestroyNotify) g_free);

		g_signal_connect (appsrc, "need-data", (GCallback)need_data_feature, ctx);
		g_signal_connect (media, "unprepared", (GCallback)unprepared_feature, ctx);
		if ( ISDEBUG )
			cout<<"media prepared_feature\n";
		gst_object_unref (appsrc);
		gst_object_unref (element);
	}
	catch(Exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("ReStream","CV::media_configure_feature",e);
	}
	catch(exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("ReStream","STD::media_configure_feature",e);
	}
}

void ReStream::client_connected (GstRTSPServer *gstrtspserver, GstRTSPClient *arg1, gpointer       user_data)
{
	if ( ISDEBUG )
		cout<<"client_connected"<<endl;
}

void ReStream::AddChannelStream(char* rtspUrl,int CID, int DID, char* width,char* height, char* frameRate,char* bitrate, char * outputRtspUrl, bool isResize, int restreamID )
{
	try
	{
		if(isServerAttached){
			GstRTSPMediaFactory *factory;
			char desc[SIZEOFCHARARRAYBIGGER];
			char mountUrl[SIZEOFCHARARRAYBIGGER];

			factory = gst_rtsp_media_factory_new ();

			if( isResize )
				sprintf(desc,"( rtspsrc location=%s latency=0 retry=2 timeout=10000 tcp-timeout=100000 ! rtph264depay ! decodebin ! videoconvert ! videoscale ! video/x-raw,width=%s,height=%s ! videorate ! video/x-raw,framerate=%s/1 ! x264enc bitrate=%s ! rtph264pay name=pay0 pt=96 )", rtspUrl,width,height, frameRate, bitrate);
			else
				sprintf(desc,"( rtspsrc location=%s latency=0 retry=2 timeout=10000 tcp-timeout=100000 do-timestamp=true protocols=tcp ! rtph264depay ! rtph264pay name=pay0 pt=96 )",rtspUrl);

			gst_rtsp_media_factory_set_launch (factory,desc);

			g_signal_connect (factory, "media-configure", (GCallback) media_configure, NULL);
			g_signal_connect (server, "client-connected", (GCallback) client_connected, NULL);
			g_signal_connect (factory, "media-constructed", (GCallback) media_constructed, NULL);

			gst_rtsp_media_factory_set_shared (factory, TRUE);

			if(!isLocalStream ){
				if ( !isResize )
					sprintf(mountUrl,"/BroadCID%dDID%d",CID,DID);
				else
					sprintf(mountUrl,"/BroadCID%dDID%dRES%d",CID,DID,restreamID);
			}
			else{
				if ( !isResize )
					sprintf(mountUrl,"/localCID%dDID%d",CID,DID);
				else
					sprintf(mountUrl,"/localCID%dDID%dRES%d",CID,DID,restreamID);
			}
			gst_rtsp_mount_points_add_factory (mounts, mountUrl, factory);
			gst_rtsp_server_set_backlog (server, 10);
			if( isLocalStream ){
				sprintf( outputRtspUrl,"rtsp://%s:%s%s","127.0.0.1",portNo,mountUrl);
			}else{
				sprintf( outputRtspUrl,"rtsp://%s:%s%s",ipAddress,portNo,mountUrl);
			}
			if ( ISDEBUG ){
				cout<<"\nDesc---"<<outputRtspUrl<<endl;
				cout<<"Channel Add Successful\n";
		}
	}
		else{
			if ( ISDEBUG )
				cout<<"Server is Not Attached. Hence cannot add channel\n";
		}
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::AddChannelStream",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::AddChannelStream",e);
	}
}


//void ReStream::AddVODStream(char* fileName,char * mountUrl, int CID, int DID, char* width,char* height, char* frameRate,char* bitrate, char * outputRtspUrl, bool isResize )
void ReStream::AddVODStream( char* fileName,char * mountUrl, char * outputRtspUrl )
{
	try
	{
		if(isServerAttached){
			GstRTSPMediaFactory *factory;
			char desc[SIZEOFCHARARRAYBIGGER];
			char localUrl[SIZEOFCHARARRAYBIGGER];

			factory = gst_rtsp_media_factory_new ();


			sprintf(desc,"( filesrc location=%s ! decodebin !  x264enc ! rtph264pay name=pay0 pt=96 )",fileName);

			gst_rtsp_media_factory_set_launch (factory,desc);

			gst_rtsp_media_factory_set_shared (factory, FALSE);


			cout<<"mountRl before = "<<mountUrl<<endl;
			sprintf(localUrl,"/%s",mountUrl);
			cout<<"mountRl after = "<<localUrl<<endl;

			gst_rtsp_mount_points_add_factory (mounts, localUrl, factory);
			gst_rtsp_server_set_backlog (server, 10);
			if( isLocalStream ){
				sprintf( outputRtspUrl,"rtsp://%s:%s%s","127.0.0.1",portNo,localUrl);
			}else{
				sprintf( outputRtspUrl,"rtsp://%s:%s%s",ipAddress,portNo,localUrl);
			}
			if ( ISDEBUG ){
				cout<<"\nVODDesc---"<<outputRtspUrl<<endl;
				cout<<"VOD Add Successful\n";
		}
	}
		else{
			if ( ISDEBUG )
				cout<<"Server is Not Attached. Hence cannot add channel\n";
		}
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::AddChannelStream",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::AddChannelStream",e);
	}
}


void ReStream::RemoveVODStream ( char * mountUrl )
{
	try{
		if ( ISDEBUG )
			cout<<"\nRemove MountUrl---->"<<mountUrl;
		gst_rtsp_mount_points_remove_factory (mounts, mountUrl);
		if ( ISDEBUG )
			cout<<"Channel Remove Successful\n";
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::RemoveChannelStream",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::RemoveChannelStream",e);
	}

}

void ReStream::RemoveChannelStream(int CID, int DID,char* width,char* height, char* frameRate,char* bitrate, bool isResize, int restreamID )
{
	try{
		char mountUrl[SIZEOFCHARARRAYBIGGER];

		if(!isLocalStream ){
			if ( !isResize )
				sprintf(mountUrl,"/BroadCID%dDID%d",CID,DID);
			else
				sprintf(mountUrl,"/BroadCID%dDID%dRES%d",CID,DID,restreamID);
		}
		else{
			if ( !isResize )
				sprintf(mountUrl,"/localCID%dDID%d",CID,DID);
			else
				sprintf(mountUrl,"/localCID%dDID%dRES%d",CID,DID,restreamID);
		}
		if ( ISDEBUG )
			cout<<"\nRemove MountUrl---->"<<mountUrl;
		gst_rtsp_mount_points_remove_factory (mounts, mountUrl);
		if ( ISDEBUG )
			cout<<"Channel Remove Successful\n";
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::RemoveChannelStream",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::RemoveChannelStream",e);
	}

}

void ReStream::AddFeatureStream(char* featureName,int COMBOID,char * outputRtspUrl,struct MyContext* ctx)
{
	/*try{
		if(isServerAttached){
			GstRTSPMediaFactory *factory;

			char mountUrl[SIZEOFCHARARRAYBIGGER];

			factory = gst_rtsp_media_factory_new ();
			gst_rtsp_media_factory_set_launch (factory, "( appsrc name=mysrc  ! videoconvert ! x264enc ! rtph264pay name=pay0 pt=96 )");
			g_signal_connect (factory, "media-configure", (GCallback) media_configure_feature, ctx);

			sprintf(mountUrl,"/%s%d",featureName,COMBOID);

			gst_rtsp_mount_points_add_factory (mounts, mountUrl, factory);
			sprintf( outputRtspUrl,"rtsp://%s:%s%s",ipAddress,portNo,mountUrl);
			//dBA->UpdateRTSPIntoChannelRtsp (outputRtspUrl, COMBOID );

			cout<<"\nDesc---"<<outputRtspUrl<<endl;
			cout<<"Feature Add Successful\n";
		}
		else{
			cout<<"Server is Not Attached. Hence cannot add channel\n";
		}
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::AddFeatureStream",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::AddFeatureStream",e);
	}*/
}

void ReStream::RemoveFeatureStream(char* featureName,int COMBOID)
{
	/*try{
		char mountUrl[1000];
		sprintf(mountUrl,"/%s%d",featureName,COMBOID);
		gst_rtsp_mount_points_remove_factory (mounts, mountUrl);
		cout<<"Feature Remove Successful\n";
	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::RemoveFeatureStream",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::RemoveFeatureStream",e);
	}*/
}

void ReStream::ThreadFunc()
{
	try{
		loop = g_main_loop_new (NULL, FALSE);
		//classContext = g_main_context_get_thread_default();

		server = gst_rtsp_server_new ();
		if ( !isLocalStream )
			gst_rtsp_server_set_address (server,ipAddress);
		else
			gst_rtsp_server_set_address (server,"127.0.0.1");
		//gst_rtsp_server_set_service (server, "0");
		mounts = gst_rtsp_server_get_mount_points (server);
		//cout<<"\nin ThreadFunc ---"<<portNo;
		//gst_rtsp_server_set_service (server,portNo);
		gst_rtsp_server_set_service (server,portNo);
		//cout<<"\nin ThreadFunc ---"<<gst_rtsp_server_get_service(server);
		mounts = gst_rtsp_server_get_mount_points (server);
		guint attached = gst_rtsp_server_attach (server, NULL);
		//g_main_context_pop_thread_default(classContext);

		if(attached){
			isServerAttached = true;
			if ( ISDEBUG )
				cout<<"Server Succesfullly attached\n";
			g_main_loop_run (loop);
		}else{
			if ( ISDEBUG )
				cout<<"Failed to attach the server.Exiting From the Code. Free the port first\n";
			exit(0);
			return;
		}

		isServerAttached = false;
		g_object_unref (mounts);
		g_object_unref (server);
		//g_main_loop_unref (loop);

	}
	catch(Exception &e){
		commclass.PrintException("ReStream","CV::ThreadFunc",e);
	}
	catch(exception &e){
		commclass.PrintException("ReStream","STD::ThreadFunc",e);
	}
}
/*
void ReStream::stopServer()
{
	//g_main_context_push_thread_default(classContext);
	if(loop!=NULL){
	if(g_main_is_running(loop)){
		g_main_loop_quit(loop);
	}
	else{
		cout<<"The Server is not in running state. Hence cannot be closed\n";
	}
	}else
		cout<<"loop is NULL";
	//g_main_context_pop_thread_default(classContext);
}
 */
