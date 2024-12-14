#!/bin/bash

# Mulai server di background
./chatServer &
SERVER_PID=$!
sleep 2

# Kirim data yang sangat besar untuk menguji buffer overflow
BIG_DATA=$(head -c 2000 </dev/urandom | tr -dc 'a-zA-Z0-9')
echo "$BIG_DATA" | nc localhost 8081

# Cek apakah server masih berjalan setelah menerima input besar
if kill -0 $SERVER_PID 2>/dev/null; then
  echo "Server tetap berjalan setelah menerima input besar"
else
  echo "Server crash akibat input besar"
fi

# Hentikan server
kill $SERVER_PID
