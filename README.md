# WiChat

A simple peer-to-peer terminal chat application written in C++ using POSIX sockets.
No server. No database. Just two machines, one TCP connection, and pure vibes.


## Features

Direct P2P connection (no central server)

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

-> sendMsgs() → sends user input

-> recMsgs() → listens for incoming messages
  
Special character ~ is used internally to signal session termination


## Build

```bash
g++ WiChat.cpp -o Wichat
```


## To use


### Step 1 : Start Receiver (Server)

On Machine A:

```bash
./WiChat
```

Enter your username

Choose option (receive req)

Enter a port (example: 8080)

You’ll see:

```bash
< listening on port 8080 >
```


### Step 2 : Start Sender (Client)

On Machine B:

```bash
./WiChat
```

Enter your username

Choose option (send req)

Enter receiver’s IP (example: 127.0.0.1 (loopback) or LAN IP)

Enter the same port (8080)

You’ll see:

```bash
< connected >
```


## Chat Format

```bash
[ You ] ( 21:34:12 ) => hello bro
[ Sarthakt ] ( 21:34:15 ) => what's good
```


## Exit

```bash
sudo-exit
```


## Limitations

No encryption (plaintext TCP)

Single connection only

Fixed username buffer size (16 bytes)

No message framing (relies on TCP stream behavior)

No Windows support (POSIX only)
