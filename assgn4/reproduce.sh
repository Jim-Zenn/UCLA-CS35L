#! /bin/sh
cd ~
touch -d '1918-11-11 11:00 GMT' wwi-armistice
touch now
sleep 1
touch now1
TZ=UTC0 ls -lt --full-time wwi-armistice now now1

