echo "Begining Test 1: Basic Single Server GET/PUT"
echo "--------------------------------------------------"
./start_service.sh --nodes 1 --rep 1 &
SERVICE_PID=$!
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

kill $SERVICE_PID
pkill storage 2>/dev/null
pkill manager 2>/dev/null
wait
echo "End Test 1"
echo "--------------------------------------------------"



echo "Begin Test 2: Basic Multi-Server GET/PUT"
echo "--------------------------------------------------"
./start_service.sh --nodes 5 --rep 3 &
SERVICE_PID=$!
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

kill $SERVICE_PID
pkill storage 2>/dev/null
pkill manager 2>/dev/null
wait
echo "End Test 2"
echo "--------------------------------------------------"