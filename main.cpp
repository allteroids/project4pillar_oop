#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "library.h"
#include "libraryitems.h"
#include "member.h"
#include "menu.h"
#include "user.h"
#include "utils.h"
using namespace std;
const int kAdminId = 1;

void attempt(const string& description, const function<void()>& action) {
    cout << ">> " << description << "\n";
    try {
        action();
    } catch (const exception& e) {
        cout << "[DITOLAK] " << e.what() << "\n";
    }
}

void section(const string& judul) {
    cout << "\n==================== " << judul << " ====================\n";
}

void seedSampleData(Library& library, bool quiet) {
    ostringstream sink;
    streambuf* original = nullptr;
    if (quiet) original = cout.rdbuf(sink.rdbuf());

    library.addItem(kAdminId, make_unique<Book>(1, "Islam Ala Prabowo", "Prob Lematik", 1));
    library.addItem(kAdminId, make_unique<Book>(2, "Isyarat dan Sistem", "Oppenheim", 450));
    library.addItem(kAdminId, make_unique<Book>(3, "Cara Prompt yang Baik dan Benar", "Dario Amodei", 464));
    library.addItem(kAdminId, make_unique<Book>(4, "Perlukah Belajar OOP?", "Azka Allida", 1000));
    library.addItem(kAdminId, make_unique<Magazine>(5, "Elle", 128));
    library.addItem(kAdminId, make_unique<BoardGame>(6, "Hues and Cues", 7));
    library.addItem(kAdminId, make_unique<BoardGame>(7, "Monopoly", 5));

    library.addUser(kAdminId, make_unique<Mahasiswa>(101, "Allida"));
    library.addUser(kAdminId, make_unique<Mahasiswa>(102, "Azka"));
    library.addUser(kAdminId, make_unique<Dosen>(201, "Dr. Nasikun"));

    if (quiet) cout.rdbuf(original);
}

void runInteractive() {
    Library library(make_unique<Admin>(kAdminId, "Admin Perpus"));
    seedSampleData(library, true);

    cout << "\nAkun Contoh:\n"
              << "  Admin     : ID 1\n"
              << "  Mahasiswa : ID 101 (Allida), ID 102 (Azka)\n"
              << "  Dosen     : ID 201 (Dr. Nasikun)\n";

    ConsoleApp app(library);
    app.run();
    cout << "Sampai jumpa.\n";
}

void runDemo() {
    Library library(make_unique<Admin>(kAdminId, "Admin Perpus"));

    section("1. Set-Up oleh Admin (Inheritance)");
    
    seedSampleData(library, false);

    cout << "\nKatalog:\n";
    library.displayAllItems(); 
    cout << "\nPengguna (hanya admin yang boleh melihat):\n";
    library.displayAllUsers(kAdminId);

    section("2. Hak Akses (Admin vs Anggota)");
    
    attempt("Allida (Mahasiswa) mencoba menambah koleksi",
            [&] { library.addItem(101, make_unique<Book>(8, "Buku Terlarang", "X", 100)); });
    attempt("Allida mencoba menghapus Buku #1", [&] { library.removeItem(101, 1); });
    attempt("Allida mencoba melihat daftar semua pengguna", [&] { library.displayAllUsers(101); });
    attempt("Dr. Nasikun (Dosen) mencoba mendaftarkan anggota baru",
            [&] { library.addUser(201, make_unique<Mahasiswa>(103, "Tamu")); });
    attempt("Admin mencoba meminjam Buku #1", [&] { library.pinjamItem(kAdminId, 1); });
    attempt("Admin menambah Buku #8", 
            [&] { library.addItem(kAdminId, make_unique<Book>(8, "Buku Usang", "Anonim", 100)); });
    attempt("Admin menghapus Buku #8", [&] { library.removeItem(kAdminId, 8); });
    attempt("Admin mendaftarkan anggota baru (ID 103)",
            [&] { library.addUser(kAdminId, make_unique<Mahasiswa>(103, "Tamu")); });
    attempt("Admin menghapus anggota ID 103", [&] { library.removeUser(kAdminId, 103); });
    attempt("Admin mencoba menghapus akunnya sendiri", [&] { library.removeUser(kAdminId, kAdminId); });

    section("3. Pencarian (Overloading / Static Polimorphism)");

    cout << "search(\"data\") -> pencarian berdasarkan kata kunci:\n";
    for (const LibraryItem* item : library.search("data")) {
        cout << "  ";
        item->displayInfo();
    }
    cout << "search(\"martin\") -> Book ikut mencocokkan penulis (override matches):\n";
    for (const LibraryItem* item : library.search("martin")) {
        cout << "  ";
        item->displayInfo();
    }
    cout << "search(6) -> pencarian berdasarkan ID:\n";
    if (const LibraryItem* found = library.search(6)) {
        cout << "  ";
        found->displayInfo();
    }

    section("4. Peminjaman oleh Anggota (Abstraction)");

    attempt("Allida meminjam Buku #1", [&] { library.pinjamItem(101, 1); });
    attempt("Allida meminjam BoardGame #6", [&] { library.pinjamItem(101, 6); });
    attempt("Azka meminjam BoardGame #7", [&] { library.pinjamItem(102, 7); });
    attempt("Dr. Nasikun meminjam Majalah #5", [&] { library.pinjamItem(201, 5); });
    attempt("Azka meminjam Buku #1 (sudah dipinjam Allida)", [&] { library.pinjamItem(102, 1); });

    cout << "\nPinjaman milik Allida (Tampilan Anggota):\n";
    library.displayDendaku(101);
    cout << "\nSemua peminjaman aktif (Tampilan Admin):\n";
    library.displayActiveLoans(kAdminId);

    attempt("Admin mencoba menghapus Buku #1 yang sedang dipinjam",
            [&] { library.removeItem(kAdminId, 1); });
    attempt("Admin mencoba menghapus Allida yang masih meminjam",
            [&] { library.removeUser(kAdminId, 101); });

    
    section("5. Batas Peminjaman Buku (Rule Encapsulation)");
   
    attempt("Allida meminjam Buku #2 (pinjaman ke-3)", [&] { library.pinjamItem(101, 2); });
    attempt("Allida meminjam Buku #3 (melebihi batas 3)", [&] { library.pinjamItem(101, 3); });

    
    section("6. Keterlambatan (Polimorphism: Tarif Denda Setiap Jenis Koleksi)");
    
    library.advanceDays(10); 
    attempt("Azka mencoba mengembalikan BoardGame #6 milik Allida", [&] { library.KembalikanItem(102, 6); });
    attempt("Allida mengembalikan Buku #1", [&] { library.KembalikanItem(101, 1); });
    attempt("Allida mengembalikan BoardGame #6", [&] { library.KembalikanItem(101, 6); });
    attempt("Azka mengembalikan BoardGame #7", [&] { library.KembalikanItem(102, 7); });
    attempt("Dr. Nasikun mengembalikan Majalah #5", [&] { library.KembalikanItem(201, 5); });
    attempt("Allida mengembalikan Buku #1 lagi (sudah dikembalikan)", [&] { library.KembalikanItem(101, 1); });

    cout << "\nStatus Pengguna:\n";
    library.displayAllUsers(kAdminId);

    section("7. Denda Melebihi Batas dan Pembayaran");

    attempt("Allida meminjam BoardGame #6 lagi", [&] { library.pinjamItem(101, 6); });
    library.advanceDays(6);
    attempt("Allida mengembalikan BoardGame #6 (telat lagi)", [&] { library.KembalikanItem(101, 6); });
    library.advanceDays(1);
    attempt("Allida meminjam BoardGame #6 sekali lagi", [&] { library.pinjamItem(101, 6); });
    library.advanceDays(10); 
    attempt("Allida mengembalikan BoardGame #6", [&] { library.KembalikanItem(101, 6); });

    attempt("Allida meminjam Buku #4 (denda masih di atas batas)", [&] { library.pinjamItem(101, 4); });
    attempt("Allida membayar denda Rp 1.000.000 (lebih dari tagihan)", [&] { library.BayarDenda(101, 1000000); });
    attempt("Allida membayar denda Rp -5.000 (tidak valid)", [&] { library.BayarDenda(101, -5000); });
    attempt("Admin mencoba membayar denda (bukan anggota)", [&] { library.BayarDenda(kAdminId, 1000); });
    attempt("Allida membayar sebagian denda Rp 50.000", [&] { library.BayarDenda(101, 50000); });
    attempt("Allida meminjam Buku #4 (denda kini di bawah batas)", [&] { library.pinjamItem(101, 4); });

    cout << "\nStatus Pengguna:\n";
    library.displayAllUsers(kAdminId);

    section("8. Encapsulation: Menolak Data yang Tidak Valid");

    attempt("Membuat Buku dengan judul kosong", [&] { Book b(99, "", "X", 100); });
    attempt("Membuat BoardGame dengan jumlah pemain 0", [&] { BoardGame d(98, "Kosong", 0); });
    attempt("Membuat Mahasiswa dengan ID 0", [&] { Mahasiswa s(0, "Hantu"); });
    attempt("Menambah koleksi dengan ID duplikat", [&] {
        library.addItem(kAdminId, make_unique<Book>(1, "Duplikat", "Z", 10));
    });

    section("9. Polimorphism: Koleksi Heterogen Menggunakan Pointer Base Class");

    vector<unique_ptr<LibraryItem>> samples;
    samples.push_back(make_unique<Book>(10, "Contoh Buku", "Penulis", 100));
    samples.push_back(make_unique<Magazine>(11, "Contoh Majalah", 1));
    samples.push_back(make_unique<BoardGame>(12, "Contoh BoardGame", 60));

    cout << "Jenis      | Masa pinjam | Denda Telat 5 Hari\n";
    for (const auto& s : samples)
        cout << s->getTipe() << string(11 - s->getTipe().size(), ' ') << "| "
                  << s->getMasaPeminjaman() << " hari"
                  << string(s->getMasaPeminjaman() < 10 ? 6 : 5, ' ') << "| "
                  << formatRupiah(s->calculateDenda(5)) << "\n";

    cout << "\nPengguna: Izin Akses Setiap User (Polimorphism pada User)\n";
    vector<unique_ptr<User>> people;
    people.push_back(make_unique<Admin>(1, "Contoh Admin"));
    people.push_back(make_unique<Mahasiswa>(2, "Contoh Mahasiswa"));
    people.push_back(make_unique<Dosen>(3, "Contoh Dosen"));
    for (const auto& p : people)
        cout << "  " << p->getStatusMember() << ": Kelola Perpustakaan = "
                  << (p->canKelola() ? "YA" : "TIDAK") << "\n";

    cout << "\nSelesai. Semua objek dibersihkan otomatis oleh unique_ptr.\n";
}



int main() {
    cout << "=== SISTEM MANAJEMEN PERPUSTAKAAN ===\n"
              << " 1. Mode Interaktif (Login sebagai Admin / Anggota)\n"
              << " 2. Demo Otomatis \n"
              << " 0. Keluar\n"
              << "Pilih: ";

    string input;
    if (!getline(cin, input)) return 0;
    char choice = input.empty() ? ' ' : input[0];

    if (choice == '1')
        runInteractive();
    else if (choice == '2')
        runDemo();
    else
        cout << "Sampai jumpa.\n";
    return 0;
}
