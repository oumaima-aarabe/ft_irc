# IRCSERV
This project is intended for a deeper study of the operation of sockets and their interactions on the example of an IRC chat/server.<br>
The project is built via Makefile and is run using the following command:<br>
./ircserv [port] [pass] <br>
To connect clients to this IRC server you can use Limechat client or necat (nc [hostname] [port]) 

## IRC Description
IRC (Internet Relay Chat) - an application-level Protocol for exchanging messages in real-time.<br>
Designed primarily for group communication in channels, it also allows you to communicate via private messages and exchange data,<br> including files, we specifically used direct client to client file transfer (DCC). <br>
IRC uses the TCP transport protocol. <br>

## IRC network
According to the protocol specifications, an IRC network is a group of servers connected to each other.<br> The simplest network is a single server. <br> In this project we used server to multiple clients connection.

##  RFC documentation
RFC 1459
