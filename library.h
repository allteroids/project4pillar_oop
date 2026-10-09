#ifndef LIBRARY_H
#define LIBRARY_H

#include <memory>
#include <string>
#include <vector>

#include "libraryitem.h"
#include "loan.h"
#include "member.h"
#include "user.h"
using namespace std;

class Library {
private:
    vector<unique_ptr<LibraryItem>> items;
    vector<unique_ptr<User>> users;
    vector<Loan> loans;
    int today;

    LibraryItem* findItem(int itemId) const;
    User* findUser(int userId) const;
    Member* findMember(int memberId) const;
    Loan* findActiveLoan(int itemId);

    User& requireIzin(int actorId, const string& action) const;
    Member& requireMember(int userId) const;
public:
    explicit Library(unique_ptr<Admin> firstAdmin);

    void addItem(int actorId, unique_ptr<LibraryItem> item);
    void removeItem(int actorId, int itemId);
    void addUser(int actorId, unique_ptr<User> user);
    void removeUser(int actorId, int userId);
    void displayAllUsers(int actorId) const;
    void displayActiveLoans(int actorId) const;

    void pinjamItem(int memberId, int itemId);
    void KembalikanItem(int memberId, int itemId);
    void BayarDenda(int memberId, long amount);
    void displayDendaku(int memberId) const;

    vector<const LibraryItem*> search(const string& keyword) const;
    const LibraryItem* search(int itemId) const;
    void displayAllItems() const;
    void advanceDays(int days);
    int getToday() const;

    const User* getUser(int userId) const;
    int nextItemId() const;
    int nextUserId() const;
};

#endif
