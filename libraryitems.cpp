#include "libraryitems.h"

#include <iostream>
#include <stdexcept>

#include "utils.h"
using namespace std;

Book::Book(int id, const string& judul, const string& penulis_, int halaman_)
    : LibraryItem(id, judul), penulis(penulis_), halaman(halaman_) {
    if (halaman_ <= 0) throw invalid_argument("Jumlah halaman harus lebih dari 0");
}

string Book::getTipe() const { return "Buku"; }
int Book::getMasaPeminjaman() const { return 14; }
long Book::calculateDenda(int TotalKeterlambatan) const { return 1000L * TotalKeterlambatan; }

void Book::displayInfo() const {
    LibraryItem::displayInfo();
    cout << " | Penulis: " << penulis << " | " << halaman << " hlm\n";
}

bool Book::matches(const string& keyword) const {
    return LibraryItem::matches(keyword) || containsHiraukanKapital(penulis, keyword);
}

Magazine::Magazine(int id, const string& judul, int issueNumber_)
    : LibraryItem(id, judul), issueNumber(issueNumber_) {
    if (issueNumber_ <= 0) throw invalid_argument("Nomor edisi harus lebih dari 0");
}

string Magazine::getTipe() const { return "Majalah"; }
int Magazine::getMasaPeminjaman() const { return 7; }
long Magazine::calculateDenda(int TotalKeterlambatan) const { return 2000L * TotalKeterlambatan; }

void Magazine::displayInfo() const {
    LibraryItem::displayInfo();
    cout << " | Edisi #" << issueNumber << "\n";
}

BoardGame::BoardGame(int id, const string& judul, int jumlahPemain_)
    : LibraryItem(id, judul), jumlahPemain(jumlahPemain_) {
    if (jumlahPemain_ <= 0) throw invalid_argument("Durasi harus lebih dari 0");
}

string BoardGame::getTipe() const { return "BoardGame"; }
int BoardGame::getMasaPeminjaman() const { return 3; }
long BoardGame::calculateDenda(int TotalKeterlambatan) const { return 5000L * TotalKeterlambatan; }

void BoardGame::displayInfo() const {
    LibraryItem::displayInfo();
    cout << " | Jumlah Pemain: " << jumlahPemain << " orang\n";
}
