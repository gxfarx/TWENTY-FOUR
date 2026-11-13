// ini jumria
#ifndef GAMETIME_H
#define GAMETIME_H

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <conio.h>
#include <windows.h>

using namespace std;
using namespace chrono;

const int MAKS_RIWAYAT = 50;
const string NAMA_FILE = "riwayat.txt";

struct Sesi {
    int nomor;
    long long durasiDetik;
    int jumlahPeringatan;
};

extern Sesi riwayat[MAKS_RIWAYAT];
extern int jumlahRiwayat;
extern int nomorBerikutnya;
extern long long batasDetik;

string formatWaktu(long long detik);
void tampilMenu();
void aturBatasWaktu();
void simpanRiwayat(long long durasi, int peringatan);
void tampilRiwayat();
void simpanKeFile();
void muatDariFile();
void tampilPeringatan(long long durasi, int nomorPeringatan);
void jalankanSesi();

#endif
