#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main() {                                              //обьявление переменных, Структура WSADATA содержит сведения о реализации сокетов Windows.
    WSADATA wsaData;                                      //обьявление переменных, Структура addrinfo используется функцией getaddrinfo для хранения информации об адресе хоста.
    ADDRINFO hints;                                       //обьявление переменных, Структура addrinfo используется функцией getaddrinfo для хранения информации об адресе хоста.
    ADDRINFO* addrResult;                                 //обьявление прослушиваемого сокета
    SOCKET ConnectSocket = INVALID_SOCKET;                //обьявление сокета для ссоединения
    char recvBuffer[512];                                 //обьявлениебуфера

    const char* sendBuffer1 = "Hello from client 1";              //сообщение о работе сокета-клиента 
    const char* sendBuffer2 = "Hello from client 2";              //сообщение о работе сокета-клиента 

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);                              //старт работы
    if (result != 0) {                                                              //вылавливание ошибки при старте работы
        cout << "WSAStartup failed with result: " << result << endl;                //вылавливание ошибки при старте работы
        return 1;                                                                   //вылавливание ошибки при старте работы
    }

    ZeroMemory(&hints, sizeof(hints));                                           //обнуление данных
    hints.ai_family = AF_INET;                                                   //семейство адресов AF_INET — это семейство адресов для IPv4.
    hints.ai_socktype = SOCK_STREAM;                                             //Функция сокета создает сокет, который привязан к определенному поставщику транспортных услуг.
    hints.ai_protocol = IPPROTO_TCP;                                             //таблица со значенями
    
    result = getaddrinfo("localhost", "666", &hints, &addrResult);             //получение адреса
    if (result != 0) {                                                         //вылавливание ошибки при получении адреса
        cout << "getaddrinfo failed with error: " << result << endl;           //вылавливание ошибки при получении адреса
        freeaddrinfo(addrResult);                                              //вылавливание ошибки при получении адреса
        WSACleanup();                                                          //вылавливание ошибки при получении адреса
        return 1;
    }

    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);     //определение сокета
    if (ConnectSocket == INVALID_SOCKET) {                                                               //вылавливание ошибки при определении сокета
        cout << "Socket creation failed" << endl;                                                        //вылавливание ошибки при определении сокета
        freeaddrinfo(addrResult);                                                                        //вылавливание ошибки при определении сокета
        WSACleanup();                                                                                    //вылавливание ошибки при определении сокета
        return 1;
    }

    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);     //соединение с сокетом-сервером
    if (result == SOCKET_ERROR) {                                                          //вылавливание ошибки при соединении с сокетом-сервером
        cout << "Unable to connect to server" << endl;                                     //вылавливание ошибки при соединении с сокетом-сервером
        closesocket(ConnectSocket);                                                        //вылавливание ошибки при соединении с сокетом-сервером
        ConnectSocket = INVALID_SOCKET;                                                    //вылавливание ошибки при соединении с сокетом-сервером
        freeaddrinfo(addrResult);                                                          //вылавливание ошибки при соединении с сокетом-сервером
        WSACleanup();
        return 1;
    }

    result = send(ConnectSocket, sendBuffer1, (int)strlen(sendBuffer1), 0);            //передача данных сокету-серверу
    if (result == SOCKET_ERROR) {                                                      //вылавливание ошибки при передачи данных сокету-серверу
        cout << "Send failed, error: " << result << endl;                              //вылавливание ошибки при передачи данных сокету-серверу
        closesocket(ConnectSocket);                                                    //вылавливание ошибки при передачи данных сокету-серверу
        freeaddrinfo(addrResult);                                                      //вылавливание ошибки при передачи данных сокету-серверу
        WSACleanup();                                                                  //вылавливание ошибки при передачи данных сокету-серверу
        return 1;
    }
    cout << "Sent: " << result << " bytes" << endl;                                   //определение посылаемых данных

    result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer2), 0);         //передача данных сокету-серверу
    if (result == SOCKET_ERROR) {                                                   //вылавливание ошибки при передачи данных сокету-серверу
        cout << "Send failed, error: " << result << endl;                           //вылавливание ошибки при передачи данных сокету-серверу
        closesocket(ConnectSocket);                                                 //вылавливание ошибки при передачи данных сокету-серверу
        freeaddrinfo(addrResult);                                                   //вылавливание ошибки при передачи данных сокету-серверу
        WSACleanup();                                                               //вылавливание ошибки при передачи данных сокету-серверу
        return 1;
    }
    cout << "Sent: " << result << " bytes" << endl;

    result = shutdown(ConnectSocket, SD_SEND);                                      //завершение работы
    if (result == SOCKET_ERROR) {                                                   //вылавливание ошибки при завершении работы
        cout << "Shutdown failed, error: " << result << endl;                       //вылавливание ошибки при завершении работы
        closesocket(ConnectSocket);                                                 //вылавливание ошибки при завершении работы
        freeaddrinfo(addrResult);                                                   //вылавливание ошибки при завершении работы
        WSACleanup();                                                               //вылавливание ошибки при завершении работы
        return 1;
    }

    do {
        ZeroMemory(recvBuffer, 512);                                                 //обнуление данных
        result = recv(ConnectSocket, recvBuffer, 512, 0);                           //получение данных
        if (result > 0) {
            cout << "Received " << result << " bytes" << endl;                      //вывод полученных данных
            cout << "Received data: " << recvBuffer << endl;                        //вывод полученных данных
        }
        else if (result == 0) {
            cout << "Connection closed" << endl;                                     //вывод сообщения о закрытии соединения
        }
        else {
            cout << "Recv failed, error: " << WSAGetLastError() << endl;             //вывод сообщения о ошибке соединения
        }
    } while (result > 0);

    closesocket(ConnectSocket);                                                       //закрытие соединения
    freeaddrinfo(addrResult);                                                         //очистка данных
    WSACleanup();                                                                     //очистка данных
    return 0;
}
