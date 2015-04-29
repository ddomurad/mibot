echo "Initializing environment ... "

cd ./software/

mkdir ./mibotAccess/build
mkdir ./mibotDriveStrategy/build
mkdir ./mibotGPIOInterface/build
mkdir ./mibotInfoStrategy/build
mkdir ./mibotServer/build
mkdir ./mibotStatusStrategy/build
mkdir ./mibotUtils/build 
mkdir ./ServerLauncher/build

mkdir /usr/local/etc/mi_bot
mkdir /usr/local/lib/mi_bot

chmod 777 /usr/local/etc/mi_bot
chmod 777 /usr/local/lib/mi_bot

chown mibot /usr/local/etc/mi_bot/
chown mibot /usr/local/lib/mi_bot/

echo "export MIBOT_CFG=ENABLE_GPIO" >> ~/.bash_profile

