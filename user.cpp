#include "user.h"

#include <iostream>
#include <stdexcept>
using namespace std;

User::User(int id_, const string& nama_) : id(id_), nama(nama_) {
    if (id_ <= 0) throw invalid_argument("ID pengguna harus lebih dari 0");
    if (nama_.empty()) throw invalid_argument("Nama pengguna tidak boleh kosong");
}

int User::getId() const { return id; }
const string& User::getNama() const { return nama; }

void User::displayInfo() const {
    cout << "[" << id << "] " << getStatusMember() << " | " << nama;
}

Admin::Admin(int id_, const string& nama_) : User(id_, nama_) {}

string Admin::getStatusMember() const { return "Admin"; }
bool Admin::canKelola() const { return true; }

void Admin::displayInfo() const {
    User::displayInfo();
    cout << " | Hak akses: kelola koleksi & pengguna\n";
}
