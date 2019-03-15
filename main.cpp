#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#define Log(message)    {std::cout << message << std::endl;}

class senderThread {
public:
    senderThread(const char *destIP, int destPort, long long period) : _destinationIP(destIP),
                                                                       _destinationPort(destPort), _period(period),
                                                                       _thread(&senderThread::main, this) {

    }

    void main() {

        int sock;
        struct sockaddr_in serverAddr;
        socklen_t serverAddrLen = sizeof(serverAddr);

        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(_destinationPort);

        if (inet_aton(_destinationIP.c_str(), &serverAddr.sin_addr) == -1) {
            Log("inet_iton failed");
        }

        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (sock < 0) {
            Log("Failed to create socket!");
            return;
        }

        char message[1024];
        size_t messageSize = 1024;

        memset(message, 1, messageSize);

        char ctr = 0;
        while (true) {
            auto start = std::chrono::steady_clock::now();

            message[0] = ctr;

            // send to LogStash
            ssize_t len = sendto(sock, message, messageSize, 0,
                                 (struct sockaddr *) &serverAddr, serverAddrLen);

            auto end = std::chrono::steady_clock::now();

            if (len < 0) {
                Log("Failed sendto operation!");
            } else {
                ctr += 1;
            }

            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            std::this_thread::sleep_for(std::chrono::nanoseconds(_period - elapsed));
        }
    }

    void Join() {
        _thread.join();
    }

private:
    std::thread _thread;
    int _destinationPort;
    std::string _destinationIP;
    long long _period;
};

int main() {

    const char* destination = "127.0.0.1";
    const long long period = 10000;

    senderThread st1(destination, 9001, period);
    senderThread st2(destination, 9002, period);
    senderThread st3(destination, 9003, period);
    senderThread st4(destination, 9004, period);
    senderThread st5(destination, 9005, period);
    senderThread st6(destination, 9006, period);
    senderThread st7(destination, 9007, period);
    senderThread st8(destination, 9008, period);
    senderThread st9(destination, 9009, period);
    senderThread st10(destination, 9010, period);

    st1.Join();
    st2.Join();
    st3.Join();
    st4.Join();
    st5.Join();
    st6.Join();
    st7.Join();
    st8.Join();
    st9.Join();
    st10.Join();

    return 0;
}