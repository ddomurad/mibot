
echo "Download code"
git pull origin master

cd ./software/

echo "Build Utils ... "
cd ./mibotUtils/build/
qmake ../mibUtils/
make
make install
cd ../..

echo "Build SettingsClient ... "
cd ./mibotSettingsClient/build/
qmake ../mibSettingsClient/
make
make install
cd ../..

echo "Build Sesnors"
cd ./mibotSensors/build
qmake ../mibSensors
make
make install
cd ../..

echo "Build GPIO Interface ... "
cd ./mibotGPIOInterface/build/
qmake ../mibGPIOInterface/
make
make install
cd ../..

echo "Build Server ... "
cd ./mibotServer/build/
qmake ../mibServer/
make
make install
cd ../..


echo "Build DriveStrategy ..."
cd ./mibotDriveStrategy/build/
qmake ../mibDriveStrategy/
make
make install
cd ../..

echo "Build StatusStrategy ..."
cd ./mibotStatusStrategy/build/
qmake ../mibStatusStrategy/
make
make install
cd ../..

echo "Build InfoStrategy"
cd ./mibotInfoStrategy/build/
qmake ../mibInfoStrategy/
make 
make install
cd ../..

echo "Build VideoStrategy"
cd ./mibotVideoStrategy/build/
qmake ../mibVideoStrategy/
make
make install
cd ../..

echo "Build Server Starter ... "
cd ./ServerLauncher/build/
qmake ../ServerLauncher/
make
make install
cd ../..
