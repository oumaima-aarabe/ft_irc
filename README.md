
###### IRC_SERVER #############################################################
This project is intended for a deeper study of the operation of sockets and their interactions on the example of an IRC chat/server.
The project is built via Makefile and is run using the following command:
./ircserv <port> <pass>
To connect clients to this IRC server, you can use Limechat client or nc (netcat) as follows:
nc <hostname> <port>
NB : you can use Limechat 

###### IRC_Description ###################################################
IRC (Internet Relay Chat) - an application-level Protocol for exchanging messages in real-time.
Designed primarily for group communication in channels, it also allows you to communicate via private messages and exchange data, including files. We specifically used direct client-to-client file transfer (DCC).
IRC uses the TCP transport protocol.

###### IRC_Network #######################################################
According to the protocol specifications, an IRC network is a group of servers connected to each other. The simplest network is a single server. In this project, we used a server-to-multiple-clients connection.

###### RFC Documentation #################################################
RFC 1459

###### Setting Up and Running the IRC Server #############################

- Prerequisites
Make sure you have the following installed on your system:
C++ compiler
Make utility

- Compilation
The IRC server is built using a Makefile. 
Run the following command to compile the project:
make

- Running the Server
Once compiled, you can run the IRC server with the following command:
./ircserv <port> <pass>
<port>: The port number on which the server will listen for incoming connections.
<pass>: An optional password for server authentication.

###### Server Implementation Details ######################################
The server is implemented in C++ and consists of the following key steps:

- Socket Creation:
The server creates a socket using the socket system call, specifying the IPv4 address family and TCP socket type.

- Socket Options:
Socket options are set using setsockopt to enable reusing the address (SO_REUSEADDR) and set the socket to non-blocking mode.

- Binding:
The socket is bound to a specific IP address and port using the bind system call.

- Listening:
The server starts listening for incoming connections using the listen system call.

- Polling for Connections:
The server uses the poll system call to wait for incoming connections or data on existing connections.

- Handling Connections:
The server distinguishes between server and client connections and handles them accordingly.
For a new connections, the is_server_connection function is called.
For client already connexted, the is_client_connection function is called.

