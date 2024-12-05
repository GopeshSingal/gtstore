./client.sh --put key1 --val value1
./client.sh --put key1 --val newvalue1
./client.sh --put key1 --val newnewvalue1

read
# Waits for user to kill server based on PID

./client.sh --get key1
> key1, newvalue1, server...

# do gets for the other keys that were mapped to serverX, they should be on some other node now with the same value.