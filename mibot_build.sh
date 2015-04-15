
echo "Download code"
git pull origin master

cd ./software/

echo "Build Utils ... "
cd ./mibotUtils/build/
qmake ../mibUtils/
make
make install
cd ../..

echo "Build Access ... "
cd ./mibotAccess/build/
qmake ../mibAccess/
make
make install
cd ../..

echo "Build GPIO Interface ... "
cd ./mibotGPIOInterface/build/
qmake ../mibGPIOInterface/
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

echo "Build Server ... "
cd ./mibotServer/build/
qmake ../mibServer/
make
make install
cd ../..

echo "Build Server Starter ... "
cd ./UtilsTests/build/
qmake ../UtilsTests/
make
make install
cd ../..
