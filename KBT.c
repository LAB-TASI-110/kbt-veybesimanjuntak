#include <stdio.h>    // Untuk fungsi input/output standar seperti printf, scanf
#include <stdlib.h>   // Untuk fungsi utilitas umum seperti system("cls") atau exit()
#include <string.h>   // Untuk fungsi manipulasi string seperti strcpy, strcmp

// --- STRUKTUR DATA ---
// Definisi struktur untuk merepresentasikan informasi rute dan tarif
typedef struct {
    char asal[20];
    char tujuan[20];
    int hargaEkonomi;
    int hargaExecutive;
} Rute;

// Definisi struktur untuk merepresentasikan sebuah tiket
typedef struct {
    int idTiket;
    char namaAnggota[50];
    char rute[50];
    char kelas[20];
    long hargaFinal;
    char status[20]; // Contoh: "Terbit", "Selesai", "Batal"
    char komentar[100]; // Komentar tambahan untuk update
    int isFreeTicket; // 1 jika tiket gratis, 0 jika tidak
} Tiket;

// Definisi struktur untuk merepresentasikan data loyalty anggota
typedef struct {
    char namaAnggota[50];
    int tripCount;    // Jumlah perjalanan yang telah dilakukan
    int freeTicketsAvailable; // Jumlah tiket gratis yang bisa digunakan
} Loyalty;

// --- VARIABEL GLOBAL ---
// Array untuk menyimpan daftar rute yang tersedia
Rute daftarRute[] = {
    {"Medan", "Parapat", 70000, 100000},
    {"Medan", "Porsea/Balige", 90000, 130000},
    {"Medan", "Siborongborong", 110000, 150000},
    {"Medan", "Tarutung", 120000, 160000},
    {"Medan", "Padang Sidempuan", 150000, 200000},
    {"Medan", "Sibolga", 140000, 180000}
};
// Jumlah rute yang ada dalam array daftarRute
const int JUMLAH_RUTE = sizeof(daftarRute) / sizeof(daftarRute[0]);

// Array untuk menyimpan daftar tiket yang telah dibuat
Tiket daftarTiket[100]; // Maksimal 100 tiket
int jumlahTiket = 0;    // Penghitung jumlah tiket saat ini
int nextIdTiket = 1;    // ID tiket berikutnya yang akan digunakan

// Array untuk menyimpan data loyalty anggota
Loyalty daftarLoyalty[50]; // Maksimal 50 anggota loyal
int jumlahLoyalty = 0;     // Penghitung jumlah anggota loyal saat ini

// --- PROTOTIPE FUNGSI ---
// Deklarasi fungsi-fungsi yang akan digunakan dalam program
void tampilkanMenuUtama();
void buatTiketBaru();
void lihatSemuaTiket();
void updateTiket();
void lihatTarifTiket();
void lihatStatusLoyalty();
void clearScreen();
void pressEnterToContinue();
long hitungHargaTiket(char* rute, char* kelas);
void updateLoyalty(char* namaAnggota);
Loyalty* findLoyalty(char* namaAnggota);
Rute* findRute(char* tujuan);

// --- FUNGSI UTAMA (MAIN FUNCTION) ---
int main() {
    int pilihan;

    // Loop utama program untuk menampilkan menu dan memproses pilihan pengguna
    do {
        clearScreen();          // Membersihkan layar konsol
        tampilkanMenuUtama();   // Menampilkan opsi menu ke pengguna
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &pilihan);  // Membaca pilihan pengguna
        // Membersihkan buffer input setelah scanf untuk menghindari masalah dengan fgets/gets di kemudian hari
        while (getchar() != '\n'); 

        clearScreen(); // Membersihkan layar sebelum menampilkan hasil dari pilihan

        // Struktur kontrol switch-case untuk menjalankan fungsi berdasarkan pilihan pengguna
        switch (pilihan) {
            case 1:
                buatTiketBaru();        // Memanggil fungsi untuk membuat tiket baru
                break;
            case 2:
                lihatSemuaTiket();      // Memanggil fungsi untuk melihat semua tiket
                break;
            case 3:
                updateTiket();          // Memanggil fungsi untuk memperbarui tiket
                break;
            case 4:
                lihatTarifTiket();      // Memanggil fungsi untuk melihat daftar tarif
                break;
            case 5:
                lihatStatusLoyalty();   // Memanggil fungsi untuk melihat status loyalty
                break;
            case 0:
                printf("Terima kasih telah menggunakan sistem Ticketing KBT. Sampai jumpa!\n");
                break; // Keluar dari loop dan program
            default:
                printf("Pilihan tidak valid. Mohon masukkan angka sesuai menu.\n");
                break;
        }
        pressEnterToContinue(); // Menunggu pengguna menekan Enter sebelum kembali ke menu
    } while (pilihan != 0); // Loop terus berjalan selama pilihan bukan 0

    return 0; // Mengakhiri program dengan sukses
}

// --- IMPLEMENTASI FUNGSI ---

// Membersihkan layar konsol (cross-platform)
void clearScreen() {
    // Untuk sistem operasi Windows
    #ifdef _WIN32
        system("cls");
    // Untuk sistem operasi Unix/Linux/macOS
    #else
        system("clear");
    #endif
}

// Fungsi untuk menjeda eksekusi hingga pengguna menekan Enter
void pressEnterToContinue() {
    printf("\nTekan ENTER untuk melanjutkan...");
    // Membaca karakter hingga newline ditemukan untuk menunggu Enter
    while (getchar() != '\n'); 
}

// Menampilkan menu utama aplikasi kepada pengguna
void tampilkanMenuUtama() {
    printf("========================================\n");
    printf("   Sistem Ticketing Koperasi Bintang Tapanuli (KBT)\n");
    printf("========================================\n");
    printf("1. Buat Tiket Baru\n");
    printf("2. Lihat Semua Tiket\n");
    printf("3. Update Tiket\n");
    printf("4. Lihat Tarif Tiket\n");
    printf("5. Lihat Status Loyalty Anggota\n");
    printf("0. Keluar\n");
    printf("----------------------------------------\n");
}

// Mencari rute berdasarkan nama tujuan
Rute* findRute(char* tujuan) {
    for (int i = 0; i < JUMLAH_RUTE; i++) {
        // Membandingkan string tujuan dengan daftar rute yang tersedia
        if (strcmp(daftarRute[i].tujuan, tujuan) == 0) {
            return &daftarRute[i]; // Mengembalikan pointer ke rute jika ditemukan
        }
    }
    return NULL; // Mengembalikan NULL jika rute tidak ditemukan
}

// Menghitung harga tiket berdasarkan rute dan kelas
long hitungHargaTiket(char* tujuan, char* kelas) {
    Rute* ruteDitemukan = findRute(tujuan); // Mencari rute yang sesuai

    if (ruteDitemukan == NULL) {
        printf("Rute '%s' tidak ditemukan.\n", tujuan);
        return 0; // Mengembalikan 0 jika rute tidak valid
    }

    // Menentukan harga berdasarkan kelas yang dipilih
    if (strcmp(kelas, "Ekonomi") == 0) {
        return ruteDitemukan->hargaEkonomi;
    } else if (strcmp(kelas, "Executive") == 0) {
        return ruteDitemukan->hargaExecutive;
    } else {
        printf("Kelas '%s' tidak valid. Hanya Ekonomi atau Executive.\n", kelas);
        return 0; // Mengembalikan 0 jika kelas tidak valid
    }
}

// Mencari data loyalty anggota berdasarkan nama
Loyalty* findLoyalty(char* namaAnggota) {
    for (int i = 0; i < jumlahLoyalty; i++) {
        if (strcmp(daftarLoyalty[i].namaAnggota, namaAnggota) == 0) {
            return &daftarLoyalty[i]; // Mengembalikan pointer ke data loyalty jika ditemukan
        }
    }
    return NULL; // Mengembalikan NULL jika anggota belum terdaftar di loyalty
}

// Memperbarui atau menambahkan data loyalty anggota
void updateLoyalty(char* namaAnggota) {
    Loyalty* loyaltyAnggota = findLoyalty(namaAnggota);

    if (loyaltyAnggota == NULL) {
        // Jika anggota belum ada dalam daftar loyalty, tambahkan baru
        if (jumlahLoyalty < 50) { // Pastikan array belum penuh
            strcpy(daftarLoyalty[jumlahLoyalty].namaAnggota, namaAnggota);
            daftarLoyalty[jumlahLoyalty].tripCount = 1; // Perjalanan pertama
            daftarLoyalty[jumlahLoyalty].freeTicketsAvailable = 0;
            loyaltyAnggota = &daftarLoyalty[jumlahLoyalty]; // Arahkan pointer ke anggota baru
            jumlahLoyalty++;
        } else {
            printf("Batas maksimum anggota loyalty tercapai.\n");
            return;
        }
    } else {
        // Jika anggota sudah ada, tingkatkan trip count
        loyaltyAnggota->tripCount++;
    }

    // Cek apakah anggota berhak mendapatkan tiket gratis (setiap 10 perjalanan)
    if (loyaltyAnggota->tripCount % 10 == 0) {
        loyaltyAnggota->freeTicketsAvailable++;
        printf("Selamat! %s mendapatkan 1 tiket gratis karena telah mencapai %d perjalanan!\n",
               namaAnggota, loyaltyAnggota->tripCount);
    }
}


// FUNGSI 1: Membuat Tiket Baru
void buatTiketBaru() {
    if (jumlahTiket >= 100) {
        printf("Batas maksimum tiket (100) telah tercapai. Tidak bisa membuat tiket baru.\n");
        return;
    }

    Tiket newTiket;
    char inputRute[50];
    char inputKelas[20];
    long hargaDasar;
    int diskonPersen = 0;
    char diskonInfo[50] = ""; // Untuk menyimpan informasi diskon

    newTiket.idTiket = nextIdTiket++; // Berikan ID tiket baru dan tingkatkan penghitung

    printf("--- Buat Tiket Baru ---\n");
    printf("Nama Anggota: ");
    fgets(newTiket.namaAnggota, sizeof(newTiket.namaAnggota), stdin);
    // Menghapus karakter newline '\n' yang mungkin terbawa oleh fgets
    newTiket.namaAnggota[strcspn(newTiket.namaAnggota, "\n")] = 0;

    printf("Pilih Rute (contoh: Parapat, Porsea/Balige, dll.): ");
    fgets(inputRute, sizeof(inputRute), stdin);
    inputRute[strcspn(inputRute, "\n")] = 0;
    strcpy(newTiket.rute, inputRute); // Salin rute yang diinput ke struktur tiket

    printf("Pilih Kelas (Ekonomi/Executive): ");
    fgets(inputKelas, sizeof(inputKelas), stdin);
    inputKelas[strcspn(inputKelas, "\n")] = 0;
    strcpy(newTiket.kelas, inputKelas); // Salin kelas yang diinput ke struktur tiket
    
    // Konversi input kelas ke format yang konsisten (huruf pertama kapital)
    if (strcmp(inputKelas, "ekonomi") == 0 || strcmp(inputKelas, "Ekonomi") == 0) {
        strcpy(newTiket.kelas, "Ekonomi");
    } else if (strcmp(inputKelas, "executive") == 0 || strcmp(inputKelas, "Executive") == 0) {
        strcpy(newTiket.kelas, "Executive");
    } else {
        printf("Kelas tidak valid. Mohon pilih 'Ekonomi' atau 'Executive'.\n");
        return;
    }

    hargaDasar = hitungHargaTiket(newTiket.rute, newTiket.kelas);

    if (hargaDasar == 0) {
        printf("Pembuatan tiket dibatalkan karena rute atau kelas tidak valid.\n");
        return;
    }

    // Cek status loyalty anggota
    Loyalty* loyaltyAnggota = findLoyalty(newTiket.namaAnggota);
    newTiket.isFreeTicket = 0; // Default: bukan tiket gratis

    if (loyaltyAnggota != NULL && loyaltyAnggota->freeTicketsAvailable > 0) {
        char konfirmasiFreeTicket;
        printf("Anggota '%s' memiliki %d tiket gratis. Gunakan sekarang? (y/n): ", 
               newTiket.namaAnggota, loyaltyAnggota->freeTicketsAvailable);
        scanf(" %c", &konfirmasiFreeTicket); // Spasi sebelum %c untuk membersihkan buffer
        while (getchar() != '\n'); // Membersihkan buffer

        if (konfirmasiFreeTicket == 'y' || konfirmasiFreeTicket == 'Y') {
            newTiket.hargaFinal = 0; // Tiket gratis
            newTiket.isFreeTicket = 1;
            loyaltyAnggota->freeTicketsAvailable--; // Kurangi jumlah tiket gratis yang tersedia
            strcpy(diskonInfo, "Tiket GRATIS (Loyalty)");
        }
    }

    if (newTiket.isFreeTicket == 0) { // Hanya terapkan diskon jika bukan tiket gratis
        // Logika diskon berdasarkan kelas dan rute
        if (strcmp(newTiket.kelas, "Executive") == 0) {
            // Contoh diskon untuk kelas Executive ke rute tertentu
            if (strcmp(newTiket.rute, "Padang Sidempuan") == 0) {
                diskonPersen = 10; // Diskon 10% untuk Executive ke Padang Sidempuan
                strcpy(diskonInfo, "Diskon 10% (Executive Padang Sidempuan)");
            } else {
                diskonPersen = 5; // Diskon 5% untuk Executive rute lainnya
                strcpy(diskonInfo, "Diskon 5% (Executive)");
            }
        } else if (strcmp(newTiket.kelas, "Ekonomi") == 0) {
             // Contoh diskon untuk kelas Ekonomi ke rute tertentu
            if (strcmp(newTiket.rute, "Parapat") == 0) {
                diskonPersen = 3; // Diskon 3% untuk Ekonomi ke Parapat
                strcpy(diskonInfo, "Diskon 3% (Ekonomi Parapat)");
            }
        }
        
        newTiket.hargaFinal = hargaDasar - (hargaDasar * diskonPersen / 100);
    }

    strcpy(newTiket.status, "Terbit"); // Status awal tiket
    strcpy(newTiket.komentar, "-");     // Komentar awal
    
    daftarTiket[jumlahTiket] = newTiket; // Tambahkan tiket baru ke array
    jumlahTiket++;                       // Tingkatkan jumlah tiket

    // Perbarui data loyalty setelah tiket berhasil dibuat (termasuk tiket gratis)
    updateLoyalty(newTiket.namaAnggota);

    printf("\n--- Tiket Berhasil Dibuat ---\n");
    printf("ID Tiket      : %d\n", newTiket.idTiket);
    printf("Nama Anggota  : %s\n", newTiket.namaAnggota);
    printf("Rute          : %s\n", newTiket.rute);
    printf("Kelas         : %s\n", newTiket.kelas);
    printf("Harga Dasar   : Rp%ld\n", hargaDasar);
    if (strlen(diskonInfo) > 0) {
        printf("Info Diskon   : %s\n", diskonInfo);
    }
    printf("Harga Final   : Rp%ld\n", newTiket.hargaFinal);
    printf("Status Tiket  : %s\n", newTiket.status);
    printf("-------------------------------\n");
}

// FUNGSI 2: Melihat Semua Tiket
void lihatSemuaTiket() {
    printf("--- Daftar Semua Tiket ---\n");
    if (jumlahTiket == 0) {
        printf("Belum ada tiket yang terdaftar.\n");
        return;
    }

    printf("%-5s | %-20s | %-20s | %-10s | %-12s | %-10s | %-10s\n", 
           "ID", "Nama Anggota", "Rute", "Kelas", "Harga Final", "Status", "Gratis?");
    printf("--------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahTiket; i++) {
        Loyalty* loyaltyAnggota = findLoyalty(daftarTiket[i].namaAnggota);
        int tripCount = (loyaltyAnggota != NULL) ? loyaltyAnggota->tripCount : 0;
        int freeTickets = (loyaltyAnggota != NULL) ? loyaltyAnggota->freeTicketsAvailable : 0;

        printf("%-5d | %-20s | %-20s | %-10s | Rp%-9ld | %-10s | %-10s\n",
               daftarTiket[i].idTiket,
               daftarTiket[i].namaAnggota,
               daftarTiket[i].rute,
               daftarTiket[i].kelas,
               daftarTiket[i].hargaFinal,
               daftarTiket[i].status,
               (daftarTiket[i].isFreeTicket ? "YA" : "TIDAK"));
    }
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("\n**Informasi Trip Count dan Free Ticket (akumulatif)**\n");
    if (jumlahLoyalty == 0) {
        printf("Belum ada data loyalty.\n");
    } else {
        printf("%-20s | %-10s | %-10s\n", "Nama Anggota", "Trip Count", "Tiket Gratis");
        printf("----------------------------------------------------\n");
        for(int i = 0; i < jumlahLoyalty; i++) {
            printf("%-20s | %-10d | %-10d\n", 
                   daftarLoyalty[i].namaAnggota,
                   daftarLoyalty[i].tripCount,
                   daftarLoyalty[i].freeTicketsAvailable);
        }
        printf("----------------------------------------------------\n");
    }
}

// FUNGSI 3: Memperbarui Tiket
void updateTiket() {
    int idToUpdate;
    char newStatus[20];
    char newKomentar[100];
    int tiketDitemukan = 0;

    printf("--- Update Tiket ---\n");
    printf("Masukkan ID Tiket yang akan diupdate: ");
    scanf("%d", &idToUpdate);
    while (getchar() != '\n'); // Membersihkan buffer

    for (int i = 0; i < jumlahTiket; i++) {
        if (daftarTiket[i].idTiket == idToUpdate) {
            tiketDitemukan = 1;

            printf("Tiket ditemukan:\n");
            printf("ID Tiket      : %d\n", daftarTiket[i].idTiket);
            printf("Nama Anggota  : %s\n", daftarTiket[i].namaAnggota);
            printf("Rute          : %s\n", daftarTiket[i].rute);
            printf("Kelas         : %s\n", daftarTiket[i].kelas);
            printf("Harga Final   : Rp%ld\n", daftarTiket[i].hargaFinal);
            printf("Status Saat Ini: %s\n", daftarTiket[i].status);
            printf("Komentar Saat Ini: %s\n", daftarTiket[i].komentar);

            printf("\nMasukkan Status Baru (Terbit/Selesai/Batal): ");
            fgets(newStatus, sizeof(newStatus), stdin);
            newStatus[strcspn(newStatus, "\n")] = 0;
            // Validasi status baru
            if (strcmp(newStatus, "Terbit") != 0 && strcmp(newStatus, "Selesai") != 0 && strcmp(newStatus, "Batal") != 0) {
                printf("Status tidak valid. Mohon pilih 'Terbit', 'Selesai', atau 'Batal'.\n");
                return;
            }
            strcpy(daftarTiket[i].status, newStatus);

            printf("Masukkan Komentar Baru (misal: 'Perjalanan sukses', 'Dibatalkan oleh pelanggan'): ");
            fgets(newKomentar, sizeof(newKomentar), stdin);
            newKomentar[strcspn(newKomentar, "\n")] = 0;
            strcpy(daftarTiket[i].komentar, newKomentar);

            printf("\nTiket ID %d berhasil diperbarui.\n", idToUpdate);
            break;
        }
    }

    if (!tiketDitemukan) {
        printf("Tiket dengan ID %d tidak ditemukan.\n", idToUpdate);
    }
}

// FUNGSI 4: Melihat Daftar Tarif Tiket
void lihatTarifTiket() {
    printf("--- Daftar Tarif Tiket KBT (Medan ke Rute Utama) ---\n");
    printf("%-25s | %-12s | %-12s\n", "Rute", "Ekonomi", "Executive");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < JUMLAH_RUTE; i++) {
        printf("%-25s | Rp%-10d | Rp%-10d\n",
               daftarRute[i].tujuan,
               daftarRute[i].hargaEkonomi,
               daftarRute[i].hargaExecutive);
    }
    printf("------------------------------------------------------\n");
}

// FUNGSI 5: Melihat Status Loyalty Anggota
void lihatStatusLoyalty() {
    char namaAnggotaCari[50];
    printf("--- Lihat Status Loyalty Anggota ---\n");
    printf("Masukkan Nama Anggota: ");
    fgets(namaAnggotaCari, sizeof(namaAnggotaCari), stdin);
    namaAnggotaCari[strcspn(namaAnggotaCari, "\n")] = 0;

    Loyalty* loyaltyAnggota = findLoyalty(namaAnggotaCari);

    if (loyaltyAnggota != NULL) {
        printf("\n--- Status Loyalty untuk %s ---\n", loyaltyAnggota->namaAnggota);
        printf("Jumlah Perjalanan   : %d\n", loyaltyAnggota->tripCount);
        printf("Tiket Gratis Tersedia: %d\n", loyaltyAnggota->freeTicketsAvailable);
        // Menghitung berapa perjalanan lagi untuk tiket gratis berikutnya
        int tripsUntilNextFree = 10 - (loyaltyAnggota->tripCount % 10);
        if (tripsUntilNextFree == 10) tripsUntilNextFree = 0; // Jika sudah pas 10 atau kelipatan
        if (tripsUntilNextFree == 0 && loyaltyAnggota->freeTicketsAvailable > 0) {
            printf("Status Tiket Gratis Berikutnya: Anda memiliki tiket gratis yang siap digunakan!\n");
        } else if (tripsUntilNextFree == 0 && loyaltyAnggota->freeTicketsAvailable == 0) {
             printf("Status Tiket Gratis Berikutnya: Anda baru saja mendapatkan tiket gratis atau belum mencapai ambang berikutnya.\n");
             printf("Diperlukan %d perjalanan lagi untuk tiket gratis berikutnya.\n", 10); // Reset untuk trip berikutnya
        }
        else {
            printf("Status Tiket Gratis Berikutnya: Diperlukan %d perjalanan lagi.\n", tripsUntilNextFree);
        }
        printf("------------------------------------------\n");
    } else {
        printf("Anggota '%s' belum terdaftar dalam sistem loyalty atau belum melakukan perjalanan.\n", namaAnggotaCari);
    }
}
