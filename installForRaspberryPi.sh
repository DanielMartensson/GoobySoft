sudo apt-get install build-essential cmake libsdl2-dev libopengl-dev libmodbus-dev libboost-all-dev git wget -y
wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.1.0-linux-glibc2.28-aarch64.tar.gz
tar -xvf mysql-connector-c++-8.1.0-linux-glibc2.28-aarch64.tar.gz
sudo mkdir /usr/include/mysql-cppconn-8/
sudo cp -r mysql-connector-c++-8.1.0-linux-glibc2.28-aarch64/include/* /usr/include/mysql-cppconn-8/
sudo cp -r mysql-connector-c++-8.1.0-linux-glibc2.28-aarch64/lib64/* /lib/aarch64-linux-gnu/
git clone https://github.com/DanielMartensson/GoobySoft.git
cd GoobySoft
cmake -B build .
cmake --build build
