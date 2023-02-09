# UDP_Client_Server

## Server(UDPServer)

UDP Server 監聽 port 8888，收到訊息就回傳一樣的訊息給 Client 端

## Client(UDPClient)

- 可以發送訊息到 Server
- 當 Server 沒有回應時，可以重新傳送最多10次，間隔時間遵照指數退幣演算法

## Usage

### Server

./main

### Client

./main <ip> <port> <message>

ex : ./main 127.0.0.1 8888 "Hello Server”

## Demo

[UDP_Client_Server_Demo.mkv](./UDP_Client_Server_Demo.mkv)