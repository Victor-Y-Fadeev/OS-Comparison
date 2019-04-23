#!/bin/bash

DIR="/opt/Espressif"

# Install Prerequisites
sudo apt-get -y install gcc git wget make minicom libncurses-dev flex bison gperf python python-pip python-setuptools python-serial python-cryptography python-future python-pyparsing python-pyelftools

# Toolchain Setup
sudo mkdir -p $DIR
sudo chown $USER:root $DIR

if [ `uname -m` = "x86_64" ]
then
	wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz -O $DIR/xtensa-esp32-elf.tar.gz
else
	wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-80-g6c4433a-5.2.0.tar.gz -O $DIR/xtensa-esp32-elf.tar.gz
fi

tar -xzf $DIR/xtensa-esp32-elf.tar.gz -C $DIR
rm $DIR/xtensa-esp32-elf.tar.gz

# Set Environment Variables
echo "export PATH=\"$DIR/xtensa-esp32-elf/bin:\$PATH\"" >> ~/.profile

# Permission issues /dev/ttyUSB0
sudo usermod -a -G dialout $USER

# Get ESP-IDF
git clone --recursive https://github.com/espressif/esp-idf $DIR/esp-idf

# Add IDF_PATH to User Profile
echo "export IDF_PATH=$DIR/esp-idf" >> ~/.profile

# Install the Required Python Packages
python -m pip install --user -r $DIR/esp-idf/requirements.txt
