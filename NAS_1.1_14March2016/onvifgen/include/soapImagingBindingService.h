/* soapImagingBindingService.h
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

#ifndef soapImagingBindingService_H
#define soapImagingBindingService_H
#include "soapH.h"
class SOAP_CMAC ImagingBindingService : public soap
{ public:
	/// Variables globally declared in onvif.h (non-static)
	/// Constructor
	ImagingBindingService();
	/// Copy constructor
	ImagingBindingService(const ImagingBindingService&);
	/// Construct from another engine state
	ImagingBindingService(const struct soap&);
	/// Constructor with engine input+output mode control
	ImagingBindingService(soap_mode iomode);
	/// Constructor with engine input and output mode control
	ImagingBindingService(soap_mode imode, soap_mode omode);
	/// Destructor deletes deserialized data and engine context
	virtual ~ImagingBindingService();
	/// Delete all deserialized data (with soap_destroy() and soap_end())
	virtual	void destroy();
	/// Delete all deserialized data and reset to defaults
	virtual	void reset();
	/// Initializer used by constructor
	virtual	void ImagingBindingService_init(soap_mode imode, soap_mode omode);
	/// Create a new copy
	virtual	ImagingBindingService *copy() SOAP_PURE_VIRTUAL;
	/// Copy assignment
	ImagingBindingService& operator=(const ImagingBindingService&);
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

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_timg__GetServiceCapabilities *timg__GetServiceCapabilities, _timg__GetServiceCapabilitiesResponse &timg__GetServiceCapabilitiesResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetImagingSettings' (returns error code or SOAP_OK)
	virtual	int GetImagingSettings(_timg__GetImagingSettings *timg__GetImagingSettings, _timg__GetImagingSettingsResponse &timg__GetImagingSettingsResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SetImagingSettings' (returns error code or SOAP_OK)
	virtual	int SetImagingSettings(_timg__SetImagingSettings *timg__SetImagingSettings, _timg__SetImagingSettingsResponse &timg__SetImagingSettingsResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetOptions' (returns error code or SOAP_OK)
	virtual	int GetOptions(_timg__GetOptions *timg__GetOptions, _timg__GetOptionsResponse &timg__GetOptionsResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Move' (returns error code or SOAP_OK)
	virtual	int Move(_timg__Move *timg__Move, _timg__MoveResponse &timg__MoveResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Stop' (returns error code or SOAP_OK)
	virtual	int Stop(_timg__Stop *timg__Stop, _timg__StopResponse &timg__StopResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetStatus' (returns error code or SOAP_OK)
	virtual	int GetStatus(_timg__GetStatus *timg__GetStatus, _timg__GetStatusResponse &timg__GetStatusResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetMoveOptions' (returns error code or SOAP_OK)
	virtual	int GetMoveOptions(_timg__GetMoveOptions *timg__GetMoveOptions, _timg__GetMoveOptionsResponse &timg__GetMoveOptionsResponse) SOAP_PURE_VIRTUAL;
};
#endif
