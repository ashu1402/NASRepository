#include "Dump.h"

Dump::Dump( int CIDInfo, int DIDInfo/*, DataBaseAccess * dBAObj*/ )
{
	try{
		rtspUrl = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIGGER);
		outputFilePath = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIGGER);
		fileName = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIGGER);
		duration = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIGGER);
		CID = CIDInfo;
		DID = DIDInfo;
		//dBA = dBAObj;
	}
	catch(Exception &e){
		commclass.PrintException("Dump","CV::DumpConstructor",e);
	}
	catch(exception &e){
		commclass.PrintException("Dump","STD::DumpConstructor",e);
	}
}

void Dump::setRtspUrl( char* Url )
{
	try{
		sprintf ( rtspUrl,"%s", Url );
	}
	catch(Exception &e){
		commclass.PrintException("Dump","CV::setRtspUrl",e);
	}
	catch(exception &e){
		commclass.PrintException("Dump","STD::setRtspUrl",e);
	}
}

void Dump::setOutputFilePath(char* Url)
{
	try{
		sprintf ( outputFilePath,"%s", Url );
	}
	catch(Exception &e){
		commclass.PrintException("Dump","CV::setOutputFilePath",e);
	}
	catch(exception &e){
		commclass.PrintException("Dump","STD::setOutputFilePath",e);
	}
}

void Dump::setFilePreFix(char* filePreFix)
{
	try{
		sprintf ( fileName,"%s", filePreFix );
	}
	catch(Exception &e){
		commclass.PrintException("Dump","CV::setFilePreFix",e);
	}
	catch(exception &e){
		commclass.PrintException("Dump","STD::setFilePreFix",e);
	}
}

void Dump::setDurationInNano(char* durationInNano)
{
	try{
		sprintf ( duration,"%s", durationInNano );
	}
	catch(Exception &e){
		commclass.PrintException("Dump","CV::setDurationInNano",e);
	}
	catch(exception &e){
		commclass.PrintException("Dump","STD::setDurationInNano",e);
	}
}


gboolean Dump::bus_call (GstBus  *bus, GstMessage *msg, gpointer    data)
{
	try{
		GMainLoop *loop = (GMainLoop *) data;

		switch (GST_MESSAGE_TYPE (msg)) {

		case GST_MESSAGE_EOS:
			printf ("End of stream\n");
			g_main_loop_quit (loop);
			return FALSE;
			break;

		case GST_MESSAGE_ERROR: {
			gchar  *debug;
			GError *error;

			gst_message_parse_error (msg, &error, &debug);
			g_free (debug);

			g_printerr ("Error: in bus %s\n", error->message);
			g_error_free (error);

			g_main_loop_quit (loop);
			return FALSE;
			break;
		}
		case GST_MESSAGE_ANY: {
			printf ("MSG other than error and end of stream\n");
			break;
		}
		default:
			break;
		}

		return TRUE;
	}
	catch(Exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("Dump","CV::bus_call",e);
	}
	catch(exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("Dump","STD::bus_call",e);
	}

	return FALSE;
}

gboolean Dump::TimeoutFunc(gpointer data)
{

	/*	struct DumpStruct* tempStruct = (struct DumpStruct*)data;
		try{
		cout<<"Timeout Function Called\n";
		if(tempStruct==NULL){
			return FALSE;
		}else if ( tempStruct->dBAOBj==NULL){
			return FALSE;
		}
		bool isDumping;
		tempStruct->dBAOBj->GetStorageVariable(tempStruct->COMBOID,isDumping);

		if(isDumping)
		{
			cout<<"Dumping is allowed\n";
			return TRUE;
		}
		else{
			GMainContext* context=g_main_loop_get_context(tempStruct->loop);
			g_main_context_push_thread_default(context);
			cout<<"Dumping is not allowed. Quitting the MAin loop\n";
			if(tempStruct->loop!=NULL){
				if(g_main_loop_is_running(tempStruct->loop)){
					g_main_loop_quit(tempStruct->loop);
					cout<<"Successfully Quitted the main loop\n";
				}
				else{
					cout<<"main loop is not runnning\n";
				}
			}
			else{
				cout<<"loop is NULL\n";
			}
			g_main_context_pop_thread_default(context);
			if(tempStruct!=NULL)
				free(tempStruct);
			return FALSE;
		}
	}
	catch(Exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("Dump","CV::TimeoutFunc",e);
	}
	catch(exception &e){
		CommonClass localcommclass;
		localcommclass.PrintException("Dump","STD::TimeoutFunc",e);
	}
	if(tempStruct!=NULL)
		free(tempStruct);*/
	return FALSE;

}

void Dump::ThreadFunc() {
	try{
		GstBus *bus;
		GstStateChangeReturn ret;
		//struct DumpStruct* dumpObj = (struct DumpStruct*)malloc(sizeof(struct DumpStruct)*1);

		sleep(2);
		loop = g_main_loop_new (NULL, FALSE);
		GError* error = NULL;

		char description[100000];
		char temp[100];

		sprintf( temp,"%s8%s","%0","d");

		sprintf( description,"rtspsrc location=%s latency=0  ! rtph264depay ! h264parse ! splitmuxsink location=%s/%s%s.avi max-size-time = %s", rtspUrl , outputFilePath , fileName, temp, duration);

		pipeline = gst_parse_launch (description, &error);
		if(error!=NULL){
			g_print ("ERROR: in pipeline parse %s\n", error->message);
			g_error_free (error);
		}
		bus = gst_element_get_bus (pipeline);
		if(!bus){
			if ( ISDEBUG )
				cout<<"Failed to obtain the bus\n";
		}
		if(gst_bus_add_watch (bus,bus_call,loop) == 0){
			if ( ISDEBUG )
				cout<<"bus already has an event source\n";
	}
		/*dumpObj->loop=loop;
		dumpObj->dBAOBj=dBA;
		dumpObj->COMBOID=COMBOID;
		*/

		//GSource* source = g_timeout_source_new_seconds ();
		//g_timeout_add(TIMEOUTDURATION,TimeoutFunc,dumpObj);   //Need to be added later

		ret=gst_element_set_state (pipeline, GST_STATE_PLAYING);
		if (ret == GST_STATE_CHANGE_FAILURE) {
	//		GstMessage *msg;
			if ( ISDEBUG )
				g_print ("Failed to start up pipeline!\n");
/*
			msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, 0);
			if (msg) {
				GError *err = NULL;
				gst_message_parse_error (msg, &err, NULL);
				g_print ("ERROR: in state change %s\n", err->message);
				g_error_free (err);
				gst_message_unref (msg);
			}*/
		}

		//cout<<"before main loop run\n";
		g_main_loop_run (loop);

		ret=gst_element_set_state (pipeline, GST_STATE_NULL);
		if (ret == GST_STATE_CHANGE_FAILURE) {
			if ( ISDEBUG )
				g_print ("Failed to change the state of pipeline !\n");
		}
		gst_object_unref (pipeline);
		gst_object_unref (bus);
		//free(dumpObj);
	}
	catch(Exception &e){
		commclass.PrintException("Dump","CV::ThreadFunc",e);
	}
	catch(exception &e){
		commclass.PrintException("Dump","STD::ThreadFunc",e);
	}
}
/*
void Dump::pause()
{
	GstStateChangeReturn ret;
	GstBus* bus = gst_element_get_bus (pipeline);
	ret=gst_element_set_state (pipeline, GST_STATE_PAUSED);
  	if (ret == GST_STATE_CHANGE_FAILURE) {
		GstMessage *msg;

		g_print ("Failed to stop the pipeline!\n");

		msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, 0);
		if (msg) {
			GError *err = NULL;
			gst_message_parse_error (msg, &err, NULL);
			g_print ("ERROR: %s\n", err->message);
			//g_error_free (err);
			gst_message_unref (msg);
		}
	}
	else{
		cout<<"State change successful\n";
	}
}

void Dump::restart()
{
	GstStateChangeReturn ret;
	GstBus* bus = gst_element_get_bus (pipeline);
	ret=gst_element_set_state (pipeline, GST_STATE_PLAYING);
  	if (ret == GST_STATE_CHANGE_FAILURE) {
		GstMessage *msg;

		g_print ("Failed to restart the pipeline!\n");

		msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, 0);
		if (msg) {
			GError *err = NULL;
			gst_message_parse_error (msg, &err, NULL);
			g_print ("ERROR: %s\n", err->message);
			//g_error_free (err);
			gst_message_unref (msg);
		}
	}
	else{
		cout<<"State change successful\n";
	}
}

void Dump::stop()
{
	g_main_loop_quit (loop);
}
 */
