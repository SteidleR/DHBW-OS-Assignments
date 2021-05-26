#!/bin/bash

quit_count=0

quit_program() {
    ((quit_count+=1))
    if [ $quit_count -eq 1 ]; then
        echo "Once more to quit the program..."
    elif [ $quit_count -eq 2 ]; then
        echo "Ok, I'll quit. Bye"
        exit 0
    fi
}

# Signal Handler
# SIGKILL and SIGSTOP should not be trapped! [see IEEE standard]
trap '{ echo "User tried to stopped the program with 'CTRL+C'. Please kill the program with 'kill -3 $$'"; }' SIGINT   # SIGINT --> CTRL+C
trap quit_program SIGQUIT

# show pid to kill
echo "PID: $$"

# while loop to continue in bg
while :
do
    sleep 1
done