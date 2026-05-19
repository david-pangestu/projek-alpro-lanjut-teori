#include <iostream>
#include <fstream>

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

    cout << "\nTekan Enter untuk kembali ke Menu Utama...";
    cin.ignore(); 
    cin.get();    
    cout << endl;
}

// ==========================================
// 3. Tambah data
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

        cout<<"Konfirmasi penambahan (y/n)? ";cin>>konfirmChar;

        if (konfirmChar == 'y' || konfirmChar == 'Y')
        {
            konfirmasi = true;
        }
    }

    idTerakhir = "000";
    ifstream fileBaca ("inventaris.csv");
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
    cout<<"Barang berhasil ditambahkan!" << endl << "Mau tambah lagi (y/n)? "; cin>>konfirmChar;

    if (konfirmChar == 'y' || konfirmChar == 'Y')
    {
        tambahBarang();
    }
    

}

// ==========================================
// 4. FUNGSI TAMPILKAN MENU
// ==========================================
void tampilkanMenu() {
    int pilihan;
    Barang databaseBarang[100];
    int jumlahBarang = 0;
    
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

        case 2:
             perbaruiData(databaseBarang, jumlahBarang);
             tampilkanMenu(); 
        break;
        
        case 3:
            tambahBarang();
            tampilkanMenu();
        break;

        case 5:
            cout << "Program Selesai. Terima kasih!" << endl;
        break;

        default:
            cout << "Pilihan tidak valid! Silahkan coba lagi.\n" << endl;
            tampilkanMenu();
        break;
    }
}

// ==========================================
// 5. FUNGSI UTAMA (MAIN)
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
            cout << "           Login Berhasil!" << endl;
            cout << "       Selamat datang, " << inputUser << "." << endl;
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
