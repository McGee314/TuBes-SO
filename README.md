# Multi-Client Chat Server Project 🚀

Proyek ini mengimplementasikan sebuah server sederhana untuk komunikasi berbasis teks menggunakan protokol TCP/IP. Server dapat menangani banyak klien secara simultan dengan memanfaatkan proses *forking*. Selain itu, server menyimpan log aktivitas dan mendukung fitur seperti pengecekan username unik serta penyiaran pesan kepada semua klien yang terhubung.

---

## Badges

![Language: C](https://img.shields.io/badge/language-C-00599C?style=for-the-badge&logo=c)
![Concurrency: Forking](https://img.shields.io/badge/Concurrency-Forking-00FF00?style=for-the-badge&logo=code)
![Platform: Linux](https://img.shields.io/badge/Platform-Linux-blue?style=for-the-badge)
![TCP/IP: Supported](https://img.shields.io/badge/TCP%2FIP-Supported-lightblue?style=for-the-badge)
![Dependencies: None](https://img.shields.io/badge/Dependencies-None-4B8B3B?style=for-the-badge)

---

## Daftar Isi

1. [Fitur 🌟](#fitur)
2. [Persiapan Awal 🛠️](#persiapan-awal)
   - [Prasyarat ⚙️](#prasyarat)
   - [Instalasi 📀](#instalasi)
3. [Menjalankan Server 🌐](#menjalankan-server)
4. [Struktur File 💻](#struktur-file)
5. [Alur Program 🔄](#alur-program)
6. [Catatan Penting ⚠️](#catatan-penting)
7. [Penulis ✍️](#penulis)

---

## Fitur 🌟

- **Multi-klien**: Mendukung banyak klien secara bersamaan menggunakan *forking*.
- **Penyiaran Pesan**: Pesan dari satu klien dapat disiarkan ke semua klien yang terhubung.
- **Validasi Username**: Memastikan username unik untuk setiap klien.
- **Log Aktivitas**: Semua aktivitas (bergabung, meninggalkan, dan pesan) dicatat dalam file `server.log`.
- **Penanganan Sinyal**: Mendukung penanganan sinyal `SIGCHLD` untuk membersihkan proses anak.

---

## Persiapan Awal 🛠️

### Prasyarat ⚙️

1. Sistem operasi Linux (atau WSL untuk pengguna Windows).
2. GCC Compiler.
3. Akses terminal.

### Instalasi 📀

1. **Clone Repositori:**
   ```bash
   git clone https://github.com/mcgee314/Tubes-SO.git
   cd repository
   ```

2. **Kompilasi Program:**
   ```bash
   gcc -o server server.c
   ```

---

## Menjalankan Server 🌐

1. **Mulai Server:**
   ```bash
   ./server
   ```
   Server akan berjalan pada port 8081 secara default.

2. **Koneksi Klien:**
   Gunakan aplikasi seperti `telnet` untuk menghubungkan klien ke server.
   ```bash
   telnet localhost 8081
   ```

3. **Interaksi Klien:**
   - Saat pertama kali terhubung, klien akan diminta memasukkan username.
   - Setelah itu, klien dapat mengirim pesan yang akan disiarkan ke semua klien lain.

---

## Struktur File 💻

- `server.c`: Berisi implementasi utama server, termasuk penanganan klien, pengelolaan username, dan penyiaran pesan.
- `server.log`: File log yang mencatat aktivitas klien.

---

## Alur Program 🔄

1. **Server Dimulai:**
   - Membuka socket pada port 8081.
   - Mulai mendengarkan koneksi masuk.

2. **Klien Bergabung:**
   - Klien memasukkan username.
   - Server memastikan username unik.
   - Server menambahkan klien ke daftar klien terhubung.

3. **Pengiriman Pesan:**
   - Klien mengirimkan pesan ke server.
   - Server menyiarkan pesan ke semua klien lain.

4. **Klien Keluar:**
   - Server menghapus klien dari daftar terhubung.
   - Mengirimkan notifikasi ke klien lain.

---

## Catatan Penting ⚠️

- Server hanya mendukung komunikasi teks sederhana.
- Pastikan port 8081 tidak digunakan oleh aplikasi lain.
- File log `server.log` akan terus bertambah; hapus secara berkala jika diperlukan.

---

## Penulis ✍️

Nama    :   Muhammad Rafli Fadhilah
            Muhammad Samudera Bagja
Email   :   muhammad.rafli.tif423@polban.ac.id   
            muhammad.samudera.tif423@polban.ac.id

Institusi: POLITEKNIK NEGERI BANDUNG

