#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

class User {
private:
    int id;
    string nama;

public:
    User(int id, const string& nama);
    virtual ~User() = default;

    int getId() const;
    const string& getNama() const;

    virtual string getStatusMember() const = 0;
    virtual bool canKelola() const = 0;

    virtual void displayInfo() const; 

};
class Admin : public User {
public:
    Admin(int id, const string& nama);

    string getStatusMember() const override;
    bool canKelola() const override;
    void displayInfo() const override;
};

#endif
