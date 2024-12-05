echo "Begining Test 1: Basic Single Server GET/PUT"
echo "--------------------------------------------------"
./client.sh --put key1 --val value1 
PID1=$!
./client.sh --get key1 
PID2=$!
./client.sh --put key1 --val value2 
PID3=$!
./client.sh --put key2 --val value3 
PID4=$!
./client.sh --put key3 --val value4 
PID5=$!
./client.sh --get key1 
PID6=$!
./client.sh --get key2 
PID7=$!
./client.sh --get key3 
PID8=$!

wait
echo "End Test 1"
echo "--------------------------------------------------"