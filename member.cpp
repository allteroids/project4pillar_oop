#include "member.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "utils.h"
using namespace std;

Member::Member(int id, const string& nama)
    : User(id, nama), totaldenda(0) {} 

bool Member::canKelola() const { return false; }

long Member::getTotalDenda() const { return totaldenda; }
const vector<int>& Member::getdaftarpinjaman() const { return daftarpinjaman; }

void Member::addBarangDipinjam(int itemId) {
    // if (!canPinjam()) throw runtime_error(getNama() + " tidak diizinkan meminjam");
    daftarpinjaman.push_back(itemId);
}

void Member::removeBarangDipinjam(int itemId) {
    auto it = find(daftarpinjaman.begin(), daftarpinjaman.end(), itemId);
    if (it == daftarpinjaman.end())
        throw runtime_error(getNama() + " tidak sedang meminjam koleksi tersebut");
    daftarpinjaman.erase(it);
}

void Member::addTotalDenda(long amount) {
    if (amount < 0) throw invalid_argument("Denda tidak boleh negatif");
    totaldenda += amount;
}

void Member::BayarDenda(long amount) {
    if (amount <= 0) throw invalid_argument("Jumlah pembayaran harus positif");
    if (amount > totaldenda)
        throw invalid_argument("Pembayaran melebihi total denda (" +
                                    formatRupiah(totaldenda) + ")");
    totaldenda -= amount;
}

int Member::getFineDiscountPercent() const { return 0; }

void Member::displayInfo() const {
    User::displayInfo();
    cout << " | Pinjaman: " << getBatasPinjaman()
              << " | Denda: " << formatRupiah(totaldenda) << "\n";
}

Mahasiswa::Mahasiswa(int id, const string& nama) : Member(id, nama) {}
string Mahasiswa::getStatusMember() const { return "Mahasiswa"; }
int Mahasiswa::getBatasPinjaman() const { return 3; }

Dosen::Dosen(int id, const string& nama) : Member(id, nama) {}
string Dosen::getStatusMember() const { return "Dosen"; }
int Dosen::getBatasPinjaman() const { return 10; }
int Dosen::getFineDiscountPercent() const { return 50; }
