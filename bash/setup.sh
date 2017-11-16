#!/usr/bin

# create file
sudo touch /var/www/html/xmas-lights.php

# change mode
sudo chmod 666 /var/www/html/xmas-lights.php

# add crontab jobs
# run every minute to clear, then once lunchtime alert 
(
  echo "* * * * mon-fri sh ~/github/ESP8266/bash/lights-off.sh" > crontab
  echo "40 12 * * mon-fri sh ~/github/ESP8266/bash/lights-on.sh" > crontab
) | crontab -u pi -
