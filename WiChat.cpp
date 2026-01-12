#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <limits>
#include <future>
#include <chrono>
#include <iomanip>
using namespace std;

string you;
char usr[16];

int sendReq(string& you, string& ip, int port){
    int csoc = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr); //loopback : "127.0.0.1"

    cout << "< sending req to " << ip << " on port " << port << " >" << endl;

    if (connect(csoc, (sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "< error connecting >" << endl;;
        exit(-1);
    }
    else{
        cout << "< connected >" << endl;
    }

    write(csoc, you.c_str(), you.length());
    read(csoc, usr, sizeof(usr[0])*16);
    return csoc;
}

int recReq(int port){
    int ssoc = socket(AF_INET, SOCK_STREAM, 0);
    if (ssoc < 0)
    {
        cerr << "< error creating socket >" << endl;
        exit(-1);
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

     if (bind(ssoc, (sockaddr *) &addr, sizeof(addr)) < 0)
    {
        cerr << "error binding socket" << endl;
        exit(-1);
    }
    else{
        cout << "< listening on port " << port << " >" << endl;
    }

    listen(ssoc, 1);
    int csoc = accept(ssoc, nullptr, nullptr);
    cout << "< connected >" << endl;

    read(csoc, usr, sizeof(usr[0])*16);
    write(csoc, you.c_str(), you.length());

    return csoc;
}

void sendMsgs(int csoc){
    string msg;
    while (true)
    {
        getline(cin, msg);
        printf("\033[A\r");
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm *tm = std::localtime(&t);
        cout << "[ " << "You" << " ]" << " ( " << std::put_time(tm, "%H:%M:%S") << " )" << " => " << msg << endl;
        if (msg == "sudo-exit")
        {
            msg.clear();
            msg = "~";
            write(csoc, msg.c_str(), strlen(msg.c_str()));
            close(csoc);
            break;
        }
        else{
            write(csoc, msg.c_str(), strlen(msg.c_str()));
        }
    }
    return ;
}

void recMsgs(int csoc){
    int i;
    char msg[1024];
    while (true)
    {
        i = read(csoc, msg, sizeof(msg[0])*1024);
        if (i <= 0)
        {
            cout << "< session ended >" << endl;
            close(csoc);
            return ;
        }
        for (int j = 0; j < i; j++)
        {
            if (msg[j] == '~')
            {
                cout << "< session ended >" << endl;
                close(csoc);
                return ;
            }
        }
        msg[i] = '\0';
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm *tm = std::localtime(&t);
        cout << "[ " << usr << " ]" << " ( " << std::put_time(tm, "%H:%M:%S") << " )" << " => " << msg << endl;
        memset(msg, 0, sizeof(msg[0])*1024);
    }
    return ;
}

int main() {
    int menu;
    cout << "< username > : ";
    getline(cin, you);
    start:
    cout << "< [0] send req / [1] recieve req > : ";
    cin >> menu;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (menu == 0)
    {
        int port;
        string ip;
        cout << "< enter reciever's ip > : ";
        getline(cin, ip);
        cout << "< enter port no > : ";
        cin >> port;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int csoc = sendReq(you, ip, port);

        thread sender(sendMsgs, csoc);
        thread reciever(recMsgs, csoc);
        
        sender.join();
        reciever.join();

    }
    else if (menu == 1)
    {
        int port;
        cout << "< enter port no > : ";
        cin >> port;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int csoc = recReq(port);

        thread sender(sendMsgs, csoc);
        thread reciever(recMsgs, csoc);
        
        sender.join();
        reciever.join();
    }
    else{
        goto start;
    }
    return 0;
}