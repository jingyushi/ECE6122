/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov 12, 2020
Description:
Implementation of ECE_UDPSocket
*/
#include "ECE_UDPSocket.h"
#include <iostream>
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
                switch (inMsg.nType)
                {
                case 0:
                    pUDpSocket->clearCompositeMsg();
                    break;
                case 1:
                    pUDpSocket->restartCompositeMsg(inMsg);
                    break;
                case 2:
                    pUDpSocket->processMessage(inMsg);
                    pUDpSocket->addSource(from);
                    break;
                case 3:
                    pUDpSocket->sendCompositeMsg();
                    pUDpSocket->clearCompositeMsg();
                    break;
                case 4:
                    std::cout << "Receiving a composite message" << std::endl;
                    break;
                default:
                    break;
                }
                std::cout << std::endl << "Received Msg Type: " << inMsg.nType << ", Seq: " << inMsg.lSeqNum << ", Msg: " << inMsg.chMsg << std::endl;
                std::cout << "Please enter a command: " << std::flush;
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
    {
        sockQuit();
        sockClose();
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ECE_UDPSocket::processMessage(const udpMessage& inMsg)
{
    // Determine what to do with the message
    
    // If we need to add the message to the list then
    // Iterate through list and insert msg based on seq num
    
    m_mutex.lock();
    bool bInsert = false;
    if (m_lstMsgs.empty())
    {
        m_lstMsgs.push_back(inMsg);
    }
    else
    {
        std::list<udpMessage>::iterator it;
        for (it = m_lstMsgs.begin(); it!= m_lstMsgs.end(); it++)
        {
            if (it->lSeqNum > inMsg.lSeqNum)
            {
                m_lstMsgs.insert(it , inMsg);
                bInsert = true;
                break;
            }
        }
        if(!bInsert) m_lstMsgs.push_back(inMsg);
    }
    m_mutex.unlock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ECE_UDPSocket::addSource(const sockaddr_in& from)
{
    m_mutex.lock();
    bool bIsPresent = false;
    // Iterate through list check is already present
    std::list<sockaddr_in>::iterator it;
    for (it = m_lstSources.begin(); it != m_lstSources.end(); it++)
    {
        if (it->sin_addr.s_addr == from.sin_addr.s_addr)
        {
            bIsPresent = true;
            break;
        }
    }
    if (!bIsPresent)
    {
        m_lstSources.push_back(from);
//         add source to list
    }
    m_mutex.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ECE_UDPSocket::getNextMessage(udpMessage& msg)
{ 
    // Get next message from list
    // return false if there no more messages
    if (m_lstMsgs.empty()) return false;
    m_mutex.lock();
    msg = m_lstMsgs.front();
    m_lstMsgs.pop_front();
    m_mutex.unlock();
    return true; 
};

void ECE_UDPSocket::clearCompositeMsg()
{
    m_mutex.lock();
    m_lstMsgs.clear();
    m_mutex.unlock();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ECE_UDPSocket::displayCompositeMsg()
{
    // Iterate through m_lstMsgs and generate the composite message
    std::list<udpMessage>::iterator it;
    std::cout << "Composite Msg: ";
    for (it = m_lstMsgs.begin(); it != m_lstMsgs.end(); it++)
    {
        std::cout << it->chMsg;
    }
    std::cout << std::endl;
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
    //std::cout << msg.chMsg << std::endl;
    int n = sendto(m_sockfd, (char*)&msg, sizeof(udpMessage), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (n < 0)
        error("ERROR writing to socket");

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ECE_UDPSocket::restartCompositeMsg(udpMessage& Msg)
{
    clearCompositeMsg();
    m_lstMsgs.push_back(Msg);
}



void ECE_UDPSocket::sendCompositeMsg()
{
    udpMessage compositeMsg, tmpMessage;
    compositeMsg.nType = 4;
    compositeMsg.nVersion = 1;
    compositeMsg.nMsgLen = 0;
    while (getNextMessage(tmpMessage))
    {
        // Combine them
        if (compositeMsg.nMsgLen + tmpMessage.nMsgLen > 1000)
        {
            restartCompositeMsg(tmpMessage);
            break;
        }
        strcat(compositeMsg.chMsg,tmpMessage.chMsg);
        compositeMsg.nMsgLen += tmpMessage.nMsgLen;
    }
    compositeMsg.lSeqNum = 0;
    // Just for the server side 
    // Loop through m_lstSources
    // sendMessage(addrTo, compositeMsg);
    std::list<sockaddr_in>::iterator it;
    for (it = m_lstSources.begin(); it != m_lstSources.end(); it++)
    {
        //std::string addressString(inet_ntoa(it->sin_addr));
        //sendMessage(addressString, it->sin_port, compositeMsg);
        int n = sendto(m_sockfd, (char*)&compositeMsg, sizeof(udpMessage), 0, (struct sockaddr*) &*it, sizeof(*it));

        if (n < 0)
            error("ERROR writing to socket");
    }
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
    m_recvThread.detach();
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ECE_UDPSocket::error(const char* msg)
{
    perror(msg);
    if (m_sockfd > 0)
    {
        sockQuit();
        sockClose();
    }
    // Make sure any open sockets are closed before calling exit
    exit(1);
}