#ifndef LIBRARY_ITEM_H
#define LIBRARY_ITEM_H
#include <string>
using namespace std;

class LibraryItem {
private:
    int id;
    string judul;
    bool tersedia;

public:
    LibraryItem(int id, const string& judul);
    virtual ~LibraryItem() = default;  

    int getId() const;
    const string& getJudul() const;
    bool isTersedia() const;

    void tandaiTerpinjam();   
    void tandaiDikembalikan();  
    virtual string getTipe() const = 0;
    virtual int getMasaPeminjaman() const = 0;
    virtual long calculateDenda(int TotalKeterlambatan) const = 0;
    virtual void displayInfo() const;
    virtual bool matches(const string& keyword) const;
};

#endif
