#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/select.h>
#include <time.h>

#define PORT 8081
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define LOG_BUFFER_SIZE 1100  // Ukuran buffer untuk log

int clients[MAX_CLIENTS]; // Array untuk menyimpan socket klien
int client_count = 0;     // Jumlah klien yang terhubung
char client_names[MAX_CLIENTS][50]; // Array untuk menyimpan nama klien

// Fungsi untuk mendapatkan waktu saat ini
void get_current_time(char *time_str, size_t len) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(time_str, len, "[%Y-%m-%d %H:%M:%S]", tm_info);
}

// Broadcast pesan ke semua klien
void broadcast_message(const char *message, int sender_fd, const char *sender_name) {
    char full_message[BUFFER_SIZE];
    snprintf(full_message, sizeof(full_message), "[%s]: %s", sender_name, message);

    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_fd) { // Jangan kirim ke pengirim
            send(clients[i], full_message, strlen(full_message), 0);
        }
    }
}

// Hapus klien dari daftar
void remove_client(int client_fd) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == client_fd) {
            clients[i] = clients[client_count - 1]; // Pindahkan klien terakhir ke posisi ini
            client_count--;
            break;
        }
    }
}

// Fungsi untuk mencatat log
void log_message(const char *message) {
    char time_str[50];
    get_current_time(time_str, sizeof(time_str));

    // Pastikan pesan log tidak melebihi buffer
    char log_message[LOG_BUFFER_SIZE];
    snprintf(log_message, LOG_BUFFER_SIZE, "%s %s", time_str, message);

    // Buka file log dengan ekstensi .log
    FILE *log_file = fopen("server_log.log", "a");
    if (log_file) {
        fprintf(log_file, "%s\n", log_message);

        // Jika pesan berasal dari server, tambahkan baris kosong
        if (strstr(message, "socket fd") || strstr(message, "Chat Server") || strstr(message, "terputus")) {
            fprintf(log_file, "\n");
        }

        fclose(log_file);
    } else {
        perror("Gagal membuka file log");
    }
}

int main() {
    int server_fd, new_socket, max_sd, activity;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    // Membuat socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket gagal dibuat");
        exit(EXIT_FAILURE);
    }

    // Bind socket ke port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind gagal");
        exit(EXIT_FAILURE);
    }

    // Listening
    if (listen(server_fd, 10) < 0) {
        perror("Listen gagal");
        exit(EXIT_FAILURE);
    }
    printf("Chat Server berjalan di port %d...\n", PORT);

    log_message("Chat Server berjalan di port 8081...");

    while (1) {
        // Reset set file descriptor
        FD_ZERO(&readfds);

        // Tambahkan server socket ke set
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Tambahkan semua klien ke set
        for (int i = 0; i < client_count; i++) {
            if (clients[i] > 0) {
                FD_SET(clients[i], &readfds);
            }
            if (clients[i] > max_sd) {
                max_sd = clients[i];
            }
        }

        // Tunggu aktivitas di salah satu socket
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("Select error");
        }

        // Jika ada koneksi baru
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("Accept gagal");
                continue;
            }

            printf("Klien baru terhubung: socket fd %d\n", new_socket);

            // Tentukan nama klien
            char client_name[50];
            snprintf(client_name, sizeof(client_name), "Klien%d", client_count + 1);  // Nama berdasarkan urutan koneksi
            strcpy(client_names[client_count], client_name);

            // Tambahkan klien baru ke daftar
            if (client_count < MAX_CLIENTS) {
                clients[client_count++] = new_socket;

                // Log klien yang terhubung
                char log_msg[LOG_BUFFER_SIZE];
                snprintf(log_msg, LOG_BUFFER_SIZE, "Klien baru terhubung: socket fd %d, Nama: %s", new_socket, client_name);
                log_message(log_msg);

                char welcome_message[BUFFER_SIZE];
                snprintf(welcome_message, sizeof(welcome_message), "Selamat datang di chat server, %s!\n", client_name);
                send(new_socket, welcome_message, strlen(welcome_message), 0);
            } else {
                char *error_message = "Server penuh. Coba lagi nanti.\n";
                send(new_socket, error_message, strlen(error_message), 0);
                close(new_socket);
            }
        }

        // Periksa aktivitas pada socket klien
        for (int i = 0; i < client_count; i++) {
            int client_fd = clients[i];

            if (FD_ISSET(client_fd, &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_read = read(client_fd, buffer, BUFFER_SIZE);

                if (bytes_read <= 0) {
                    // Klien terputus
                    printf("Klien socket fd %d terputus\n", client_fd);
                    // Log klien yang keluar
                    char log_msg[LOG_BUFFER_SIZE];
                    snprintf(log_msg, LOG_BUFFER_SIZE, "Klien socket fd %d terputus", client_fd);
                    log_message(log_msg);
                    close(client_fd);
                    remove_client(client_fd);
                } else {
                    // Kirim pesan dengan nama klien
                    printf("Pesan diterima dari [%s]: %s", client_names[i], buffer);

                    // Log pesan yang diterima
                    char log_msg[LOG_BUFFER_SIZE];
                    snprintf(log_msg, LOG_BUFFER_SIZE, "Pesan diterima dari [%s]: %s", client_names[i], buffer);
                    log_message(log_msg);

                    broadcast_message(buffer, client_fd, client_names[i]);
                }
            }
        }
    }

    return 0;
}