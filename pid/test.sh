####### nohup ./protect_process &  ######这个是启动本脚本时作为守护进程，即使退出当前终端，进程也不会被关掉
###cmd是被监视的进程的启动命令###


cmd="./pid"

##检查进程是否存在的函数


CheckProcess(){
    if [ "$1" = "" ];
    then
        return 1
    fi

    process_num=`ps -ef | grep "$1" | grep -v "grep" | wc -l`

    if [ $process_num -eq 1 ];
    then
        return 1
    else
        return 0
    fi
}

##强制关闭具有相同命令的进程，防止重复

KillProcessWithParams(){
    if [ "$1" = "" ];
    then
        return 1
    fi
   
    process_id=`ps -ef | grep "$1" | grep -v "grep" | awk '{print $2}'`
#    echo $process_id
    if [ "$process_id" = "" ];
    then
        return 1
    fi
#    #echo $process_id
    kill -9 $process_id

}



##开始死循环，不断重复检查
while [ 1 ] ; do
    sleep 3
        CheckProcess "${cmd}"
        check_ret=$?
        if [ $check_ret -eq 0 ];
        then
            #echo "killall -9 "${cmd}" no start "
            KillProcessWithParams "${cmd}"
            ${cmd}
        fi
done
