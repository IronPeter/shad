rm -rf temp
mkdir temp
mkdir temp/data
./a.out < file.list
echo "compressed size "
du -b temp/data