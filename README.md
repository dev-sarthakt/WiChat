# WiChat

A simple peer-to-peer terminal chat application written in C++ using POSIX sockets.
No server. No database. Just two machines, one TCP connection, and pure vibes.


## Features

Direct P2P connection (no central server).
Multithreaded I/O
Message timestamps
Username exchange on connect
Works entirely in terminal
Lightweight & fast
Type sudo-exit to end the session cleanly


## How does it works?

One user acts as receiver (server) and listens on a port
The other user acts as sender (client) and connects via IP + port
Both exchange usernames once connected
Two threads run in parallel:
  sendMsgs() → sends user input
  recMsgs() → listens for incoming messages
Special character ~ is used internally to signal session termination


## Build

$ ./chat
