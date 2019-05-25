#!/bin/bash

init()
{
    DEVICE=/dev/ttyUSB0
    BAUD_RATE=115200
    
    DIRECTORY=$(cd `dirname $0` && pwd)
    HOME=$DIRECTORY/..
    LOG=log

    TEST=test
    EMPTY=empty.c
    MEM=task.c

    FREERTOS=freeRTOS
    FREERTOS_MAIN=main/main.c
    FREERTOS_MAKE="make"
    FREERTOS_FLASH="make flash"

    MONGOOSE=mongoose
    MONGOOSE_MAIN=src/main.c
    MONGOOSE_MAKE="mos build --platform esp32"
    MONGOOSE_FLASH="mos flash"

    exec 3>&-
    exec 3<>$DEVICE
}

logging()
{
    file=$1
    stty -F $DEVICE $BAUD_RATE raw

    read line <&3
    until [[ $line == ---* || $line == Error:* ]]
    do
        read line <&3
    done

    if [[ $line == Error:* ]] ; then
        echo ${line::-1} > $file
        return
    fi

    rm -f $file
    echo ${line::-1} >> $file    # ---Test name---
    read line <&3
    echo ${line::-1} >> $file    # CPU frequency
    read line <&3
    echo ${line::-1} >> $file    # Tick rate
    read line <&3
    echo ${line::-1} >> $file    # Iterations or Average

    if [[ $line != Iterations:* ]] ; then
        return
    fi

    iter=${line:12}
    iter=$(echo $iter | tr -d "\r\n")

    read line <&3
    echo ${line::-1} >> $file    # Average
    read line <&3
    echo ${line::-1} >> $file    # Variance

    for ((i = 1; i <= iter; i++))
    do
        read line <&3
        until [[ $line == \#* ]]
        do
            echo ${line::-1} >> $file
            read line <&3
        done

        echo ${line::-1} >> $file
    done
}

system_test()
{
    mem_func=$1
    if [[ $mem_func == *mongoose ]] ; then
        sys_dir=$MONGOOSE
        sys_main=$MONGOOSE_MAIN
        sys_make=$MONGOOSE_MAKE
        sys_flash=$MONGOOSE_FLASH
    else
        sys_dir=$FREERTOS
        sys_main=$FREERTOS_MAIN
        sys_make=$FREERTOS_MAKE
        sys_flash=$FREERTOS_FLASH
    fi


    cd $sys_dir

    for i in `ls $TEST/*.c`
    do
        name=${i:${#TEST}+1}
        name=${name::-2}
        
        cp -r $i $sys_main
        $sys_make

        if [[ $i != $TEST/$EMPTY && $i != $TEST/$MEM ]] ; then
            $sys_flash
            logging $HOME/$LOG/$sys_dir/$name.log

            while read -t 1 <&3
            do
                read -t 0 <&3
            done
        else
            $mem_func $HOME/$LOG/$sys_dir/$name.log
        fi
    done

    cp -r $TEST/$EMPTY $sys_main
    cd ..
}

mem()
{
    file=$1
    byte=0
    
    for i in `find -name *.bin`
    do
        bin=`du -b $i`
        bin=${bin%$i}
        byte=$(($byte + ${bin::-1}))
    done
    
    echo "$byte bytes" > $file
}

mem_mongoose()
{
    FW=build/fw.zip
    ZIP=build/zip
    
    unzip -o $FW -d $ZIP
    
    mem $1
}

main()
{
    init

    cd $HOME
    mkdir -p $LOG

    mkdir -p $LOG/$FREERTOS
    system_test mem

    mkdir -p $LOG/$MONGOOSE
    system_test mem_mongoose
}

main $@
