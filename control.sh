#!/usr/bin/env bash

ADDR=$1
PORT=1112

HEADER="Press up and down arrow keys:"

clear
echo $HEADER

while true;
do
    read -r -sn1 cmd

    case "$cmd" in
        A)
            clear
            echo $HEADER
            echo -e "going up!"
            `echo -e "\033[A" | nc -c ${ADDR} ${PORT}`
            ;;
        B)
            clear
            echo $HEADER
            echo -e "going down!"
            `echo -e "\033[B" | nc -c ${ADDR} ${PORT}`
            ;;
    esac
done

