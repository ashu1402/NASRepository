


#include "Notification.h"

char text[SIZEOFCHARARRAYBIG][SIZEOFCHARARRAYBIG];

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct upload_status *upload_ctx = (struct upload_status *)userp;
	const char *data;
	if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}
	data = (const char *)text[upload_ctx->lines_read];
	if(data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;
		return len;
	}
	return 0;
}


Notification::Notification(){

}


void Notification::CreateMail(char * to , char * from , char * cc, char * subject,char * containText, char *signature ){
	int index = 0;
	if ( to != NULL ){
		sprintf(text[index],"To: %s\r\n",to);
		index ++;
	}
	if ( from != NULL ){
		sprintf(text[index],"From: %s\r\n", from);
		index ++;
	}
	if ( cc != NULL){
		sprintf(text[index],"Cc: %s (Another example User)\r\n",cc);
		index ++;
	}
	if ( subject != NULL ){
		sprintf(text[index],"Subject: %s\r\n", subject);
		index++;
	}
	sprintf(text[index],"\r\n"); /* empty line to divide headers from body, see RFC5322 */
	index++;
	if ( containText != NULL ){
		sprintf(text[index],"%s\r\n",containText);
		index++;
	}
	sprintf(text[index],"\r\n"); /* empty line to divide headers from body, see RFC5322 */
		index++;
		sprintf(text[index],"\r\n"); /* empty line to divide headers from body, see RFC5322 */
			index++;
			sprintf(text[index],"\r\n"); /* empty line to divide headers from body, see RFC5322 */
				index++;
				sprintf(text[index],"\r\n"); /* empty line to divide headers from body, see RFC5322 */
					index++;
	if ( signature != NULL ){
			sprintf(text[index],"%s\r\n",signature);
			index ++;
		}
}


void Notification::SendMail(char * from ,char * to , char * cc, char *subject, char * containText , char * signature){
		CURL *curl;
		CURLcode res = CURLE_OK;
		struct curl_slist *recipients = NULL;
		struct upload_status upload_ctx1;
		upload_ctx1.lines_read = 0;
		CreateMail( to , from , cc, subject, containText ,signature );
		curl = curl_easy_init();
		if(curl) {
			cout<<"\nSender account details are missing";
			curl_easy_setopt(curl, CURLOPT_USERNAME, "ashu@dakshaimaging.com");
			curl_easy_setopt(curl, CURLOPT_PASSWORD, "ashu.1402");
			curl_easy_setopt(curl, CURLOPT_URL, "smtp://mail.dakshaimaging.com");
			curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
			recipients = curl_slist_append(recipients, to);
			recipients = curl_slist_append(recipients, cc);
			curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
			curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx1);
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
						curl_easy_strerror(res));
			curl_slist_free_all(recipients);
			curl_easy_cleanup(curl);
	}
}

void Notification::SendMailData( char *msg, char *channelName, char *serverName,char * CID,char * DID){

}



