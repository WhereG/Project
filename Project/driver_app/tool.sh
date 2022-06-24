#!/bin/sh

server_function(){
    sudo make clean                           
    sudo make 
    sudo mknod /dev/demo_char c 236 0
    sudo chmod 666 /dev/demo_char
    sudo rmmod ThunderSoft_Project.ko
    sudo insmod ThunderSoft_Project.ko
    watch "dmesg | tail -20" 
    return $?
}
cat /dev/null > log
if [ "$1" == "build" ]
then
    sudo make clean
    sudo make
elif [ "$1" == "run" ]
then
    sudo insmod ThunderSoft_Project.ko
    watch "dmesg | tail -20"
elif [ "$1" == "server" ]
then
    server_function
    if [ $? -eq 0 ]; then
        echo "Server Successful"
    else
        times = 0
        echo "Server Failed and Run Five Times Next..."
        while ($times < 5)
        do
            server_function 2>&1 | tee -a log
            if [ $? -eq 0]; then
                break
            fi
            $times  = $times + 1
        done
    fi
elif [ "$1" == "exit" ]
then 
    sudo make clean 
    sudo rmmod ThunderSoft_Project.ko
else 
    echo "Error Command:$1"
fi

