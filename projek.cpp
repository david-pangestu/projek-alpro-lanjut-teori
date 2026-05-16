#include <iostream>
#include <fstream>

using namespace std;

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

                // Reset kursor agar kembali ke awal file
                fileInventaris.clear();             // Menghapus tanda bahwa file sudah mentok (EOF)
                fileInventaris.seekg(0, ios::beg);  // Menggulung kursor kembali ke byte ke-0 (begin)

                // Baca ulang file dari atas ke bawah untuk mencari kecocokan
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

struct Akun {
    string username;
    string password;
};

void tampilkanMenu() {
    int pilihan;
    
    cout << "=== DAFTAR PILIHAN ===" << endl;
    cout << "1. Cek Inventaris" << endl;
    cout << "2. Perbarui Data" << endl;
    cout << "3. Tambah Barang" << endl;
    cout << "4. Cari Transaksi" << endl;
    cout << "5. Keluar" << endl;
    cout << "Pilih yang mana? : "; cin >> pilihan;
    cout << endl;

    switch(pilihan) {
        case 1:
            cekInventaris(); 
            tampilkanMenu(); 
        break;

        // case 2:
        //     perbaruiData();
        //     tampilkanMenu(); 
        // break;   
        
        // case 3:
        //     tambahBarang();
        //     tampilkanMenu(); 
        // break;

        // case 4:
        //     cariTransaksi();
        //     tampilkanMenu(); 
        // break;

        case 5:
            cout << "Program Selesai. Terima kasih!" << endl;
        break;

        default:
            cout << "Pilihan tidak valid! Silahkan coba lagi.\n" << endl;
            tampilkanMenu();
        break;
    }
}

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
        cout << "==== LOGIN ====" << endl;
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
            cout << "\n=====================================" << endl;
            cout << "Login Berhasil!" << endl;
            cout << "Selamat datang, " << inputUser << "." << endl;
            cout << "=====================================\n" << endl;
            break; 
        }

        kesempatan--;
        cout << "Login Gagal. Username atau Password salah!!" << endl;
       
        if (kesempatan > 0) {
            cout << "Kesempatan anda tinggal " << kesempatan << " lagi\n" << endl;
        } else {
            cout << "Kesempatan anda habis. Program dihentikan!" << endl;
            return 0;
        }
    }
    
    tampilkanMenu();

    return 0;
}