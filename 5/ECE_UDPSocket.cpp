#include "ECE_UDPSocket.h"

// Function called by thread

void receiveSocketMsgs(ECE_UDPSocket* pUDpSocket)
{
    // Loop that waits on incoming messages
    udpMessage inMsg;

    sockaddr_in from;
    socklen_t fromlen{ sizeof(struct sockaddr_in) };
    int n;

    do
    {
        n = recvfrom(pUDpSocket->m_sockfd, (char*)&inMsg, sizeof(udpMessage), 0, (struct sockaddr*)&from, &fromlen);
        if (n < 0)
        {
            break;
        }
        else
        {
            if (inMsg.nVersion == 1)
            {
                pUDpSocket->processMessage(inMsg);
                pUDpSocket->addSource(from);
            }
        }
    } while (true);
}

ECE_UDPSocket::ECE_UDPSocket(unsigned short usPort) :m_usPort(usPort) 
{
    sockaddr_in serv_addr;
    sockInit();
    // Create the socket
    m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // Make sure the socket was created
    if (m_sockfd < 0)
        error("ERROR opening socket");
    // Zero out the variable serv_addr
    memset((char*)&serv_addr, sizeof(serv_addr), 0);

    // Initialize the serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // Convert port number from host to network
    serv_addr.sin_port = htons(usPort);
    // Bind the socket to the port number
    if (bind(m_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    // Start thread that waits for messages 
    m_recvThread = std::thread(receiveSocketMsgs, this);
};

ECE_UDPSocket::~ECE_UDPSocket()
{
    if (m_sockfd > 0)
        sockClose();

}


void ECE_UDPSocket::processMessage(const udpMessage& inMsg)
{
    // Determine what to do with the message

    // If we need to add the message to the list then
    // Iterate through list and insert msg based on seq num
}

void ECE_UDPSocket::addSource(const sockaddr_in& from)
{
    bool bIsPresent = false;
    // Iterate through list check is already present

    if (!bIsPresent)
    {
//         add source to list
    }
}

bool ECE_UDPSocket::getNextMessage(udpMessage& msg)
{ 
    // Get next message from list

    // return false if there no more messages


    return true; 
};

void ECE_UDPSocket::clearCompositeMsg()
{
    m_mutex.lock();
    m_lstMsgs.clear();
    m_mutex.unlock();

}
void ECE_UDPSocket::displayCompositeMsg()
{
    // Iterate through m_lstMsgs and generate the composite message

}



void ECE_UDPSocket::sendMessage(const std::string& strTo, unsigned short usPortNum, const udpMessage& msg) 
{
    struct hostent* server;
    struct sockaddr_in serv_addr;
    socklen_t fromlen;
    struct sockaddr_in from;

    server = gethostbyname(strTo.c_str());

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    // Zero out serv_addr variable
    memset((char*)&serv_addr, sizeof(serv_addr), 0);

    serv_addr.sin_family = AF_INET;

    memmove((char*)&serv_addr.sin_addr.s_addr, (char*)server->h_addr, server->h_length);

    serv_addr.sin_port = htons(usPortNum);

    fromlen = sizeof(from);

    int n = sendto(m_sockfd, (char*)&msg, sizeof(udpMessage), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (n < 0)
        error("ERROR writing to socket");

};

void ECE_UDPSocket::sendCompositeMsg()
{
    udpMessage compositeMsg, tmpMessage;
    while (getNextMessage(tmpMessage))
    {
        // Combine them
    }

    // Just for the server side 
    // Loop through m_lstSources
    // sendMessage(addrTo, compositeMsg);
};

/////////////////////////////////////////////////
// Cross-platform socket initialize
int ECE_UDPSocket::sockInit(void)
{
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
    return 0;
#endif
}
/////////////////////////////////////////////////
// Cross-platform socket quit
int ECE_UDPSocket::sockQuit(void)
{
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif
}
/////////////////////////////////////////////////
// Cross-platform socket close
int ECE_UDPSocket::sockClose()
{

    int status = 0;

#ifdef _WIN32
    status = shutdown(m_sockfd, SD_BOTH);
    if (status == 0)
    {
        status = closesocket(m_sockfd);
    }
#else
    status = shutdown(m_sockfd, SHUT_RDWR);
    if (status == 0)
    {
        status = close(m_sockfd);
    }
#endif

    // Set to show socket closed
    m_sockfd = -1;
    // Wait for thread to shut down.
    return status;

}
/////////////////////////////////////////////////
// Output error message and exit
void ECE_UDPSocket::error(const char* msg)
{
    perror(msg);
    // Make sure any open sockets are closed before calling exit
    exit(1);
}