#!/usr/bin/env bash

#DOWNLOAD_PATH=${HOME}/Downloads/
DOWNLOAD_PATH=/home/brekkit/Desktop/APO_Semprac/mzapo
#NUCLEO_PATH=/media/MBED_A/
MZAPO_PATH=/home/brekkit/Desktop/APO_Semprac/sshfs

#find latest modified BIN file
filename=`ls -tp ${DOWNLOAD_PATH}/main | grep -v /$ | head -1`

#burn it to the mzapo board
cp ${filename} ${MZAPO_PATH}
