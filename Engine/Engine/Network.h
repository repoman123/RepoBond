///////////////////////////////////////////////////////////////////////////////
// Filename: Network.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _NETWORKCLASS_H_
#define _NETWORKCLASS_H_


/////////////
// GLOBALS //
/////////////
const int MAX_MESSAGE_SIZE = 512;
const int MAX_QUEUE_SIZE = 200;


/////////////
// LINKING //
/////////////
#pragma comment(lib, "ws2_32.lib")


//////////////
// INCLUDES //
//////////////
#include <winsock2.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "networkmessages.h"
#include <unordered_map>
#include "Ray.h"
using namespace std;


///////////////////////////////////////////////////////////////////////////////
// Class name: Network
///////////////////////////////////////////////////////////////////////////////
class Network
{
private:
	struct QueueType
	{
		bool active;
		struct sockaddr_in address;
		int size;
		char message[MAX_MESSAGE_SIZE];
	};

public:
	struct Client
	{
		float px,py,pz,rx,ry,rz;
		unsigned short id;
	};
	unsigned int clientCount;
	Network();
	Network(const Network&);
	~Network();
	unordered_map<unsigned int,Client*> clients;
	unordered_map<unsigned short,Client*> clientsById;
	bool Initialize(char*, unsigned short);
	void Shutdown();
	void Frame();
	bool clientAdded;
	bool shot;
	Ray shoot;
	unsigned int gunId;
	int GetLatency();
	void SetThreadActive();
	void SetThreadInactive();
	bool Online();
	SOCKET GetClientSocket();

	void ReadNetworkMessage(char*, int, struct sockaddr_in);

	bool SendStateChange(char);
	bool SendPositionUpdate(float, float, float, float, float, float);
	bool SendShootUpdate(float,float,float,float,float,float,char);

private:
	bool InitializeWinSock();
	void ShutdownWinsock();

	bool ConnectToServer(char*, unsigned short);
	void HandlePingMessage();
	void ProcessLatency();
	void SendPing();
	void SendDisconnectMessage();

	void AddMessageToQueue(char*, int, struct sockaddr_in);
	void ProcessMessageQueue();
	void HandleChatMessage(int);
	void HandleEntityInfoMessage(int);
	void HandleNewUserLoginMessage(int);
	void HandleUserDisconnectMessage(int);
	void HandleStateChangeMessage(int);
	void HandlePositionMessage(int);
	void HandleAIRotateMessage(int);
	void HandleShootMessage(int);
	bool SendChatMessage(char*);
	bool RequestEntityList();

private:
	int m_latency;
	SOCKET m_clientSocket;
	int m_addressLength;
	struct sockaddr_in m_serverAddress;
	unsigned short m_idNumber, m_sessionId;
	bool m_online, m_threadActive;
	unsigned long m_pingTime;
	QueueType* m_networkMessageQueue;
	int m_nextQueueLocation, m_nextMessageForProcessing;
	char m_chatMessage[64];
	char m_uiMessage[50];
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
void NetworkReadFunction(void*);


#endif