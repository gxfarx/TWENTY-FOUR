#include "gametime.h"

Sesi riwayat[MAKS_RIWAYAT];   
int jumlahRiwayat = 0;        
int nomorBerikutnya = 1;      
long long batasDetik = 0;     


// ============================================================================
// BAGIAN JENI
// Fungsi: formatWaktu, tampilMenu, aturBatasWaktu, simpanRiwayat
// ============================================================================

string formatWaktu(long long detik) {
    long long jam = detik / 3600;            
    long long menit = (detik % 3600) / 60;   
    long long sisaDetik = detik % 60;        

    ostringstream os;
    os << setfill('0') << setw(2) << jam << ":"
       << setw(2) << menit << ":"
       << setw(2) << sisaDetik;
    return os.str();
}

void tampilMenu() {
    cout << "\n========================================\n";
    cout << "             GAME TIME GUARD\n";
    cout << "========================================\n";

    cout << "Batas waktu saat ini : ";
    if (batasDetik == 0) {
        cout << "Belum diatur\n";
    } else {
        cout << batasDetik / 60 << " menit\n";
    }

    cout << "----------------------------------------\n";
    cout << "[1] Mulai sesi bermain\n";
    cout << "[2] Atur batas waktu\n";
    cout << "[3] Lihat riwayat bermain\n";
    cout << "[4] Keluar\n";
    cout << "----------------------------------------\n";
    cout << "Pilih menu (1-4): ";
}

void aturBatasWaktu() {
    long long menit;
    cout << "\nMasukkan batas waktu bermain (menit): ";

    // Validasi input: harus angka, lebih dari 0, dan maksimal 24 jam atau 1440 menit
    if (!(cin >> menit) || menit <= 0 || menit > 1440) {
        cin.clear();                                          // reset status error cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // buang sisa input
        cout << "Input tidak valid. Batas waktu tidak diubah.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // buang sisa baris
    batasDetik = menit * 60;                              // menit -> detik
    cout << "Batas waktu berhasil diubah menjadi " << menit << " menit.\n";
}

void simpanRiwayat(long long durasi, int peringatan) {
    if (jumlahRiwayat == MAKS_RIWAYAT) {
        for (int i = 1; i < MAKS_RIWAYAT; ++i) {
            riwayat[i - 1] = riwayat[i];   // geser data ke kiri
        }
        --jumlahRiwayat;                   // sediakan 1 slot kosong di akhir
    }

    riwayat[jumlahRiwayat].nomor = nomorBerikutnya++;
    riwayat[jumlahRiwayat].durasiDetik = durasi;
    riwayat[jumlahRiwayat].jumlahPeringatan = peringatan;
    ++jumlahRiwayat;
}
// ============================ AKHIR BAGIAN JENI =============================


// ============================================================================
// BAGIAN GIFFAR
// Fungsi: tampilRiwayat, simpanKeFile, muatDariFile
// ============================================================================

void tampilRiwayat() {
    cout << "\n============== RIWAYAT BERMAIN ==============\n";

    if (jumlahRiwayat == 0) {
        cout << "Belum ada riwayat bermain.\n";
        return;
    }

    long long total = 0;   // akumulasi seluruh durasi
    cout << left << setw(10) << "Sesi"
         << setw(16) << "Durasi"
         << "Peringatan\n";
    cout << "---------------------------------------------\n";

    for (int i = 0; i < jumlahRiwayat; ++i) {
        cout << left << setw(10) << riwayat[i].nomor
             << setw(16) << formatWaktu(riwayat[i].durasiDetik)
             << riwayat[i].jumlahPeringatan << " kali\n";
        total += riwayat[i].durasiDetik;
    }

    cout << "---------------------------------------------\n";
    cout << "Total waktu bermain : " << formatWaktu(total) << "\n";
    cout << "Rata-rata per sesi  : "
         << formatWaktu(total / jumlahRiwayat) << "\n";
}

void simpanKeFile() {
    ofstream file(NAMA_FILE.c_str());

    if (!file) {
        cout << "Gagal menyimpan riwayat ke file.\n";
        return;
    }

    for (int i = 0; i < jumlahRiwayat; ++i) {
        file << riwayat[i].nomor << " "
             << riwayat[i].durasiDetik << " "
             << riwayat[i].jumlahPeringatan << "\n";
    }
}

void muatDariFile() {
    ifstream file(NAMA_FILE.c_str());
    if (!file) return;   // file belum ada -> tidak ada yang dimuat

    Sesi sesi;
    while (jumlahRiwayat < MAKS_RIWAYAT &&
           file >> sesi.nomor >> sesi.durasiDetik >> sesi.jumlahPeringatan) {
        riwayat[jumlahRiwayat++] = sesi;
        if (sesi.nomor >= nomorBerikutnya) {
            nomorBerikutnya = sesi.nomor + 1;
        }
    }
}
// ============================ AKHIR BAGIAN GIFFAR ===========================


// ============================================================================
// BAGIAN SUKMA
// Fungsi: bunyiAlarmHeboh (pembantu), tampilPeringatan, jalankanSesi
// ============================================================================

static void bunyiAlarmHeboh() {
    // 1) Sirine: frekuensi naik lalu turun, diulang 3 kali
    for (int ulang = 0; ulang < 3; ++ulang) {
        for (int f = 600; f <= 1800; f += 100) Beep(f, 25);   // naik
        for (int f = 1800; f >= 600; f -= 100) Beep(f, 25);   // turun
    }

    // 2) Beep cepat selang-seling nada tinggi dan sangat tinggi
    for (int i = 0; i < 10; ++i) {
        Beep(2500, 70);
        Beep(1500, 70);
    }

    // 3) Beep panjang sebagai penutup
    Beep(2000, 700);

    // Buang tombol yang tidak sengaja tertekan selama alarm berbunyi
    while (_kbhit()) _getch();
}

void tampilPeringatan(long long durasi, int nomorPeringatan) {
    cout << "\n========== PERINGATAN #" << nomorPeringatan << " ==========\n";
    cout << "Kamu sudah bermain selama " << formatWaktu(durasi) << ".\n";
    cout << "Waktunya istirahat! Minum air, peregangan, dan rehat mata.\n";
    cout << "==========================================\n";

    bunyiAlarmHeboh();   // bunyikan alarm
}

void jalankanSesi() {
    // Batas waktu belum diatur -> sesi tidak boleh dimulai
    if (batasDetik <= 0) {
        cout << "\nBatas waktu belum diatur!\n";
        cout << "Silakan pilih menu [2] untuk mengatur batas waktu dulu.\n";
        return;
    }

    int jumlahPeringatan = 0;
    const steady_clock::time_point waktuAwal = steady_clock::now();  // titik mulai
    long long peringatanBerikutnya = batasDetik;   // detik ke berapa peringatan muncul

    cout << "\nSesi bermain dimulai!\n";
    cout << "Batas waktu: " << batasDetik / 60 << " menit.\n";
    cout << "Tekan ENTER untuk mengakhiri sesi bermain.\n";
    cout << "Pemantauan waktu berjalan otomatis...\n";

    while (true) {
        // Hitung lama bermain sampai saat ini
        long long durasi = duration_cast<seconds>(
            steady_clock::now() - waktuAwal
        ).count();

        // Waktunya peringatan? Tampilkan lalu jadwalkan peringatan berikutnya
        if (durasi >= peringatanBerikutnya) {
            ++jumlahPeringatan;
            tampilPeringatan(durasi, jumlahPeringatan);
            peringatanBerikutnya =
                static_cast<long long>(jumlahPeringatan + 1) * batasDetik;
            cout << "Sesi masih berjalan. Tekan ENTER untuk berhenti.\n";
        }

        // Cek tombol tanpa menghentikan penghitungan waktu
        if (_kbhit()) {
            int tombol = _getch();
            if (tombol == '\r' || tombol == '\n') {
                break;   // ENTER ditekan -> akhiri sesi
            }
        }

        Sleep(100);   // jeda 0,1 detik agar CPU tidak terbebani
    }

    // Hitung durasi final, lalu simpan ke riwayat dan file
    long long durasiAkhir = duration_cast<seconds>(
        steady_clock::now() - waktuAwal
    ).count();

    simpanRiwayat(durasiAkhir, jumlahPeringatan);
    simpanKeFile();

    cout << "\nSesi selesai.\n";
    cout << "Durasi bermain    : " << formatWaktu(durasiAkhir) << "\n";
    cout << "Jumlah peringatan : " << jumlahPeringatan << "\n";
    cout << "Riwayat tersimpan di file " << NAMA_FILE << ".\n";
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.get();
}
// ============================ AKHIR BAGIAN SUKMA ============================