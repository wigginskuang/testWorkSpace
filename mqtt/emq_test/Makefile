gcc mosquitto_sub.c -lmosquitto -o sub
gcc mosquitto_pub.c -lmosquitto -o pub

nohup ./sub >> sub.out 2>&1 &
./pub

clean:
	rm sub pub
