#include "TCPConnection.h"
#include "XMLParser.h"

TCPConnection::TCPConnection(int value1,Retrieve* Obj,VOD* vodObj, Notification * notifi, bool isAnroidStatus, Failover * failoverObj,Interaction * interactionObj)
{
	portNo =value1;
	timeout.tv_sec = 360000;
	timeout.tv_usec = 0;
	RetrieveObj = Obj;
	VODObj = vodObj;
	notification = notifi;
	isAndroid = isAnroidStatus;
	failover = failoverObj;
	interaction = interactionObj;

}

TCPConnection::~TCPConnection(){
	VODObj = NULL;
		notification = NULL;
		isAndroid = NULL;
		failover = NULL;
		interaction = NULL;
}

void TCPConnection::error(const char *msg){
	perror(msg);
	exit(0);
}


void TCPConnection::addSock(int fd){

	sockfd.push_back(fd);
}

void TCPConnection::deleteSock(int fd)
{
	vector<int>::iterator position = find(sockfd.begin(), sockfd.end(), fd);
	if (position != sockfd.end())
		sockfd.erase(position);
}

void TCPConnection::closeSock(int fd)
{
	close(fd);
}

void TCPConnection::assignPort(int num)
{
	portNo= num;
}


void TCPConnection::ThreadFunc()
{
	char *cli_ip;

	int sockfd,newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portNo);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	socklen_t len = sizeof(serv_addr);

	if (getsockname(sockfd, (struct sockaddr *)&serv_addr,&len) == -1)
		perror("getsockname");
	else{
		portNo =ntohs(serv_addr.sin_port);
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	while (1) {
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		cli_ip = inet_ntoa(cli_addr.sin_addr);
		cout << "\nThe NAS Sending Thread with following ip has made connection:"<<cli_ip << endl;
		addSock(newsockfd);
		if (setsockopt (newsockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
			perror("\nsetsockopt failed\n");
		if (setsockopt (newsockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
			perror("\nsetsockopt failed\n");
		struct threadData* threadObj = (struct threadData*)(malloc)(sizeof(threadData));
		threadObj->obj=RetrieveObj;
		threadObj->sockfd = newsockfd;
		threadObj->vodObj = VODObj;
		threadObj->notification = notification;
		threadObj->isAndroid = isAndroid;
		threadObj->failover = failover;
		threadObj->interaction = interaction;
		sprintf(threadObj->clientIp,"%s",cli_ip);
		pthread_t threadID;
		if(pthread_create(&threadID,NULL,StartXMLParser,(void*)threadObj)!=0)
			cout<<"Erro Creating thread\n";
		else
			cout<<"client Thread Creation Successful\n\n\n";
	}
}

bool TCPConnection::sendData(int size,const void *data,int fd)
{
	int ret = send(fd,data,size,0);
	if(ret>0 && ret!=size){
		cout<<"\nError: Incomplete data sent\n";
		cout<<"\nClosing the socket and deleting the entry:\n";
		closeSock(fd);
		deleteSock(fd);
		return false;
	}
	if(ret<=0){
		switch(errno){
		case EAGAIN: cout<<"\nSend Timeout\n";
		break;
		case ECONNRESET: cout<<"\nConnection forcibly closed by peer:";
		break;
		default:
			printf("\nFailed to send the file. Equal to zero. Closing the socket.\n");
			cout<<strerror(errno)<<endl;
			break;
		}
		cout<<"\nClosing the socket and deleting the entry:\n";
		closeSock(fd);
		deleteSock(fd);
		cout<<"\nSize of sockfd after deletion "<<sockfd.size()<<endl;
		return false;
	}
	return true;

}

void* TCPConnection::StartXMLParser(void * data)
{
	struct threadData* threadObj = (threadData*)(data);
	int fd = threadObj->sockfd;
	XMLParser * XMLParserObj = new XMLParser( fd,threadObj->obj,threadObj->vodObj,threadObj->notification, threadObj->clientIp,threadObj->isAndroid, threadObj->failover, threadObj->interaction);

	XMLParserObj->parse();
	XMLParserObj->CloseUserSession();
	delete XMLParserObj;

	threadObj = NULL;

	cout<<"out of parser  class\n";
	return NULL;
}
