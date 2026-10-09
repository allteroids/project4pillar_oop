#include "library.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "utils.h"
using namespace std;

Library::Library(unique_ptr<Admin> firstAdmin) : today(1) {
    if (!firstAdmin)
        throw invalid_argument("Perpustakaan membutuhkan minimal satu admin");
    users.push_back(move(firstAdmin)); 
}

LibraryItem* Library::findItem(int itemId_) const {
    for (const auto& item : items)
        if (item->getId() == itemId_) return item.get();
    return nullptr;
}

User* Library::findUser(int userId_) const {
    for (const auto& user : users)
        if (user->getId() == userId_
) return user.get();
    return nullptr;
}

Member* Library::findMember(int userId_) const {
    return dynamic_cast<Member*>(findUser(userId_
));
}

Loan* Library::findActiveLoan(int itemId_) {
    for (auto& loan : loans)
        if (loan.getItemId() == itemId_ && !loan.isSudahDikembalikan()) return &loan;
    return nullptr;
}

User& Library::requireIzin(int actorId, const string& action) const {
    User* actor = findUser(actorId);
    if (!actor)
        throw runtime_error("Pengguna dengan ID " + to_string(actorId) + " tidak ditemukan");
    if (!actor->canKelola())
        throw runtime_error(actor->getStatusMember() + " " + actor->getNama() + " tidak memiliki izin untuk " + action);
    return *actor;
}

Member& Library::requireMember(int userId_) const {
    User* user = findUser(userId_);
    if (!user)
        throw runtime_error("Pengguna dengan ID " + to_string(userId_) + " tidak ditemukan");
    Member* member = dynamic_cast<Member*>(user);
    if (!member)
        throw runtime_error(user->getStatusMember() + " " + user->getNama() + " bukan anggota peminjam, gunakan akun Mahasiswa/Dosen");
    return *member;
} 

void Library::addItem(int actorId, unique_ptr<LibraryItem> item) {
    User& actor = requireIzin(actorId, "menambah koleksi");
    if (!item) throw invalid_argument("Koleksi tidak valid");
    if (findItem(item->getId()))
        throw invalid_argument("ID koleksi " + to_string(item->getId()) + " sudah dipakai");
    string tipe = item->getTipe();
    string judul = item->getJudul();
    items.push_back(move(item));
    cout << "[OK] " << actor.getNama() << " menambahkan " << tipe << " '" << judul << "'\n";
}

void Library::removeItem(int actorId, int itemId_) {
    User& actor = requireIzin(actorId, "menghapus koleksi");
    LibraryItem* item = findItem(itemId_);
    if (!item) throw runtime_error("Koleksi tidak ditemukan");
    if (!item->isTersedia())
        throw runtime_error("'" + item->getJudul() + "' sedang dipinjam, tidak bisa dihapus");

    string tipe = item->getTipe();
    string judul = item->getJudul();
    items.erase(remove_if(items.begin(), items.end(),
                                [itemId_](const unique_ptr<LibraryItem>& p) {
                                    return p->getId() == itemId_;
                                }),
                 items.end());
    cout << "[OK] " << actor.getNama() << " menghapus " << tipe << " '" << judul << "'\n";
}

void Library::addUser(int actorId, unique_ptr<User> user) {
    User& actor = requireIzin(actorId, "menambah pengguna");
    if (!user) throw invalid_argument("Pengguna tidak valid");
    if (findUser(user->getId()))
        throw invalid_argument("ID pengguna " + to_string(user->getId()) +
                                    " sudah dipakai");
    string role = user->getStatusMember();
    string nama = user->getNama();
    users.push_back(move(user));
    cout << "[OK] " << actor.getNama() << " mendaftarkan " << role << " '" << nama << "'\n";
}

void Library::removeUser(int actorId, int userId_) {
    User& actor = requireIzin(actorId, "menghapus pengguna");
    if (userId_ == actorId)
        throw runtime_error("Admin tidak dapat menghapus akunnya sendiri");
    User* user = findUser(userId_);
    if (!user) throw runtime_error("Pengguna tidak ditemukan");

    if (Member* member = dynamic_cast<Member*>(user)) {
        if (member->getTotalDenda() > 0)
            throw runtime_error(member->getNama() + " masih punya denda " + formatRupiah(member->getTotalDenda()));
    }

    string role = user->getStatusMember();
    string nama = user->getNama();
    users.erase(remove_if(users.begin(), users.end(),
                                [userId_](const unique_ptr<User>& p) {
                                    return p->getId() == userId_;
                                }),
                 users.end());
    cout << "[OK] " << actor.getNama() << " menghapus " << role << " '" << nama << "'\n";
}

void Library::displayAllUsers(int actorId) const {
    requireIzin(actorId, "melihat daftar pengguna");
    for (const auto& user : users) user->displayInfo();  
}

void Library::displayActiveLoans(int actorId) const {
    requireIzin(actorId, "melihat seluruh peminjaman");
    bool any = false;
    for (const auto& loan : loans) {
        if (loan.isSudahDikembalikan()) continue;
        any = true;
        const LibraryItem* item = findItem(loan.getItemId());
        const Member* member = findMember(loan.getMemberId());
        cout << "  - " << member->getNama() << " -> '" << item->getJudul()
                  << "' (jatuh tempo hari ke-" << loan.getTenggatHari() << ")\n";
    }
    if (!any) cout << "  (tidak ada peminjaman aktif)\n";
}

void Library::pinjamItem(int memberId, int itemId_) {
    Member& member = requireMember(memberId);  
    LibraryItem* item = findItem(itemId_);
    if (!item) throw runtime_error("Koleksi tidak ditemukan");

    if (!item->isTersedia())
        throw runtime_error("'" + item->getJudul() + "' sedang dipinjam");

    int TenggatHari = today + item->getMasaPeminjaman();

    item->tandaiTerpinjam();
    member.addBarangDipinjam(itemId_);
    loans.emplace_back(itemId_, memberId, today, TenggatHari);

    cout << "[OK] " << member.getNama() << " meminjam " << item->getTipe() << " '"
              << item->getJudul() << "' (jatuh tempo hari ke-" << TenggatHari << ")\n";
}

void Library::KembalikanItem(int memberId, int itemId_) {
    Member& member = requireMember(memberId);
    Loan* loan = findActiveLoan(itemId_);
    if (!loan) throw runtime_error("Tidak ada peminjaman aktif untuk koleksi tersebut");
    if (loan->getMemberId() != memberId)
        throw runtime_error("Koleksi tersebut tidak dipinjam oleh " + member.getNama());

    LibraryItem* item = findItem(itemId_);
    int TotalKeterlambatan = max(0, today - loan->getTenggatHari());

    long fee = item->calculateDenda(TotalKeterlambatan);
    fee = fee * (100 - member.getFineDiscountPercent()) / 100;

    loan->markSudahDikembalikan(today);
    item->tandaiDikembalikan();
    member.removeBarangDipinjam(itemId_);
    if (fee > 0) member.addTotalDenda(fee);

    cout << "[OK] " << member.getNama() << " mengembalikan " << item->getTipe() << " '"
              << item->getJudul() << "'";
    if (TotalKeterlambatan > 0)
        cout << " - terlambat " << TotalKeterlambatan << " hari, denda " << formatRupiah(fee)
                  << (member.getFineDiscountPercent() > 0
                          ? " (diskon " + to_string(member.getFineDiscountPercent()) + "%)"
                          : "");
    else
        cout << " - tepat waktu";
    cout << "\n";
}

void Library::BayarDenda(int memberId, long amount) {
    Member& member = requireMember(memberId);
    member.BayarDenda(amount);
    cout << "[OK] " << member.getNama() << " membayar denda " << formatRupiah(amount)
              << ", sisa " << formatRupiah(member.getTotalDenda()) << "\n";
}

void Library::displayDendaku(int memberId) const {
    requireMember(memberId);
    bool any = false;
    for (const auto& loan : loans) {
        if (loan.isSudahDikembalikan() || loan.getMemberId() != memberId) continue;
        any = true;
        const LibraryItem* item = findItem(loan.getItemId());
        int remaining = loan.getTenggatHari() - today;
        cout << "  - [" << item->getId() << "] " << item->getTipe() << " '"
                  << item->getJudul() << "' | jatuh tempo hari ke-" << loan.getTenggatHari() << " | ";
        if (remaining >= 0)
            cout << "sisa " << remaining << " hari\n";
        else
            cout << "TERLAMBAT " << -remaining << " hari\n";
    }
    if (!any) cout << "  (anda tidak sedang meminjam apa pun)\n";
}

vector<const LibraryItem*> Library::search(const string& keyword) const {
    vector<const LibraryItem*> result;
    for (const auto& item : items)
        if (item->matches(keyword))
            result.push_back(item.get());
    return result;
}

const LibraryItem* Library::search(int itemId_) const { return findItem(itemId_); }

void Library::displayAllItems() const {
    for (const auto& item : items)
        item->displayInfo(); 
}

void Library::advanceDays(int days) {
    if (days <= 0) throw invalid_argument("Jumlah hari harus positif");
    today += days;
    cout << "[..] " << days << " hari berlalu, sekarang hari ke-" << today << "\n";
}

int Library::getToday() const { return today; }

const User* Library::getUser(int userId_) const { return findUser(userId_); }

int Library::nextItemId() const {
    int maxId = 0;
    for (const auto& item : items) maxId = max(maxId, item->getId());
    return maxId + 1;
}

int Library::nextUserId() const {
    int maxId = 0;
    for (const auto& user : users) maxId = max(maxId, user->getId());
    return maxId + 1;
}
