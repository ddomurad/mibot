echo "Initializing environment ... "

cd ./software/

mkdir ./mibotGPIOInterface/build
mkdir ./mibotSensors/build
mkdir ./mibotUtils/build 
mkdir ./ServerLauncher/build
mkdir ./mibotGPS/build
mkdir ./mibotServer/build
mkdir ./mibotStatusStrategy/build
mkdir ./mibotVideoStrategy/build
mkdir ./mibotDriveStrategy/build
mkdir ./mibotInfoStrategy/build
mkdir ./mibotSettingsClient/build
mkdir ./mibotSystemExecute/build

sudo mkdir /usr/local/etc/mi_bot
sudo mkdir /usr/local/lib/mi_bot

sudo chmod 777 /usr/local/etc/mi_bot
sudo chmod 777 /usr/local/lib/mi_bot

sudo chown mibot /usr/local/etc/mi_bot/
sudo chown mibot /usr/local/lib/mi_bot/

echo "export MIBOT_CFG=ENABLE_GPIO" >> ~/.bash_profile

