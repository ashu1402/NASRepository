/* soapPullPointSubscriptionBindingService.h
   Generated by gSOAP 2.8.27 from onvif.h

gSOAP XML Web services tools
Copyright (C) 2000-2015, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapPullPointSubscriptionBindingService_H
#define soapPullPointSubscriptionBindingService_H
#include "soapH.h"
class SOAP_CMAC PullPointSubscriptionBindingService : public soap
{ public:
	/// Variables globally declared in onvif.h (non-static)
	/// Constructor
	PullPointSubscriptionBindingService();
	/// Copy constructor
	PullPointSubscriptionBindingService(const PullPointSubscriptionBindingService&);
	/// Construct from another engine state
	PullPointSubscriptionBindingService(const struct soap&);
	/// Constructor with engine input+output mode control
	PullPointSubscriptionBindingService(soap_mode iomode);
	/// Constructor with engine input and output mode control
	PullPointSubscriptionBindingService(soap_mode imode, soap_mode omode);
	/// Destructor deletes deserialized data and engine context
	virtual ~PullPointSubscriptionBindingService();
	/// Delete all deserialized data (with soap_destroy() and soap_end())
	virtual	void destroy();
	/// Delete all deserialized data and reset to defaults
	virtual	void reset();
	/// Initializer used by constructor
	virtual	void PullPointSubscriptionBindingService_init(soap_mode imode, soap_mode omode);
	/// Create a new copy
	virtual	PullPointSubscriptionBindingService *copy() SOAP_PURE_VIRTUAL;
	/// Copy assignment
	PullPointSubscriptionBindingService& operator=(const PullPointSubscriptionBindingService&);
	/// Close connection (normally automatic)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Return sender-related fault to sender
	virtual	int soap_senderfault(const char *string, const char *detailXML);
	/// Return sender-related fault with SOAP 1.2 subcode to sender
	virtual	int soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML);
	/// Return receiver-related fault to sender
	virtual	int soap_receiverfault(const char *string, const char *detailXML);
	/// Return receiver-related fault with SOAP 1.2 subcode to sender
	virtual	int soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML);
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Put SOAP Header in message
	virtual	void soap_header(struct _wsse__Security *wsse__Security, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance, struct wsdd__AppSequenceType *wsdd__AppSequence);
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Run simple single-thread (iterative, non-SSL) service on port until a connection error occurs (returns error code or SOAP_OK), use this->bind_flag = SO_REUSEADDR to rebind for a rerun
	virtual	int run(int port);
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
	/// Run simple single-thread SSL service on port until a connection error occurs (returns error code or SOAP_OK), use this->bind_flag = SO_REUSEADDR to rebind for a rerun
	virtual	int ssl_run(int port);
#endif
	/// Bind service to port (returns master socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET bind(const char *host, int port, int backlog);
	/// Accept next request (returns socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET accept();
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
	/// Then accept SSL handshake, when SSL is used
	virtual	int ssl_accept();
#endif
	/// Serve this request (returns error code or SOAP_OK)
	virtual	int serve();
	/// Used by serve() to dispatch a request (returns error or SOAP_OK)
	virtual	int dispatch();

	///
	/// Service operations (you should define these):
	/// Note: compile with -DWITH_PURE_VIRTUAL for pure virtual methods
	///

	/// Web service operation 'PullMessages' (returns error code or SOAP_OK)
	virtual	int PullMessages(_tev__PullMessages *tev__PullMessages, _tev__PullMessagesResponse &tev__PullMessagesResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Seek' (returns error code or SOAP_OK)
	virtual	int Seek(_tev__Seek *tev__Seek, _tev__SeekResponse &tev__SeekResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SetSynchronizationPoint' (returns error code or SOAP_OK)
	virtual	int SetSynchronizationPoint(_tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, _tev__SetSynchronizationPointResponse &tev__SetSynchronizationPointResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_tev__GetServiceCapabilities *tev__GetServiceCapabilities, _tev__GetServiceCapabilitiesResponse &tev__GetServiceCapabilitiesResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'CreatePullPointSubscription' (returns error code or SOAP_OK)
	virtual	int CreatePullPointSubscription(_tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, _tev__CreatePullPointSubscriptionResponse &tev__CreatePullPointSubscriptionResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetEventProperties' (returns error code or SOAP_OK)
	virtual	int GetEventProperties(_tev__GetEventProperties *tev__GetEventProperties, _tev__GetEventPropertiesResponse &tev__GetEventPropertiesResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Renew' (returns error code or SOAP_OK)
	virtual	int Renew(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse &wsnt__RenewResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
	virtual	int Unsubscribe(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse &wsnt__UnsubscribeResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Subscribe' (returns error code or SOAP_OK)
	virtual	int Subscribe(_wsnt__Subscribe *wsnt__Subscribe, _wsnt__SubscribeResponse &wsnt__SubscribeResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetCurrentMessage' (returns error code or SOAP_OK)
	virtual	int GetCurrentMessage(_wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, _wsnt__GetCurrentMessageResponse &wsnt__GetCurrentMessageResponse) SOAP_PURE_VIRTUAL;

	/// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
	virtual	int Notify(_wsnt__Notify *wsnt__Notify) SOAP_PURE_VIRTUAL;
	virtual	int send_Notify_empty_response(int httpcode) { return soap_send_empty_response(this, httpcode); }

	/// Web service operation 'GetMessages' (returns error code or SOAP_OK)
	virtual	int GetMessages(_wsnt__GetMessages *wsnt__GetMessages, _wsnt__GetMessagesResponse &wsnt__GetMessagesResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'DestroyPullPoint' (returns error code or SOAP_OK)
	virtual	int DestroyPullPoint(_wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, _wsnt__DestroyPullPointResponse &wsnt__DestroyPullPointResponse) SOAP_PURE_VIRTUAL;

	/// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
	virtual	int Notify_(_wsnt__Notify *wsnt__Notify) SOAP_PURE_VIRTUAL;
	virtual	int send_Notify__empty_response(int httpcode) { return soap_send_empty_response(this, httpcode); }

	/// Web service operation 'CreatePullPoint' (returns error code or SOAP_OK)
	virtual	int CreatePullPoint(_wsnt__CreatePullPoint *wsnt__CreatePullPoint, _wsnt__CreatePullPointResponse &wsnt__CreatePullPointResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Renew' (returns error code or SOAP_OK)
	virtual	int Renew_(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse &wsnt__RenewResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
	virtual	int Unsubscribe_(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse &wsnt__UnsubscribeResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'PauseSubscription' (returns error code or SOAP_OK)
	virtual	int PauseSubscription(_wsnt__PauseSubscription *wsnt__PauseSubscription, _wsnt__PauseSubscriptionResponse &wsnt__PauseSubscriptionResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'ResumeSubscription' (returns error code or SOAP_OK)
	virtual	int ResumeSubscription(_wsnt__ResumeSubscription *wsnt__ResumeSubscription, _wsnt__ResumeSubscriptionResponse &wsnt__ResumeSubscriptionResponse) SOAP_PURE_VIRTUAL;
};
#endif