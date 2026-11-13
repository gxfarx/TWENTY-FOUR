// jumria
#include "gametime.h"

int main() {
    muatDariFile();
    int pilihan = 0;

    do {
        tampilMenu();

        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Masukkan angka 1 sampai 4.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1:
                jalankanSesi();
                break;
            case 2:
                aturBatasWaktu();
                break;
            case 3:
                tampilRiwayat();
                break;
            case 4:
                simpanKeFile();
                cout << "Riwayat disimpan. Sampai jumpa!\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan pilih 1-4.\n";
        }
    } while (pilihan != 4);

    return 0;
}
