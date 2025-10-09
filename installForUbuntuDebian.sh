sudo apt-get install build-essential cmake libsdl3-dev libopengl-dev libboost-all-dev libmysqlcppconn-dev wget git -y
wget https://dev.mysql.com/get/mysql-apt-config_0.8.34-1_all.deb
sudo dpkg -i mysql-apt-config_0.8.34-1_all.deb
sudo apt-get update
git clone https://github.com/DanielMartensson/GoobySoft.git
cd GoobySoft
cmake -B build .
cmake --build build
