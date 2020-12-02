#include <iostream>
#include <vector>
#include <sstream>
#include "ECE_UDPSocket.h"

using namespace std;


// g++ server_main.cpp ECE_UDPSocket.cpp -o server.out
// g++ client_main.cpp ECE_UDPSocket.cpp -o client.out

// g++ main.cpp ECE_UDPSocket.cpp -o Lab5.out
// Lab5.out 65112
// Lab5.out localhost 65112
void terminateApplication(ECE_UDPSocket& activeSocket)
{
    activeSocket.sockClose();
}

void doServerInteractions(ECE_UDPSocket &activeSocket)
{
    bool bStop = false;
    int nCommand{ 0 };
    do
    {
        // Handle the interactions with the user
        cout << "Please enter a command: ";
        cin >> nCommand;

        switch (nCommand)
        {
        case 0: // the server immediately sends to all clients the current 
                //composite message and clears out the composite message
            activeSocket.sendCompositeMsg();
            break;
        case 1: // the server immediately clears out the composite message.
            activeSocket.clearCompositeMsg();
            break;
        case 2: // the server immediately displays to the console the 
                // composite message but takes no other actions.
            activeSocket.displayCompositeMsg();
            break;
        case 3:
            terminateApplication(activeSocket);
            bStop = true;
            break;
        }
    } while (!bStop);

    exit(0);
}

void doClientInteractions(ECE_UDPSocket& activeSocket, string ipAddress, unsigned short portNum)
{
    bool bStop = false;
    std::string inStr;
    int nCommand{ 0 };
    udpMessage outMsg;
    do
    {
        // Handle the interactions with the user
        cout << "Please enter command: ";
        std::getline(std::cin, inStr);
        // Parse the inStr
        vector<string> strings;
        istringstream f(inStr);
        string s;
        while (getline(f, s, ' ')) {
            strings.push_back(s);
        }
        // v #
        // t # # message string
        // q
        if (strings.size() == 1 && strings[0].at(0) == 'q')
        {
            terminateApplication(activeSocket);
        }
        else if (strings.size() == 2 && strings[0].at(0) == 'v')
        {
            outMsg.nVersion = atoi(strings[1].c_str());
        }
        else if (strings.size() > 3 && strings[0].at(0) == 't')
        {
            string outStr;
            outMsg.nType = atoi(strings[1].c_str());
            outMsg.lSeqNum = atoi(strings[2].c_str());
            for (int ii = 3; ii < strings.size(); ii++)
            {
                outStr += strings[ii];
                if (ii != strings.size()-1)
                    outStr += " ";
            }
            outMsg.nMsgLen = outStr.length();
            memcpy(outMsg.chMsg, outStr.c_str(), outStr.length());
            activeSocket.sendMessage(ipAddress, portNum, outMsg);
        }
    } while (!bStop);

    exit(0);

}

int main(int argc, char** argv)
{
    bool bIsServer = false;
    std::string strLine;
    unsigned short usPortNum { 0 };

    // Get the command line parameters to determine if this is server or a client
    if (argc == 2)
    {
        // This is a server
        bIsServer = true;
        usPortNum = atoi(argv[1]);
    }
    else if (argc == 3)
    {
        // This is a client
        bIsServer = false;
    }
    else
    {
        // This is a mistake
        cout << "You entered a incorrect number of command line arguments" << endl;
        cout << "For a server enter just the port number!" << endl;
        cout << "For a client enter the IP Address and the port number!" << endl;
    }

    // Setup socket
    ECE_UDPSocket udpSocket(usPortNum);

    if (bIsServer)
    {
        doServerInteractions(udpSocket);
    }
    else
    {
        doClientInteractions(udpSocket, argv[1], atoi(argv[2]));
    }


    return 0;
}