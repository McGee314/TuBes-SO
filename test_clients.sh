#!/bin/bash

# Mulai server di background
./chatServer &  # Ganti dengan perintah menjalankan server Anda
SERVER_PID=$!
sleep 2

# Simulasi beberapa klien
for i in {1..5}; do
  echo "Test message from client $i" | nc localhost 8081 &
done

# Simulasi error: Matikan satu klien secara paksa
sleep 2
kill -9 $(pgrep -n nc)  # Matikan proses Netcat terakhir

# Cek apakah server masih berjalan
if kill -0 $SERVER_PID 2>/dev/null; then
  echo "Server tetap berjalan setelah kegagalan klien"
else
  echo "Server berhenti setelah kegagalan klien"
fi

# Hentikan server
kill $SERVER_PID
