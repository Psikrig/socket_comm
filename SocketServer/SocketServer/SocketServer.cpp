#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main() {
    WSADATA wsaData;                                    //обьявление переменных, Структура WSADATA содержит сведения о реализации сокетов Windows.
    ADDRINFO hints;                                     //обьявление переменных, Структура addrinfo используется функцией getaddrinfo для хранения информации об адресе хоста.
    ADDRINFO* addrResult;                               //обьявление переменных, Структура addrinfo используется функцией getaddrinfo для хранения информации об адресе хоста.
    SOCKET ListenSocket = INVALID_SOCKET;               //обьявление прослушиваемого сокета
    SOCKET ConnectSocket = INVALID_SOCKET;              //обьявление сокета для ссоединения
    char recvBuffer[512];                               //обьявлениебуфера

    const char* sendBuffer = "Hello from server";       //сообщение для поддтверждения работы сервера

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);                  //старт работы, Создает значение WORD путем объединения указанных значений.     
    if (result != 0) {                                                  //вылавливание ошибок при старте работы     
        cout << "WSAStartup failed with result: " << result << endl;    //вылавливание ошибок при старте работы     
        return 1;                                                       //вылавливание ошибок при старте работы     
    }

    ZeroMemory(&hints, sizeof(hints));                            //обнуление данных
    hints.ai_family = AF_INET;                                    //семейство адресов AF_INET — это семейство адресов для IPv4.
    hints.ai_socktype = SOCK_STREAM;                              //Функция сокета создает сокет, который привязан к определенному поставщику транспортных услуг.
    hints.ai_protocol = IPPROTO_TCP;                              //таблица со значенями
    hints.ai_flags = AI_PASSIVE;                                  //обьявление переменных

    result = getaddrinfo(NULL, "666", &hints, &addrResult);                   //получение адреса
    if (result != 0) {                                                        //вылавливание ошибок при получении адреса
        cout << "getaddrinfo failed with error: " << result << endl;          //вылавливание ошибок при получении адреса
        freeaddrinfo(addrResult);                                             //вылавливание ошибок при получении адреса
        WSACleanup();                                                         //вылавливание ошибок при получении адреса
        return 1;                                                             //вылавливание ошибок при получении адреса
    }

    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);     //определение сокета для прослушки
    if (ListenSocket == INVALID_SOCKET) {                                                               //вылавливание ошибок при определении сокета
        cout << "Socket creation failed" << endl;                                                       //вылавливание ошибок при определении сокета
        freeaddrinfo(addrResult);                                                                       //вылавливание ошибок при определении сокета
        WSACleanup();                                                                                   //вылавливание ошибок при определении сокета
        return 1;                                                                                       //вылавливание ошибок при определении сокета
    }

    result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);      //биндинг сокета
    if (result == SOCKET_ERROR) {                                                       //вылавливание ошибок при старте сервера
        cout << "Bind failed, error: " << result << endl;                               //вылавливание ошибок при старте сервера
        closesocket(ListenSocket);                                                      //вылавливание ошибок при старте сервера
        freeaddrinfo(addrResult);                                                       //вылавливание ошибок при старте сервера
        WSACleanup();                                                                   //вылавливание ошибок при старте сервера
        return 1;
    }

    result = listen(ListenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {                                       //прослушка сокета
        cout << "Listen failed, error: " << result << endl;             //вылавливание ошибок при прослушке сокета
        closesocket(ListenSocket);                                      //вылавливание ошибок при прослушке сокета
        freeaddrinfo(addrResult);                                       //вылавливание ошибок при прослушке сокета
        WSACleanup();                                                   //вылавливание ошибок при прослушке сокета
        return 1;                                                       //вылавливание ошибок при прослушке сокета
    }

    ConnectSocket = accept(ListenSocket, NULL, NULL);                       //соединение с прослушиваемым сокетом
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Accept failed, error: " << WSAGetLastError() << endl;      //вылавливание ошибок при соединении с прослушиваемым сокетом
        closesocket(ListenSocket);                                          //вылавливание ошибок при соединении с прослушиваемым сокетом
        freeaddrinfo(addrResult);                                           //вылавливание ошибок при соединении с прослушиваемым сокетом
        WSACleanup();                                                       //вылавливание ошибок при соединении с прослушиваемым сокетом
        return 1;                                                           //вылавливание ошибок при соединении с прослушиваемым сокетом
    }                                                                      

    closesocket(ListenSocket);                                               //закрытие прослушиваемого сокета

    do {                                                                           //получение данных от сокета-клиента
        ZeroMemory(recvBuffer, 512);                                               //получение данных от сокета-клиента
        result = recv(ConnectSocket, recvBuffer, 512, 0);                          //получение данных от сокета-клиента
        if (result > 0) {                                                          //получение данных от сокета-клиента
            cout << "Received " << result << " bytes" << endl;                     //получение данных от сокета-клиента
            cout << "Received data: " << recvBuffer << endl;                       //получение данных от сокета-клиента

            result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);       //посыл данных сокету-клиенту
            if (result == SOCKET_ERROR) {                                               //вылавливание ошибки при передаче данных
                cout << "Send failed, error: " << result << endl;                       //вылавливание ошибки при передаче данных
                closesocket(ConnectSocket);                                             //вылавливание ошибки при передаче данных
                freeaddrinfo(addrResult);                                               //вылавливание ошибки при передаче данных
                WSACleanup();                                                           //вылавливание ошибки при передаче данных
                return 1;
            }
        }
        else if (result == 0) {                                                      //вывод сообщения о закрытии соединения
            cout << "Connection closing" << endl;                                    //вывод сообщения о закрытии соединения
        }                                                                            //вывод сообщения о закрытии соединения
        else {
            cout << "Recv failed, error: " << WSAGetLastError() << endl;             //вывод сообщения об ошибке
            closesocket(ConnectSocket);                                              //закрытие соединения
            freeaddrinfo(addrResult);                                                
            WSACleanup();                                                            //очистка данных
            return 1;                                                                
        }
    } while (result > 0);

    result = shutdown(ConnectSocket, SD_SEND);                               //вывод сообщения об ошибке закрытия соединения, очистка данных
    if (result == SOCKET_ERROR) {                                            //вывод сообщения об ошибке закрытия соединения, очистка данных
        cout << "Shutdown failed, error: " << result << endl;                //вывод сообщения об ошибке закрытия соединения, очистка данных
        closesocket(ConnectSocket);                                          //вывод сообщения об ошибке закрытия соединения, очистка данных
        freeaddrinfo(addrResult);                                            //вывод сообщения об ошибке закрытия соединения, очистка данных
        WSACleanup();                                                        //вывод сообщения об ошибке закрытия соединения, очистка данных
        return 1;
    }

    closesocket(ConnectSocket);                                      //закрытие соединения, очистка данных
    freeaddrinfo(addrResult);                                        //закрытие соединения, очистка данных
    WSACleanup();                                                    //закрытие соединения, очистка данных
    return 0;                                                        //закрытие соединения, очистка данных
}
