# Threaded C++ TCP Echo Server

This folder contains a small cross-platform threaded TCP echo server and a simple client to test it.

Files
- `server.cpp` — cross-platform (Windows/POSIX) TCP server. Accepts connections and handles each client in a detached `std::thread`. Echoes received data back to the client.
- `client.cpp` — simple interactive client to connect and send messages to the server.

Build & run

Windows (using MinGW/g++):

1. Ensure MinGW-w64 is installed and `g++` is on PATH.
2. From a regular `cmd` in this folder:

```bat
cd /d C:\Ilavarasan\upskilling\CPP_DSA\server
g++ -std=c++17 -O2 server.cpp -o server.exe -lws2_32
g++ -std=c++17 -O2 client.cpp -o client.exe -lws2_32
server.exe 54000
```

In a separate terminal run the client:

```bat
client.exe 127.0.0.1 54000
```

Windows (MSVC Developer Command Prompt):

```bat
cl /EHsc server.cpp ws2_32.lib
cl /EHsc client.cpp ws2_32.lib
server.exe 54000
```

Linux / WSL / macOS:

```bash
g++ -std=c++17 -O2 server.cpp -o server
g++ -std=c++17 -O2 client.cpp -o client
./server 54000
# in another shell
./client 127.0.0.1 54000
```

Notes
- Press Ctrl+C in the server terminal to stop the server.
- The server detaches client threads; for production use you may want to keep track of threads and join them on shutdown or use a thread pool.
- This code is intentionally minimal for learning and testing purposes.
