echo "Begin Test 2: Basic Multi-Server GET/PUT"
echo "--------------------------------------------------"
./client.sh --put key1 --val value1
PID9=$!
./client.sh --get key1
PID10=$!
./client.sh --put key1 --val value2
PID11=$!
./client.sh --put hello --val value3
PID12=$!
./client.sh --put world --val value4
PID13=$!
./client.sh --get key1
PID14=$!
./client.sh --get hello
PID15=$!
./client.sh --get world
PID16=$!

echo "End Test 2"
echo "--------------------------------------------------"