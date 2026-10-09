#include "libraryitem.h"

#include <iostream>
#include <stdexcept>

#include "utils.h"
using namespace std;

LibraryItem::LibraryItem(int id_, const string& judul_)
    : id(id_), judul(judul_), tersedia(true) {
    if (id_ <= 0) throw invalid_argument("ID koleksi harus lebih dari 0");
    if (judul.empty()) throw invalid_argument("Judul koleksi tidak boleh kosong");
}

int LibraryItem::getId() const { return id; }
const string& LibraryItem::getJudul() const { return judul; }
bool LibraryItem::isTersedia() const { return tersedia; }

void LibraryItem::tandaiTerpinjam() {
    if (!tersedia) throw runtime_error("'" + judul + "' sedang dipinjam");
    tersedia = false;
}

void LibraryItem::tandaiDikembalikan() {
    if (tersedia) throw runtime_error("'" + judul + "' tidak sedang dipinjam");
    tersedia = true;
}

void LibraryItem::displayInfo() const {
    cout << "[" << id << "] " << getTipe() << " | " << judul << " | " << (tersedia ? "Tersedia" : "Dipinjam");
}

bool LibraryItem::matches(const string& keyword) const {
    return containsHiraukanKapital(judul, keyword);
}
