./zhiduo

while [ 1 ] ; do
    sleep 3

process_num=`ps -ef | grep "zhiduo" | grep -v "grep" | wc -l`

echo $process_num

if [ $process_num -ne 4 ];
then

process_id=`ps -ef | grep "zhiduo" | grep -v "grep" | awk '{print $2}'`

echo $process_id
            
kill -9 $process_id

./zhiduo

fi

done
        












