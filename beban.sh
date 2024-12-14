#!/bin/bash

# Mulai server di background
./chatServer &
SERVER_PID=$!
sleep 2

# Simulasi beban dengan banyak klien
for i in {1..100}; do
  echo "Message $i from client" | nc localhost 8081 &
done

# Tunggu proses selesai
wait

# Periksa apakah server masih berjalan
if kill -0 $SERVER_PID 2>/dev/null; then
  echo "Server berhasil menangani 100 koneksi"
else
  echo "Server crash di bawah beban 100 koneksi"
fi

# Hentikan server
kill $SERVER_PID
