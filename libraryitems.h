#ifndef LIBRARY_ITEMS_H
#define LIBRARY_ITEMS_H
#include <string>
#include "libraryitem.h"
using namespace std;

class Book : public LibraryItem {
private:
    string penulis;
    int halaman;

public:
    Book(int id, const string& judul, const string& penulis, int halaman);

    string getTipe() const override;
    int getMasaPeminjaman() const override;
    long calculateDenda(int TotalKeterlambatan) const override;
    void displayInfo() const override;
    bool matches(const string& keyword) const override;  
};

class Magazine : public LibraryItem {
private:
    int issueNumber;

public:
    Magazine(int id, const string& judul, int issueNumber);

    string getTipe() const override;
    int getMasaPeminjaman() const override;
    long calculateDenda(int TotalKeterlambatan) const override;
    void displayInfo() const override;
};

class BoardGame : public LibraryItem {
private:
    int jumlahPemain;

public:
    BoardGame(int id, const string& judul, int jumlahPemain_);

    string getTipe() const override;
    int getMasaPeminjaman() const override;
    long calculateDenda(int TotalKeterlambatan) const override;
    void displayInfo() const override;
};

#endif
