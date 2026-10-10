#include "menu.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "libraryitems.h"
#include "member.h"
using namespace std;
ConsoleApp::ConsoleApp(Library& library) : library(library), inputClosed(false) {}

string ConsoleApp::readLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        inputClosed = true;
        return "";
    }
    return line;
}

int ConsoleApp::readInt(const string& prompt) {
    string text = readLine(prompt);
    try {
        size_t pos = 0;
        int value = stoi(text, &pos);
        if (text.find_first_not_of(" \t\r", pos) != string::npos) return -1;
        return value;
    } catch (const exception&) {
        return -1;
    }
}

long ConsoleApp::readLong(const string& prompt) {
    string text = readLine(prompt);
    try {
        size_t pos = 0;
        long value = stol(text, &pos);
        if (text.find_first_not_of(" \t\r", pos) != string::npos) return -1;
        return value;
    } catch (const exception&) {
        return -1;
    }
}

void ConsoleApp::guarded(const function<void()>& action) {
    try {
        action();
    } catch (const exception& e) {
        cout << "[DITOLAK] " << e.what() << "\n";
    }
}

void ConsoleApp::run() {
    while (!inputClosed) {
        int id = readInt("\nLogin - masukkan ID akun (0 = kembali): ");
        if (inputClosed || id == 0) break;

        const User* user = library.getUser(id);
        if (!user) {
            cout << "ID tidak ditemukan.\n";
            continue;
        }
        cout << "Selamat datang, " << user->getNama() << " (" << user->getStatusMember() << ")\n";

        if (user->canKelola())
            adminMenu(*user);
        else
            memberMenu(*user);
    }
}

void ConsoleApp::adminMenu(const User& admin) {
    const int id = admin.getId();
    while (!inputClosed) {
        cout << "\nAkun: ";
        admin.displayInfo();
        cout << "--- MENU ADMIN ---\n"
                  << " 1. Lihat Katalog Koleksi\n"
                  << " 2. Tambah Koleksi\n"
                  << " 3. Hapus Koleksi\n"
                  << " 4. Lihat Semua Pengguna\n"
                  << " 5. Tambah Pengguna\n"
                  << " 6. Hapus Pengguna\n"
                  << " 7. Lihat Semua Peminjaman Aktif\n"
                  << " 8. Majukan Hari (Simulasi)\n"
                  << " 0. Logout\n";
        int choice = readInt("Pilih: ");
        if (inputClosed || choice == 0) break;

        guarded([&] {
            switch (choice) {
                case 1: library.displayAllItems(); break;
                case 2: addItemFlow(id); break;
                case 3: library.removeItem(id, readInt("ID Koleksi yang Dihapus: ")); break;
                case 4: library.displayAllUsers(id); break;
                case 5: addUserFlow(id); break;
                case 6: library.removeUser(id, readInt("ID Pengguna yang Dihapus: ")); break;
                case 7: library.displayActiveLoans(id); break;
                case 8: library.advanceDays(readInt("Majukan Berapa Hari: ")); break;
                default: cout << "Pilihan tidak dikenal.\n";
            }
        });
    }
    cout << "Logout.\n";
}

void ConsoleApp::addItemFlow(int adminId) {
    int tipe = readInt("Jenis koleksi (1=Buku, 2=Majalah, 3=BoardGame): ");
    if (tipe < 1 || tipe > 3) throw invalid_argument("Jenis koleksi tidak dikenal");

    string judul = readLine("Judul: ");
    int newId = library.nextItemId(); 
    if (tipe == 1) {
        string penulis = readLine("Penulis: ");
        int halaman = readInt("Jumlah Halaman: ");
        library.addItem(adminId, make_unique<Book>(newId, judul, penulis, halaman));
    } else if (tipe == 2) {
        int issue = readInt("Nomor Edisi: ");
        library.addItem(adminId, make_unique<Magazine>(newId, judul, issue));
    } else {
        int orang = readInt("Jumlah Pemain (orang): ");
        library.addItem(adminId, make_unique<BoardGame>(newId, judul, orang));
    }
}

void ConsoleApp::addUserFlow(int adminId) {
    int role = readInt("Peran (1=Mahasiswa, 2=Dosen, 3=Admin): ");
    if (role < 1 || role > 3) throw invalid_argument("Peran tidak dikenal");

    string nama = readLine("Nama: ");
    int newId = library.nextUserId();

    if (role == 1)
        library.addUser(adminId, make_unique<Mahasiswa>(newId, nama));
    else if (role == 2)
        library.addUser(adminId, make_unique<Dosen>(newId, nama));
    else
        library.addUser(adminId, make_unique<Admin>(newId, nama));
    cout << "ID Akun Baru: " << newId << "\n";
}

void ConsoleApp::memberMenu(const User& member) {
    const int id = member.getId();
    while (!inputClosed) {
        cout << "\nAkun: ";
        member.displayInfo();
        cout << "--- MENU ANGGOTA ---\n"
                  << " 1. Lihat Katalog Koleksi\n"
                  << " 2. Cari Koleksi (Judul / Penulis)\n"
                  << " 3. Pinjam Koleksi\n"
                  << " 4. Kembalikan Koleksi\n"
                  << " 5. Lihat Pinjaman Saya\n"
                  << " 6. Bayar Denda\n"
                  << " 7. Majukan Hari (Simulasi)\n"
                  << " 0. Logout\n";
        int choice = readInt("Pilih: ");
        if (inputClosed || choice == 0) break;

        guarded([&] {
            switch (choice) {
                case 1: library.displayAllItems(); break;
                case 2: {
                    string keyword = readLine("Kata Kunci: ");
                    auto found = library.search(keyword);
                    if (found.empty()) cout << "Tidak ada koleksi yang cocok.\n";
                    for (const LibraryItem* item : found) item->displayInfo();
                    break;
                }
                case 3: library.pinjamItem(id, readInt("ID Koleksi yang Dipinjam: ")); break;
                case 4: library.KembalikanItem(id, readInt("ID Koleksi yang Dikembalikan: ")); break;
                case 5: library.displayDendaku(id); break;
                case 6: library.BayarDenda(id, readLong("Jumlah Pembayaran (Rp): ")); break;
                case 7: library.advanceDays(readInt("Majukan Berapa Hari: ")); break;
                default: cout << "Pilihan tidak dikenal.\n";
            }
        });
    }
    cout << "Logout.\n";
}
