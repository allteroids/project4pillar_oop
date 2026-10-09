#ifndef MEMBER_H
#define MEMBER_H
#include "user.h"
#include <string>
#include <vector>
using namespace std;

class Member : public User {
private:
    vector<int> daftarpinjaman;
    long totaldenda;
public: 
    Member(int id, const string& nama);
    virtual ~Member() = default;

    bool canKelola() const override final;

    long getTotalDenda() const;
    const vector<int>& getdaftarpinjaman() const;

    // bool canPinjam() const;   
    void addBarangDipinjam(int itemId_);
    void removeBarangDipinjam(int itemId_);
    void addTotalDenda(long amount);
    void BayarDenda(long amount); 

    virtual int getBatasPinjaman() const = 0;
    virtual int getFineDiscountPercent() const;
    void displayInfo() const override;
};


class Mahasiswa : public Member {
public:
    Mahasiswa(int id, const string& name);
    string getStatusMember() const override;
    int getBatasPinjaman() const override;
};
class Dosen : public Member {
public:
    Dosen(int id, const string& name);
    string getStatusMember() const override;
    int getBatasPinjaman() const override;
    int getFineDiscountPercent() const override;
};

#endif
