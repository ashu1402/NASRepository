/* soapReplayBindingProxy.h
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

#ifndef soapReplayBindingProxy_H
#define soapReplayBindingProxy_H
#include "soapH.h"

class SOAP_CMAC ReplayBindingProxy : public soap
{ public:
	/// Endpoint URL of service 'ReplayBindingProxy' (change as needed)
	const char *soap_endpoint;
	/// Variables globally declared in onvif.h (non-static)
	/// Constructor
	ReplayBindingProxy();
	/// Copy constructor
	ReplayBindingProxy(const ReplayBindingProxy& rhs);
	/// Construct from another engine state
	ReplayBindingProxy(const struct soap&);
	/// Constructor with endpoint URL
	ReplayBindingProxy(const char *url);
	/// Constructor with engine input+output mode control
	ReplayBindingProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	ReplayBindingProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	ReplayBindingProxy(soap_mode imode, soap_mode omode);
	/// Destructor deletes deserialized data and engine context
	virtual	~ReplayBindingProxy();
	/// Initializer used by constructors
	virtual	void ReplayBindingProxy_init(soap_mode imode, soap_mode omode);
	/// Create a new copy
	virtual	ReplayBindingProxy *copy() SOAP_PURE_VIRTUAL;
	/// Copy assignment
	ReplayBindingProxy& operator=(const ReplayBindingProxy&);
	/// Delete all deserialized data (with soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to default
	virtual	void reset();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Put SOAP Header in message
	virtual	void soap_header(struct _wsse__Security *wsse__Security, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance, struct wsdd__AppSequenceType *wsdd__AppSequence);
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
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

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_trp__GetServiceCapabilities *trp__GetServiceCapabilities, _trp__GetServiceCapabilitiesResponse &trp__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, trp__GetServiceCapabilities, trp__GetServiceCapabilitiesResponse); }
	virtual	int GetServiceCapabilities(const char *endpoint, const char *soap_action, _trp__GetServiceCapabilities *trp__GetServiceCapabilities, _trp__GetServiceCapabilitiesResponse &trp__GetServiceCapabilitiesResponse);

	/// Web service operation 'GetReplayUri' (returns error code or SOAP_OK)
	virtual	int GetReplayUri(_trp__GetReplayUri *trp__GetReplayUri, _trp__GetReplayUriResponse &trp__GetReplayUriResponse) { return this->GetReplayUri(NULL, NULL, trp__GetReplayUri, trp__GetReplayUriResponse); }
	virtual	int GetReplayUri(const char *endpoint, const char *soap_action, _trp__GetReplayUri *trp__GetReplayUri, _trp__GetReplayUriResponse &trp__GetReplayUriResponse);

	/// Web service operation 'GetReplayConfiguration' (returns error code or SOAP_OK)
	virtual	int GetReplayConfiguration(_trp__GetReplayConfiguration *trp__GetReplayConfiguration, _trp__GetReplayConfigurationResponse &trp__GetReplayConfigurationResponse) { return this->GetReplayConfiguration(NULL, NULL, trp__GetReplayConfiguration, trp__GetReplayConfigurationResponse); }
	virtual	int GetReplayConfiguration(const char *endpoint, const char *soap_action, _trp__GetReplayConfiguration *trp__GetReplayConfiguration, _trp__GetReplayConfigurationResponse &trp__GetReplayConfigurationResponse);

	/// Web service operation 'SetReplayConfiguration' (returns error code or SOAP_OK)
	virtual	int SetReplayConfiguration(_trp__SetReplayConfiguration *trp__SetReplayConfiguration, _trp__SetReplayConfigurationResponse &trp__SetReplayConfigurationResponse) { return this->SetReplayConfiguration(NULL, NULL, trp__SetReplayConfiguration, trp__SetReplayConfigurationResponse); }
	virtual	int SetReplayConfiguration(const char *endpoint, const char *soap_action, _trp__SetReplayConfiguration *trp__SetReplayConfiguration, _trp__SetReplayConfigurationResponse &trp__SetReplayConfigurationResponse);
};
#endif
