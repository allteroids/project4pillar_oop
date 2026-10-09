#ifndef LOAN_H
#define LOAN_H

#include <stdexcept>
using namespace std;

class Loan {
private:
    int itemId;
    int memberId;
    int HariPeminjaman;
    int TenggatHari;
    bool SudahDikembalikan;
    int HariPengembalian;

public:
    Loan(int itemId_, int memberId_, int HariPeminjaman_, int TenggatHari_)
        : itemId(itemId_), memberId(memberId_), HariPeminjaman(HariPeminjaman_),
          TenggatHari(TenggatHari_), SudahDikembalikan(false), HariPengembalian(-1) {}

    int getItemId() const { return itemId; }
    int getMemberId() const { return memberId; }
    int getHariPeminjaman() const { return HariPeminjaman; }
    int getTenggatHari() const { return TenggatHari; }
    bool isSudahDikembalikan() const { return SudahDikembalikan; }
    int getHariPengembalian() const { return HariPengembalian; }

    void markSudahDikembalikan(int day) {
        if (SudahDikembalikan) throw std::runtime_error("Peminjaman sudah dikembalikan");
        SudahDikembalikan = true;
        HariPengembalian = day;
    }
};

#endif
