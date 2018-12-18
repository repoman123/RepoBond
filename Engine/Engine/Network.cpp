///////////////////////////////////////////////////////////////////////////////
// Filename: Network.cpp
///////////////////////////////////////////////////////////////////////////////
#include "Network.h"


Network::Network()
{
	m_latency = 0;
	m_networkMessageQueue = 0;
	clientAdded=false;
	clientCount=0;
	shot=false;
}


Network::Network(const Network& other)
{
}


Network::~Network()
{
}


bool Network::Initialize(char* ipAddress, unsigned short serverPort)
{
	bool result;
	int i;


	// Initialize the network message queue.
	m_networkMessageQueue = new QueueType[MAX_QUEUE_SIZE];
	if(!m_networkMessageQueue)
	{
		return false;
	}

	m_nextQueueLocation = 0;
	m_nextMessageForProcessing = 0;

	for(i=0; i<MAX_QUEUE_SIZE; i++)
	{
		m_networkMessageQueue[i].active = false;
	}
	
	// Initialize winsock for using window's sockets.
	result = InitializeWinSock();
	if(!result)
	{
		return false;
	}

	// Connect to the server.
	result = ConnectToServer(ipAddress, serverPort);
	if(!result)
	{
		return false;
	}

	// Send a request to the zone server for the list of user and non-user entities currently online as well as their status.
	result = RequestEntityList();
	if(!result)
	{

	}
	return true;
}


void Network::Shutdown()
{
	// Send a message to the server letting it know this client is disconnecting.
	SendDisconnectMessage();

	// Set the client to be offline.
	m_online = false;

	// Wait for the network I/O thread to complete.
	while(m_threadActive)
	{
		Sleep(5);
	}

	// Close the socket.
	closesocket(m_clientSocket);

	// Shutdown winsock.
	ShutdownWinsock();

	// Release the network message queue.
	if(m_networkMessageQueue)
	{
		delete [] m_networkMessageQueue;
		m_networkMessageQueue = 0;
	}

	return;
}


void Network::Frame()
{
	bool newMessage;


	// Update the network latency.
	//ProcessLatency();

	// Read and process the network messages that are in the queue.
	ProcessMessageQueue();

	// Check if there is a chat message that this user wants to send to the server.

	return;
}

int Network::GetLatency()
{
	return m_latency;
}


bool Network::InitializeWinSock()
{
	unsigned short version;      
	WSADATA wsaData;
	int error;
	unsigned long bufferSize;
	WSAPROTOCOL_INFOW* protocolBuffer;  
	int protocols[2];


	// Create a 2.0 macro to check versions.
	version = MAKEWORD(2, 0);
	
	// Get the data to see if it handles the version we want.
	error = WSAStartup(version, &wsaData);
    if(error != 0)
	{
		return false;
	}
	
	// Check to see if the winsock dll is version 2.0.
	if((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 0))
	{
		return false;
	}

	// Request the buffer size needed for holding the protocols available.
	WSAEnumProtocols(NULL, NULL, &bufferSize);

	// Create a buffer for the protocol information structs.
	protocolBuffer = new WSAPROTOCOL_INFOW[bufferSize];
	if(!protocolBuffer)
	{
		return false;
	}

	// Create the list of protocols we are looking for which are TCP and UDP.
	protocols[0] = IPPROTO_TCP;
	protocols[1] = IPPROTO_UDP;

	// Retrieve information about available transport protocols, if no socket error then the protocols from the list will work.
	error = WSAEnumProtocols(protocols, protocolBuffer, &bufferSize);
	if(error == SOCKET_ERROR)
	{
		return false;
	}

	// Release the protocol buffer.
	delete [] protocolBuffer;
	protocolBuffer = 0;

	return true;
}


void Network::ShutdownWinsock()
{
	WSACleanup();
	return;
}


bool Network::ConnectToServer(char* ipAddress, unsigned short portNumber)
{
	unsigned long setting, inetAddress, startTime, threadId;
	int error, bytesSent, bytesRead;
	MSG_GENERIC_DATA connectMessage;
	bool done, gotId;
	char recvBuffer[4096];
	MSG_NEWID_DATA* message;
	HANDLE threadHandle;


	// Create a UDP socket.
	m_clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_clientSocket == INVALID_SOCKET)
	{
		return false;
	}

	// Set the client socket to non-blocking I/O.
	setting = 1;
	error = ioctlsocket(m_clientSocket, FIONBIO, &setting);
	if(error == SOCKET_ERROR)
	{
		return false;
	}

	// Save the size of the server address structure.
	m_addressLength = sizeof(m_serverAddress);

	// Setup the address of the server we are sending data to.
	inetAddress = inet_addr(ipAddress);
	memset((char*)&m_serverAddress, 0, m_addressLength);
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(portNumber);
	m_serverAddress.sin_addr.s_addr = inetAddress;

	// Setup a connect message to send to the server.
	connectMessage.type = MSG_CONNECT;

	// Send the connect message to the server.
	bytesSent = sendto(m_clientSocket, (char*)&connectMessage, sizeof(MSG_GENERIC_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent < 0)
	{
		return false;
	}

	// Record the time when the connect packet was sent.
	startTime = timeGetTime();

	// Set the boolean loop values.
	done = false;
	gotId = false;

	// Loop for two seconds checking for the ID return message.
	while(!done)
	{
		// Check for a reply message from the server.
		bytesRead = recvfrom(m_clientSocket, recvBuffer, 4096, 0, (struct sockaddr*)&m_serverAddress, &m_addressLength);
		if(bytesRead > 0)
		{
			done = true;
			gotId = true;
		}

		// Check to see if this loop has been running for longer than 2 seconds.
		if(timeGetTime() > (startTime + 2000))
		{
			done = true;
			gotId = false;
		}
	}

	// If it didn't get an ID in 2 seconds then the server was not up.
	if(!gotId)
	{
		return false;
	}

	// Coerce the network message that was received into a new id message type.
	message = (MSG_NEWID_DATA*)recvBuffer;

	// Ensure it was a new id message.
	if(message->type != MSG_NEWID)
	{
		return false;
	}

	// Store the ID number for this client for all future communication with the server.
	m_idNumber = message->idNumber;
	m_sessionId = message->sessionId;
	
	// Set the client to be online now.
	m_online = true;
	
	// Initialize the thread activity variable.
	m_threadActive = false;
	
	// Create a thread to listen for network I/O from the server.
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NetworkReadFunction, (void*)this, 0, &threadId);
	if(threadHandle == NULL)
	{
		return false;
	}
	
	// Initialize the network latency variables.
	m_pingTime = timeGetTime();

	return true;
}


void NetworkReadFunction(void* ptr)
{
	Network* networkClassPtr;
	struct sockaddr_in serverAddress;
	int addressLength;
	int bytesRead;
	char recvBuffer[4096];


	// Get a pointer to the calling object.
	networkClassPtr = (Network*)ptr;

	// Notify parent object that this thread is now active.
	networkClassPtr->SetThreadActive();

	// Set the size of the address.
	addressLength = sizeof(serverAddress);

	// Loop and read network messages while the client is online.
	while(networkClassPtr->Online())
	{
	    // Check if there is a message from the server.
		bytesRead = recvfrom(networkClassPtr->GetClientSocket(), recvBuffer, 4096, 0, (struct sockaddr*)&serverAddress, &addressLength);
		if(bytesRead > 0)
		{
			networkClassPtr->ReadNetworkMessage(recvBuffer, bytesRead, serverAddress);
		}
	}

	// Notify parent object that this thread is now inactive.
	networkClassPtr->SetThreadInactive();

	// Release the pointer.
	networkClassPtr = 0;

	return;
}


void Network::SetThreadActive()
{
	m_threadActive = true;
	return;
}


void Network::SetThreadInactive()
{
	m_threadActive = false;
	return;
}


bool Network::Online()
{
	return m_online;
}


SOCKET Network::GetClientSocket()
{
	return m_clientSocket;
}


void Network::ReadNetworkMessage(char* recvBuffer, int bytesRead, struct sockaddr_in serverAddress)
{
	MSG_GENERIC_DATA* message;


	// Check that the address the message came from is the correct IP address from the server and not a hack attempt from someone else.


	// Check for buffer overflow.
	if(bytesRead > MAX_MESSAGE_SIZE)
	{
		return;
	}

	// If it is a ping message then process it immediately for accurate stats.
	message = (MSG_GENERIC_DATA*)recvBuffer;
	if(message->type == MSG_PING)
	{
		HandlePingMessage();
	}
	// Otherwise place the message in the queue to be processed during the frame processing for the network.
	else
	{
		AddMessageToQueue(recvBuffer, bytesRead, serverAddress);
	}

	return;
}


void Network::HandlePingMessage()
{
	m_latency = timeGetTime() - m_pingTime;
	return;
}


void Network::ProcessLatency()
{
	// If 5 seconds is up then send a ping message to the server.
	if(timeGetTime() >= (m_pingTime + 5000))
	{
		m_pingTime = timeGetTime();
		SendPing();
	}

	return;
}


void Network::SendPing()
{
	MSG_PING_DATA message;
	int bytesSent;

	
	// Create the ping message.
	message.type = MSG_PING;
	message.idNumber = m_idNumber;
	message.sessionId = m_sessionId;

	// Send the ping message to the server.
	bytesSent = sendto(m_clientSocket, (char*)&message, sizeof(MSG_PING_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent != sizeof(MSG_PING_DATA))
	{

	}

	return;
}


void Network::SendDisconnectMessage()
{
	MSG_DISCONNECT_DATA message;
	int bytesSent;

	
	// Create the disconnect message.
	message.type = MSG_DISCONNECT;
	message.idNumber = m_idNumber;
	message.sessionId = m_sessionId;

	// Send the disconnect message to the server.
	bytesSent = sendto(m_clientSocket, (char*)&message, sizeof(MSG_DISCONNECT_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent != sizeof(MSG_DISCONNECT_DATA))
	{

	}

	return;
}


void Network::AddMessageToQueue(char* message, int messageSize, struct sockaddr_in serverAddress)
{
	// Check for buffer overflow.
	if(messageSize > MAX_MESSAGE_SIZE)
	{
	}

	// Otherwise add it to the circular message queue to be processed.
	else
	{
		m_networkMessageQueue[m_nextQueueLocation].address = serverAddress;
		m_networkMessageQueue[m_nextQueueLocation].size = messageSize;
		memcpy(m_networkMessageQueue[m_nextQueueLocation].message, message, messageSize);

		// Set active last so that racing conditions in processing the queue do not exist.
		m_networkMessageQueue[m_nextQueueLocation].active = true;

		// Increment the queue position.
		m_nextQueueLocation++;
		if(m_nextQueueLocation == MAX_QUEUE_SIZE)
		{
			m_nextQueueLocation = 0;
		}
	}

	return;
}


void Network::ProcessMessageQueue()
{
	MSG_GENERIC_DATA* message;


	// Loop through all process all the active messages in the queue.
	while(m_networkMessageQueue[m_nextMessageForProcessing].active == true)
	{
		// Coerce the message into a generic format to read the type of message.
		message = (MSG_GENERIC_DATA*)m_networkMessageQueue[m_nextMessageForProcessing].message;

		switch(message->type)
		{
			case MSG_CHAT:
			{
				HandleChatMessage(m_nextMessageForProcessing);
				break;
			}
			case MSG_ENTITY_INFO:
			{
				HandleEntityInfoMessage(m_nextMessageForProcessing);
				break;
			}
			case MSG_NEW_USER_LOGIN:
			{
				HandleNewUserLoginMessage(m_nextMessageForProcessing);
				break;
			}
			case MSG_USER_DISCONNECT:
			{
				HandleUserDisconnectMessage(m_nextMessageForProcessing);
				break;
			}
			case MSG_STATE_CHANGE:
			{
				HandleStateChangeMessage(m_nextMessageForProcessing);
				break;
			}
			case MSG_POSITION:
			{
				HandlePositionMessage(m_nextMessageForProcessing);
				break;
			}
 			case MSG_AI_ROTATE:
			{
				HandleAIRotateMessage(m_nextMessageForProcessing);
				break;
			}
           default:
			{
		        break;
			}
		}

		// Set the message as processed.
	    m_networkMessageQueue[m_nextMessageForProcessing].active = false;

	    // Increment the queue position.
	    m_nextMessageForProcessing++;
		if(m_nextMessageForProcessing == MAX_QUEUE_SIZE)
		{
		m_nextMessageForProcessing = 0;
		}
	}
	
	return;
}


void Network::HandleChatMessage(int queuePosition)
{
	MSG_CHAT_DATA* msg;
	unsigned short clientId;


	// Confirm this came from the server and not someone else.
	//VerifyServerMessage();

	// Get the contents of the message.
	msg = (MSG_CHAT_DATA*)m_networkMessageQueue[queuePosition].message;
	clientId = msg->idNumber;

	// Copy text into a string of a specific size.
	strcpy_s(m_chatMessage, 64, msg->text);

	return;
}


void Network::HandleEntityInfoMessage(int queuePosition)
{
	MSG_ENTITY_INFO_DATA* message;
	unsigned short entityId;
	char entityType;
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;


	// Confirm this came from the server and not someone else.

	// Get the contents of the message.
	message = (MSG_ENTITY_INFO_DATA*)m_networkMessageQueue[queuePosition].message;

	entityId   = message->entityId;
	entityType = message->entityType;
	positionX  = message->positionX;
	positionY  = message->positionY;
	positionZ  = message->positionZ;
	rotationX  = message->rotationX;
	rotationY  = message->rotationY;
	rotationZ  = message->rotationZ;

	return;
}


void Network::HandleNewUserLoginMessage(int queuePosition)
{
	MSG_ENTITY_INFO_DATA* message;
	unsigned short entityId;
	char entityType;
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	Client* cli = new Client();

	// Confirm this came from the server and not someone else.

	// Get the contents of the message.
	message = (MSG_ENTITY_INFO_DATA*)m_networkMessageQueue[queuePosition].message;

	entityId   = message->entityId;
	entityType = message->entityType;
	positionX  = message->positionX;
	positionY  = message->positionY;
	positionZ  = message->positionZ;
	rotationX  = message->rotationX;
	rotationY  = message->rotationY;
	rotationZ  = message->rotationZ;

	cli->px=positionX;
	cli->py=positionY;
	cli->pz=positionZ;

	cli->rx=rotationX;
	cli->ry=rotationY;
	cli->rz=rotationZ;
	cli->id=entityId;
	clients.insert(pair<unsigned int,Client*>(clientCount,cli));
	this->clientsById.insert(pair<unsigned short,Client*>(entityId,cli));
	clientCount++;
	clientAdded=true;
	return;
}


void Network::HandleUserDisconnectMessage(int queuePosition)
{
	MSG_USER_DISCONNECT_DATA* message;
	unsigned short entityId;


	// Confirm this came from the server and not someone else.

	// Get the contents of the message.
	message = (MSG_USER_DISCONNECT_DATA*)m_networkMessageQueue[queuePosition].message;

	entityId  = message->idNumber;


	return;
}


void Network::HandleStateChangeMessage(int queuePosition)
{
	MSG_STATE_CHANGE_DATA* message;
	unsigned short entityId;
	char state;


	// Confirm this came from the server and not someone else.

	// Get the contents of the message.
	message = (MSG_STATE_CHANGE_DATA*)m_networkMessageQueue[queuePosition].message;

	entityId = message->idNumber;
	state = message->state;
	
	return;
}


void Network::HandlePositionMessage(int queuePosition)
{
	MSG_POSITION_DATA* message;
	unsigned short entityId;
	float positionX, positionY, positionZ, rotationX, rotationY, rotationZ;


	// Confirm this came from the server and not someone else.

	// Get the contents of the message.
	message = (MSG_POSITION_DATA*)m_networkMessageQueue[queuePosition].message;

	entityId = message->sessionId;
	positionX = message->positionX;
	positionY = message->positionY;
	positionZ = message->positionZ;
	rotationX = message->rotationX;
	rotationY = message->rotationY;
	rotationZ = message->rotationZ;
	clientsById[entityId]->px=positionX;
	clientsById[entityId]->py=positionY;
	clientsById[entityId]->pz=positionZ;

	clientsById[entityId]->rx=rotationX;
	clientsById[entityId]->ry=rotationY;
	clientsById[entityId]->rz=rotationZ;
	return;
}


void Network::HandleShootMessage(int queuePosition)
{
	MSG_SHOOT_DATA* message;
	unsigned short entityId;
	float positionX, positionY, positionZ, rotationX, rotationY, rotationZ;


	// Confirm this came from the server and not someone else.

	// Get the contents of the message.
	message = (MSG_SHOOT_DATA*)m_networkMessageQueue[queuePosition].message;
	char gunId;
	entityId = message->idNumber;
	positionX = message->px;
	positionY = message->py;
	positionZ = message->pz;
	rotationX = message->dx;
	rotationY = message->dy;
	rotationZ = message->dz;
	gunId = message->gunId;
	shot=true;
	shoot.origPos.x=message->px;
	shoot.origPos.y=message->py;
	shoot.origPos.z=message->pz;
	shoot.origDir.x=message->dx;
	shoot.origDir.y=message->dy;
	shoot.origDir.z=message->dz;
	gunId = message->gunId;
	return;
}


void Network::HandleAIRotateMessage(int queuePosition)
{
	MSG_AI_ROTATE_DATA* message;
	unsigned short entityId;
	bool rotate;


	// Confirm this came from the server and not someone else.

	// Get the contents of the message.
	message = (MSG_AI_ROTATE_DATA*)m_networkMessageQueue[queuePosition].message;
	entityId = message->idNumber;
	rotate = message->rotate;

	return;
}


bool Network::SendChatMessage(char* inputMsg)
{
	MSG_CHAT_DATA message;
	int bytesSent;


	// Create the chat message.
	message.type = MSG_CHAT;
	message.idNumber = m_idNumber;
	message.sessionId = m_sessionId;
	strcpy_s(message.text, 64, inputMsg);

	// Send the message to the server.
	bytesSent = sendto(m_clientSocket, (char*)&message, sizeof(MSG_CHAT_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent != sizeof(MSG_CHAT_DATA))
	{
		return false;
	}

	return true;
}


bool Network::RequestEntityList()
{
	MSG_SIMPLE_DATA message;
	int bytesSent;

	
	// Create the entity request message.
	message.type = MSG_ENTITY_REQUEST;
	message.idNumber = m_idNumber;
	message.sessionId = m_sessionId;

	// Send the message to the server.
	bytesSent = sendto(m_clientSocket, (char*)&message, sizeof(MSG_SIMPLE_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent != sizeof(MSG_SIMPLE_DATA))
	{
		return false;
	}

	return true;
}


bool Network::SendStateChange(char state)
{
	MSG_STATE_CHANGE_DATA message;
	int bytesSent;

	
	// Create the state change message.
	message.type = MSG_STATE_CHANGE;
	message.idNumber = m_idNumber;
	message.sessionId = m_sessionId;
	message.state = state;

	bytesSent = sendto(m_clientSocket, (char*)&message, sizeof(MSG_STATE_CHANGE_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent != sizeof(MSG_STATE_CHANGE_DATA))
	{
		return false;
	}

	return true;
}

bool Network::SendPositionUpdate(float positionX, float positionY, float positionZ, float rotationX, float rotationY, float rotationZ)
{
	MSG_POSITION_DATA message;
	int bytesSent;

	
	// Create the position message.
	message.type = MSG_POSITION;
	message.idNumber = m_idNumber;
	message.sessionId = m_sessionId;
	message.positionX = positionX;
	message.positionY = positionY;
	message.positionZ = positionZ;
	message.rotationX = rotationX;
	message.rotationY = rotationY;
	message.rotationZ = rotationZ;

	// Send the position update message to the server.
	bytesSent = sendto(m_clientSocket, (char*)&message, sizeof(MSG_POSITION_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent != sizeof(MSG_POSITION_DATA))
	{
		return false;
	}

	return true;
}

bool Network::SendShootUpdate(float positionX, float positionY, float positionZ, float rotationX, float rotationY, float rotationZ,char gunId)
{
	MSG_SHOOT_DATA message;
	int bytesSent;

	
	// Create the position message.
	message.type = MSG_SHOOT;
	message.idNumber = m_idNumber;
	message.sessionId = m_sessionId;
	message.gunId = gunId;
	message.px = positionX;
	message.py = positionY;
	message.pz = positionZ;
	message.dx = rotationX;
	message.dy = rotationY;
	message.dz = rotationZ;
	// Send the position update message to the server.
	bytesSent = sendto(m_clientSocket, (char*)&message, sizeof(MSG_SHOOT_DATA), 0, (struct sockaddr*)&m_serverAddress, m_addressLength);
	if(bytesSent != sizeof(MSG_SHOOT_DATA))
	{
		return false;
	}
	return true;
}