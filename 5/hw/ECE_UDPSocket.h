/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov 12, 2020
Description:
Definition of ECE_UDPSocket
*/


#pragma once

#include <thread>
#include <list>
#include <string>
#include <cstring>
#include <mutex>

#ifdef _WIN32
    /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
    #endif
    #include <winsock2.h>
    #include <Ws2tcpip.h>

    #pragma comment (lib, "Ws2_32.lib")
#else
    /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
    #include <unistd.h> /* Needed for close() */

    typedef int SOCKET;
#endif



struct udpMessage
{
    unsigned short nVersion;  // htons()
    unsigned short nType;     // htons()
    unsigned short nMsgLen;   // htons()
    unsigned long lSeqNum;    // htonl()
    char chMsg[1000];
};


class ECE_UDPSocket
{
public:
    ECE_UDPSocket() = delete;
    ~ECE_UDPSocket();
    ECE_UDPSocket(unsigned short usPort);

    bool getNextMessage(udpMessage& msg);

    void sendMessage(const std::string& strTo, unsigned short usPortNum, const udpMessage& msg);
    void clearCompositeMsg();
    void displayCompositeMsg();
    // Other stuff
    void sendCompositeMsg();
    void processMessage(const udpMessage &inMsg); // Add msg to the list
    void addSource(const sockaddr_in &from); // Add source to the list
    void restartCompositeMsg(udpMessage& Msg);

    int sockInit(void);
    int sockQuit(void);
    int sockClose();
    void error(const char* msg);


    int m_sockfd;
private:
    unsigned short m_usPort;
    std::thread m_recvThread;
    std::list<udpMessage> m_lstMsgs;
    std::list<sockaddr_in> m_lstSources;
    std::mutex m_mutex;

};

