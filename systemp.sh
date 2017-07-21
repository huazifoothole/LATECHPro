cat ./sensor.txt | grep temp1 |awk '{print $2}'|cut -c 2-3
