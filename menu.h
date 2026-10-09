#ifndef MENU_H
#define MENU_H

#include <functional>
#include <string>

#include "library.h"
#include "user.h"
using namespace std;

class ConsoleApp {
private:
    Library& library;
    bool inputClosed;

    string readLine(const string& prompt);
    int readInt(const string& prompt);    
    long readLong(const string& prompt);  

    void guarded(const function<void()>& action);

    void adminMenu(const User& admin);
    void memberMenu(const User& member);
    void addItemFlow(int adminId);
    void addUserFlow(int adminId);

public:
    explicit ConsoleApp(Library& library);
    void run();
};

#endif
