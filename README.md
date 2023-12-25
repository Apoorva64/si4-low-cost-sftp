sudo apt-get install libssl-dev
sudo apt install build-essential
sudo apt-get -y install cmake
sudo apt-get install gdb
sudo apt-get install libiddawc-dev
sudo apt install libyder-dev


# When launching server be sure that your time is synced with NTP server
```shell
sudo apt install systemd-timesyncd
sudo timedatectl set-ntp true
sudo timedatectl status
```

USER: user1

PASS: user1