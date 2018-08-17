#o!/bin/sh

project_home=$(pwd)
install_path=$project_home/build/FPDemo

mkdir -p $install_path
rm -r $install_path/bin
rm -r $install_path/conf
rm -r $install_path/logs
mkdir -p $install_path/bin
mkdir -p $install_path/logs

echo "compile core modules"
cd $project_home/core
make
cd $project_home

echo "compile cache module"
cd $project_home/cache
make
cd $project_home

echo "compile daemon server"
cd $project_home/daemon
make
cp daemon_server $install_path/bin
cp *.sh $install_path/bin

echo "compile node server"
cd $project_home/server
make
cp node_server $install_path/bin
cp -r $project_home/conf $install_path

echo "compile test"
cd $project_home/test
make

echo "done."
