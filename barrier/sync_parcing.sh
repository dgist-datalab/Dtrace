# bin/bash

echo "fsync data pasing start!"
cat $1 | grep "D\|C\|I\|Q\|G" >> $2
sort $2 >> $2
echo "fsync data pasing end!"

