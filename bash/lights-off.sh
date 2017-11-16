#!/usr/bin

# delay 10 seconds as crontab job runs on and off in parallel
# so there is no race condition between states
sleep 10

echo "off" > /var/www/html/xmas-lights.php
