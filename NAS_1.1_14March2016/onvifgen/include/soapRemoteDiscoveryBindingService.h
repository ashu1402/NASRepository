/* soapRemoteDiscoveryBindingService.h
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

#ifndef soapRemoteDiscoveryBindingService_H
#define soapRemoteDiscoveryBindingService_H
#include "soapH.h"
class SOAP_CMAC RemoteDiscoveryBindingService : public soap
{ public:
	/// Variables globally declared in onvif.h (non-static)
	/// Constructor
	RemoteDiscoveryBindingService();
	/// Copy constructor
	RemoteDiscoveryBindingService(const RemoteDiscoveryBindingService&);
	/// Construct from another engine state
	RemoteDiscoveryBindingService(const struct soap&);
	/// Constructor with engine input+output mode control
	RemoteDiscoveryBindingService(soap_mode iomode);
	/// Constructor with engine input and output mode control
	RemoteDiscoveryBindingService(soap_mode imode, soap_mode omode);
	/// Destructor deletes deserialized data and engine context
	virtual ~RemoteDiscoveryBindingService();
	/// Delete all deserialized data (with soap_destroy() and soap_end())
	virtual	void destroy();
	/// Delete all deserialized data and reset to defaults
	virtual	void reset();
	/// Initializer used by constructor
	virtual	void RemoteDiscoveryBindingService_init(soap_mode imode, soap_mode omode);
	/// Create a new copy
	virtual	RemoteDiscoveryBindingService *copy() SOAP_PURE_VIRTUAL;
	/// Copy assignment
	RemoteDiscoveryBindingService& operator=(const RemoteDiscoveryBindingService&);
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

	/// Web service operation 'Hello' (returns error code or SOAP_OK)
	virtual	int Hello(struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType &tdn__HelloResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Bye' (returns error code or SOAP_OK)
	virtual	int Bye(struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType &tdn__ByeResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Probe' (returns error code or SOAP_OK)
	virtual	int Probe(struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType &tdn__ProbeResponse) SOAP_PURE_VIRTUAL;
};
#endif
