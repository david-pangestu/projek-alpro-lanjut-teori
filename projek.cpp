#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct Barang {
    string id;       
    string nama;     
    int stok;
    double harga;
};

struct Akun {
    string username;
    string password;
};

// ==========================================
// FUNGSI TAMBAHAN: PEMBERSIH & PENAHAN LAYAR
// ==========================================

// Fungsi ini mendeteksi OS agar fungsi clear screen tidak error 
// baik dijalankan di Windows (cls) maupun Mac/Linux (clear)
void bersihkanLayar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fungsi agar terminal tidak langsung bersih sebelum user menekan Enter
void tahanLayar() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(256, '\n'); // Membersihkan sisa buffer input (seperti karakter 'enter' sebelumnya)
    cin.get();             // Menunggu user menekan Enter
}

// ==========================================
// 1. FUNGSI CEK INVENTARIS
// ==========================================
void cekInventaris() {
    char pilihanCari;
    string kodeBarang;
    bool found;
    char pilihanCariLagi;

    cout << "\n[Sistem]: Membaca data dari inventaris.csv...\n" << endl;

    ifstream fileInventaris("inventaris.csv");

    if (!fileInventaris.is_open()) {
        cout << "ERROR: File inventaris.csv tidak ditemukan!\n" << endl;
        return;
    }

    string headerAbaikan;
    getline(fileInventaris, headerAbaikan);

    string kode, nama, stok, harga;

    cout << "===================================================" << endl;
    cout << "KODE\t| NAMA BARANG\t\t| STOK\t| HARGA" << endl;
    cout << "===================================================" << endl;

    while (getline(fileInventaris, kode, ',')) {
        getline(fileInventaris, nama, ',');  
        getline(fileInventaris, stok, ',');  
        getline(fileInventaris, harga, '\n'); 

        cout << kode << "\t| " << nama;
        if (nama.length() < 14) {
            cout << "\t\t| ";
        } else {
            cout << "\t| ";
        }
        cout << stok << "\t| Rp" << harga << endl;
    }

    cout << "===================================================\n" << endl;

    cout << "Apakah ingin mencari barang? (y/n) : "; cin >> pilihanCari;
    cout << endl;
    
    do {
        if (pilihanCari == 'y' || pilihanCari == 'Y') {
            do {
                cout << "Masukkan Kode Barang : "; cin >> kodeBarang;
                found = false;

                fileInventaris.clear();             
                fileInventaris.seekg(0, ios::beg);  
                getline(fileInventaris, headerAbaikan);

                while (getline(fileInventaris, kode, ',')) {
                    getline(fileInventaris, nama, ',');  
                    getline(fileInventaris, stok, ',');  
                    getline(fileInventaris, harga, '\n'); 

                    if (kode == kodeBarang) {
                        found = true; 
                        cout << "\n--- BARANG DITEMUKAN ---" << endl;
                        cout << "Kode Barang : " << kode << endl;
                        cout << "Nama Barang : " << nama << endl;
                        cout << "Sisa Stok   : " << stok << endl;
                        cout << "Harga       : Rp" << harga << endl;
                        cout << "------------------------\n" << endl;
                        break;
                    } 
                }

                if (!found) {
                    cout << "\n[Sistem]: Barang dengan kode '" << kodeBarang << "' tidak ditemukan!\n" << endl;
                }
            } while (!found);

            cout << "Apakah ingin mencari barang lagi? (y/n) : "; cin >> pilihanCariLagi;
            cout << endl;
        }
    } while(pilihanCariLagi == 'y' || pilihanCariLagi == 'Y');

    fileInventaris.close();
}

// ==========================================
// 2. FUNGSI PERBARUI DATA
// ==========================================
void perbaruiData(Barang databaseBarang[], int& jumlahBarang) {
    jumlahBarang = 0;
    ifstream fileInput("inventaris.csv");
    if (!fileInput.is_open()) {
        cout << "Gagal membuka file inventaris.csv!\n\n";
        return;
    }

    string header;
    getline(fileInput, header); 

    string strStok, strHarga, idTmp, namaTmp;
    while (getline(fileInput, idTmp, ',') &&
           getline(fileInput, namaTmp, ',') &&
           getline(fileInput, strStok, ',') &&
           getline(fileInput, strHarga, '\n')) {
        
        databaseBarang[jumlahBarang].id = idTmp;
        databaseBarang[jumlahBarang].nama = namaTmp;

        databaseBarang[jumlahBarang].stok = 0;
        for(size_t i=0; i < strStok.length(); i++) {
            databaseBarang[jumlahBarang].stok = databaseBarang[jumlahBarang].stok * 10 + (strStok[i] - '0');
        }
        
        databaseBarang[jumlahBarang].harga = 0;
        for(size_t i=0; i < strHarga.length() && strHarga[i] != '.'; i++) {
            databaseBarang[jumlahBarang].harga = databaseBarang[jumlahBarang].harga * 10 + (strHarga[i] - '0');
        }

        jumlahBarang++;
    }
    fileInput.close();

    char editBarangLain;
    do {
        // Bagian Tampilan Tabel yang Sudah Disamakan dengan cekInventaris
        cout << "===================================================" << endl;
        cout << "KODE\t| NAMA BARANG\t\t| STOK\t| HARGA" << endl;
        cout << "===================================================" << endl;
        for (int i = 0; i < jumlahBarang; i++) {
            cout << databaseBarang[i].id << "\t| " << databaseBarang[i].nama;
            if (databaseBarang[i].nama.length() < 14) {
                cout << "\t\t| ";
            } else {
                cout << "\t| ";
            }
            cout << databaseBarang[i].stok << "\t| Rp" << databaseBarang[i].harga << endl;
        }
        cout << "===================================================\n" << endl;

        string idInput;
        int indexDitemukan = -1;

        while (true) {
            cout << "Masukkan ID Barang yang ingin diubah: ";
            cin >> idInput;

            for (int i = 0; i < jumlahBarang; i++) {
                if (databaseBarang[i].id == idInput) {
                    indexDitemukan = i;
                    break;
                }
            }

            if (indexDitemukan != -1) {
                cout << "-> ID Barang ditemukan.\n";
                break;
            } else {
                cout << "-> ID Barang tidak ditemukan! Coba lagi.\n\n";
            }
        }

        cout << "\n--- Detail Barang Terpilih ---\n";
        cout << "ID    : " << databaseBarang[indexDitemukan].id << "\n";
        cout << "Nama  : " << databaseBarang[indexDitemukan].nama << "\n";
        cout << "Stok  : " << databaseBarang[indexDitemukan].stok << "\n";
        cout << "Harga : " << databaseBarang[indexDitemukan].harga << "\n";

        char editLagi;
        do {
            cout << "\nAtribut yang ingin di-edit:\n";
            cout << "1. Nama\n2. Stok\n3. Harga\nPilihan (1-3): ";
            int pilihanEdit;
            cin >> pilihanEdit;

            if (pilihanEdit == 1) {
                cout << "Masukkan nama baru: ";
                cin.ignore();
                getline(cin, databaseBarang[indexDitemukan].nama);
                cout << "-> Nama berhasil diperbarui.\n";
            } 
            else if (pilihanEdit == 2) {
                cout << "Masukkan stok baru: ";
                cin >> databaseBarang[indexDitemukan].stok;
                cout << "-> Stok berhasil diperbarui.\n";
            } 
            else if (pilihanEdit == 3) {
                cout << "Masukkan harga baru: ";
                cin >> databaseBarang[indexDitemukan].harga;
                cout << "-> Harga berhasil diperbarui.\n";
            } 
            else {
                cout << "Pilihan tidak valid.\n";
            }

            cout << "\nApakah ingin mengedit atribut lain dari barang ini? (y/n): ";
            cin >> editLagi;

        } while (editLagi == 'y' || editLagi == 'Y');

        cout << "\nApakah Anda ingin mengedit BARANG LAIN? (y/n): ";
        cin >> editBarangLain;
        cout << endl;

    } while (editBarangLain == 'y' || editBarangLain == 'Y');

    ofstream fileOutput("inventaris.csv");
    if (fileOutput.is_open()) {
        fileOutput << "id,nama,stok,harga\n"; 
        for (int i = 0; i < jumlahBarang; i++) {
            fileOutput << databaseBarang[i].id << "," 
                       << databaseBarang[i].nama << "," 
                       << databaseBarang[i].stok << "," 
                       << databaseBarang[i].harga << "\n";
        }
        fileOutput.close();
        cout << "\n-> Perubahan berhasil disimpan ke inventaris.csv!\n";
    } else {
        cout << "\n[Error] Gagal menyimpan data ke file!\n";
    }

    tahanLayar(); // Tahan layar sebelum kembali ke menu
}

// ==========================================
// 3. Tambah Barang
// ==========================================
void tambahBarang(){
    bool konfirmasi = false;
    char konfirmChar;
    string namaBarang, hargaBarang, stokBarang, idBaru, idTerakhir, baris;

    while (!konfirmasi)
    {
        cin.ignore();
        cout<<"Nama barang  : "; getline(cin, namaBarang);
        cout<<"Harga barang : "; cin>>hargaBarang;
        cout<<"Stok barang  : "; cin>>stokBarang;
        cin.ignore();

        cout<<"\nKonfirmasi penambahan (y/n)? ";cin>>konfirmChar;

        if (konfirmChar == 'y' || konfirmChar == 'Y')
        {
            konfirmasi = true;
        }
    }

    idTerakhir = "000";
    ifstream fileBaca ("inventaris.csv");
    if (!fileBaca.is_open()) {
        // Jika file belum ada, anggap file baru
    } else {
        string headerSkip;
        getline(fileBaca, baris);

        while (getline(fileBaca, baris))
        {
            if (!baris.empty())
            {
                idTerakhir = baris.substr(0, baris.find(','));
            }
        }
        fileBaca.close();
    }
    
    int noBaru = 0;
    for (size_t i = 0; i < idTerakhir.length(); i++) {
        noBaru = noBaru * 10 + (idTerakhir[i] - '0');
    }
    noBaru++;

    // format jadi 3 digit
    if (noBaru < 10) {
        idBaru = "00" + (char)('0' + noBaru);
    } else if (noBaru < 100) {
        idBaru = "0";
        idBaru += (char)('0' + noBaru / 10);
        idBaru += (char)('0' + noBaru % 10);
    } else {
        idBaru += (char)('0' + noBaru / 100);
        idBaru += (char)('0' + (noBaru / 10) % 10);
        idBaru += (char)('0' + noBaru % 10);
    }

    ofstream tulisBarang ("inventaris.csv", ios::app);
    if (!tulisBarang.is_open())
    {
        cout<<"ERROR: File tidak ditemukan!"<<endl;
        return;
    }

    tulisBarang << idBaru << ',' << namaBarang << ',' << stokBarang << ',' << hargaBarang << endl;

    tulisBarang.close();
    cout<<"Barang berhasil ditambahkan!" << endl;
    cout << "\nMau tambah lagi (y/n)? "; cin >> konfirmChar;

    if (konfirmChar == 'y' || konfirmChar == 'Y')
    {
        tambahBarang();
    }
    
}

// ==========================================
// 4. FUNGSI CARI TRANSAKSI
// ==========================================
void cariTransaksi() {
    bersihkanLayar(); // Bersihkan layar saat masuk ke fitur

    cout << "--- CARI HISTORI TRANSAKSI ---\n" << endl;
    cout << "[Sistem]: Membaca data transaksi...\n" << endl;

    ifstream fileTransaksi("daftar_transaksi.csv");

    if (!fileTransaksi.is_open()) {
        cout << "ERROR: File daftar_transaksi.csv tidak ditemukan!\n";
        return;
    }

    string header, cariID;
    bool ditemukan;
    char cariLagi;

    do {
        ditemukan = false;

        cout << "Masukkan ID transaksi : ";
        cin >> cariID;

        // reset pointer file ke awal
        fileTransaksi.clear();
        fileTransaksi.seekg(0, ios::beg);
        getline(fileTransaksi, header);

        string id, nama, jumlah, harga, total;
        int totalSemua = 0;

        cout << "\n========================================================\n";
        cout << left
            << setw(20) << "Barang"
            << setw(10) << "Jumlah"
            << setw(12) << "Harga"
            << setw(12) << "Subtotal"
            << endl;

        cout << "========================================================\n";

        while (
            getline(fileTransaksi, id, ',') &&
            getline(fileTransaksi, nama, ',') &&
            getline(fileTransaksi, jumlah, ',') &&
            getline(fileTransaksi, harga, ',') &&
            getline(fileTransaksi, total, '\n')
            ) {

            if (id == cariID) {
                ditemukan = true;

                cout << left
                << setw(20) << nama
                << setw(10) << jumlah
                << setw(12) << ("Rp" + harga)
                << setw(12) << ("Rp" + total)
                << endl;
                    totalSemua += stoi(total);
            }
        }

        if (ditemukan) {
            cout << "========================================================\n";
            cout << "TOTAL TRANSAKSI : Rp" << totalSemua << endl;
            cout << "========================================================\n";
        }
        else {
            cout << "\nID transaksi tidak ditemukan!\n";
            cout << "Silakan input ulang.\n\n";
        }
        
        cout << "Cari transaksi lain? (y/n): ";
        cin >> cariLagi;
        cout << endl;

    } while (cariLagi != 'n');


    fileTransaksi.close();
    tahanLayar(); // Tahan layar sebelum kembali ke menu
}


// ==========================================
// FUNGSI SORTING INVENTARIS
// ==========================================
int ubahKeAngka(string teks) {
    int angka = 0;
    for (size_t i = 0; i < teks.length(); i++) {
        if (teks[i] >= '0' && teks[i] <= '9') {
            angka = angka * 10 + (teks[i] - '0');
        }
    }
    return angka;
}

void tampilkanTabelSorting(Barang dataBarang[], int jumlahBarang) {
    cout << "===================================================" << endl;
    cout << "KODE\t| NAMA BARANG\t\t| STOK\t| HARGA" << endl;
    cout << "===================================================" << endl;

    for (int i = 0; i < jumlahBarang; i++) {
        cout << dataBarang[i].id << "\t| " << dataBarang[i].nama;
        if (dataBarang[i].nama.length() < 14) {
            cout << "\t\t| ";
        } else {
            cout << "\t| ";
        }
        cout << dataBarang[i].stok << "\t| Rp" << dataBarang[i].harga << endl;
    }

    cout << "===================================================\n" << endl;
}

bool perluDitukar(Barang dataKiri, Barang dataKanan, int kolom, int urutan) {
    if (kolom == 1) {
        if (urutan == 1) return dataKiri.id > dataKanan.id;
        else return dataKiri.id < dataKanan.id;
    } else if (kolom == 2) {
        if (urutan == 1) return dataKiri.nama > dataKanan.nama;
        else return dataKiri.nama < dataKanan.nama;
    } else if (kolom == 3) {
        if (urutan == 1) return dataKiri.stok > dataKanan.stok;
        else return dataKiri.stok < dataKanan.stok;
    } else if (kolom == 4) {
        if (urutan == 1) return dataKiri.harga > dataKanan.harga;
        else return dataKiri.harga < dataKanan.harga;
    }

    return false;
}

void bubbleSortInventaris(Barang dataBarang[], int jumlahBarang, int kolom, int urutan) {
    Barang temp;

    for (int i = 0; i < jumlahBarang - 1; i++) {
        for (int j = 0; j < jumlahBarang - 1 - i; j++) {
            if (perluDitukar(dataBarang[j], dataBarang[j + 1], kolom, urutan)) {
                temp = dataBarang[j];
                dataBarang[j] = dataBarang[j + 1];
                dataBarang[j + 1] = temp;
            }
        }
    }
}

void sortingInventaris() {
    bersihkanLayar();

    Barang dataBarang[100];
    int jumlahBarang = 0;

    cout << "--- SORTING INVENTARIS ---\n" << endl;
    cout << "[Sistem]: Membaca data dari inventaris.csv...\n" << endl;

    ifstream fileInput("inventaris.csv");
    if (!fileInput.is_open()) {
        cout << "ERROR: File inventaris.csv tidak ditemukan!\n" << endl;
        tahanLayar();
        return;
    }

    string header;
    getline(fileInput, header);

    string id, nama, stok, harga;
    while (getline(fileInput, id, ',') &&
           getline(fileInput, nama, ',') &&
           getline(fileInput, stok, ',') &&
           getline(fileInput, harga, '\n')) {
        dataBarang[jumlahBarang].id = id;
        dataBarang[jumlahBarang].nama = nama;
        dataBarang[jumlahBarang].stok = ubahKeAngka(stok);
        dataBarang[jumlahBarang].harga = ubahKeAngka(harga);
        jumlahBarang++;
    }

    fileInput.close();

    if (jumlahBarang == 0) {
        cout << "Data barang masih kosong.\n" << endl;
        tahanLayar();
        return;
    }

    int kolom, urutan;

    cout << "Urutkan berdasarkan:" << endl;
    cout << "1. Kode Barang" << endl;
    cout << "2. Nama Barang" << endl;
    cout << "3. Stok" << endl;
    cout << "4. Harga" << endl;
    cout << "Pilihan (1-4): "; cin >> kolom;

    if (kolom < 1 || kolom > 4) {
        cout << "\nPilihan tidak valid!\n";
        tahanLayar();
        return;
    }

    cout << "\nJenis urutan:" << endl;
    cout << "1. Ascending" << endl;
    cout << "2. Descending" << endl;
    cout << "Pilihan (1-2): "; cin >> urutan;

    if (urutan < 1 || urutan > 2) {
        cout << "\nPilihan tidak valid!\n";
        tahanLayar();
        return;
    }

    bubbleSortInventaris(dataBarang, jumlahBarang, kolom, urutan);

    cout << "\nData inventaris setelah diurutkan:" << endl;
    tampilkanTabelSorting(dataBarang, jumlahBarang);
    cout << "Catatan: sorting hanya untuk tampilan, tidak mengubah isi file inventaris.csv.\n";

    tahanLayar();
}

// ==========================================
// 5. FUNGSI TAMPILKAN MENU
// ==========================================
void tampilkanMenu() {
    bersihkanLayar(); // Pastikan menu utama selalu ditampilkan di layar yang bersih

    int pilihan;
    Barang databaseBarang[100];
    int jumlahBarang = 0;
    
    cout << "=================================" << endl;
    cout << "   SISTEM INVENTARIS MINIMARKET  " << endl;
    cout << "=================================" << endl;
    cout << "1. Cek Inventaris" << endl;
    cout << "2. Perbarui Data Barang" << endl;
    cout << "3. Tambah Barang Baru" << endl;
    cout << "4. Cari Histori Transaksi" << endl;
    cout << "5. Sorting Inventaris" << endl;
    cout << "6. Keluar" << endl;
    cout << "=================================" << endl;
    cout << "Pilih menu (1-6): "; cin >> pilihan;
    cout << endl;

    switch(pilihan) {
        case 1:
            cekInventaris(); 
            tampilkanMenu(); 
        break;

        case 2:
             perbaruiData(databaseBarang, jumlahBarang);
             tampilkanMenu(); 
        break;
        
        case 3:
            tambahBarang();
            tampilkanMenu();
        break;

        case 4:
            cariTransaksi();
            tampilkanMenu();
        break;

        case 5:
            sortingInventaris();
            tampilkanMenu();
        break;

        case 6:
            bersihkanLayar();
            cout << "=================================" << endl;
            cout << "  Program Selesai. Terima kasih! " << endl;
            cout << "=================================\n" << endl;
        break;

        default:
            cout << "Pilihan tidak valid! Silahkan coba lagi.\n" << endl;
            tahanLayar();
            tampilkanMenu();
        break;
    }
}

// ==========================================
// 6. FUNGSI UTAMA (MAIN)
// ==========================================
int main(){
    string inputUser, inputPass;
    int kesempatan = 3;
    int jumlahAkun = 3;

    Akun loginAkun[3] = {
        {"admin", "admin123"},
        {"gudang", "gudang123"},
        {"kasir", "kasir123"}
    };

    while (kesempatan > 0) {
        bersihkanLayar(); // Layar dibersihkan setiap kali user mencoba login ulang

        cout << "=================================" << endl;
        cout << "           HALAMAN LOGIN         " << endl;
        cout << "=================================" << endl;
        cout << "Masukkan Username: "; cin >> inputUser;
        cout << "Masukkan Password: "; cin >> inputPass;

        bool loginBerhasil = false;

        for (int i = 0; i < jumlahAkun; i++) {
            if (inputUser == loginAkun[i].username && inputPass == loginAkun[i].password) {
                loginBerhasil = true;
                break; 
            }
        }

        if (loginBerhasil == true) {
            bersihkanLayar(); // Bersihkan form login
            cout << "\n=====================================" << endl;
            cout << "           Login Berhasil!" << endl;
            cout << "       Selamat datang, " << inputUser << "." << endl;
            cout << "=====================================\n" << endl;
            
            tahanLayar(); // Biarkan user membaca pesan sukses sebelum masuk ke menu
            break; 
        }

        kesempatan--;
        cout << "\nLogin Gagal. Username atau Password salah!!" << endl;
        
        if (kesempatan > 0) {
            cout << "Kesempatan anda tinggal " << kesempatan << " lagi\n" << endl;
            tahanLayar(); // Biarkan user membaca sisa kesempatan sebelum form direfresh
        } else {
            cout << "Kesempatan anda habis. Program dihentikan!" << endl;
            return 0;
        }
    }
    
    tampilkanMenu();

    return 0;
}
