

inline OnvifServerEventProducer::OnvifServerEventProducer()
{
}

inline OnvifServerEventProducer::~OnvifServerEventProducer()
{

}

inline void OnvifServerEventProducer::NotifyMessage()
{
	PullPointSubscriptionBindingProxy m_proxy(soap_new());
	SOAP_ENV__Header m_header;
	m_header.wsa5__Action = (char *) soap_malloc( m_proxy.soap, 1024 );
	strcpy(m_header.wsa5__Action, "http://docs.oasis-open.org/wsn/bw-2/NotificationConsumer/Notify");
	//m_header.wsa__MessageID = NULL;
	//m_header.wsa__To = NULL;
       //m_header.wsa__Action = NULL;
	m_proxy.soap->header = &m_header;
	
	_wsnt__Notify message;
	wsnt__NotificationMessageHolderType *holder = soap_instantiate_wsnt__NotificationMessageHolderType(m_proxy.soap, -1, "", "", NULL);
	wsnt__TopicExpressionType * topic = soap_instantiate_wsnt__TopicExpressionType(m_proxy.soap, -1, "", "", NULL );
	topic->Dialect = "http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete";
	holder->Topic = topic;
	holder->SubscriptionReference = NULL;
	holder->ProducerReference = NULL;
	message.NotificationMessage.push_back(holder);
	Lock();
	std::vector<std::string> localSubs =  m_subscribers;
	UnLock();
        for( std::vector<std::string>::const_iterator it = localSubs.begin();
             it != localSubs.end(); ++it )
	{
	     printf("Notify %s\n", (*it).c_str());
            m_proxy.Notify((*it).c_str(), 
				"http://docs.oasis-open.org/wsn/bw-2/NotificationConsumer/Notify", &message);
        }
}

inline void OnvifServerEventProducer::AddConsumer(std::string consumerEndpoint)
{
	printf("OnvifServerEventProducer::AddConsumer %s\n", consumerEndpoint.c_str());
	Lock();
	if( std::find( m_subscribers.begin(), m_subscribers.end(), consumerEndpoint ) == m_subscribers.end() ) {
		m_subscribers.push_back( consumerEndpoint );
	}
	UnLock();
	
	return;
}

