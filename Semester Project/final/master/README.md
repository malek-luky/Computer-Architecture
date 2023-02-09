# Semester Project APO
The main task of the project is to try how to use DC motor on mzapo board.

## Getting Started
To run the project you will need the mzapo board from the subject Computer
Architecture, which is lectured at CTU in Prague, or you can use the CTU
account, connect to postel.felk.cvut.cz and watch the boards via live stream.

## Prerequisites
The only necessary package is the ARM cross compiler toolchain. For Ubuntu
you can download the package and support programs by typing:
```
sudo apt install crossbuild-essential-armhf
```
If you still receive make: arm-linux-gnueabihf-gcc: Command not found try the following
```
sudo apt-get install libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev
```
For more information follow [these instruction](https://cw.fel.cvut.cz/wiki/courses/b35apo/documentation/mz_apo-howto/start).

## Connect via live stream
The first option how to run the program is to use the live stream on the address
https://youtu.be/QhbVtUUyPsA and connect to the SSH server on postel.felk.cvut.cz.
You can use git and follow only the first two steps on how to connect to the board and
clone the repository on the mzapo board. But if you want to copy it
directly from your PC without git, follow these instructions.

#### First part
Firstly you need to connect to the board and create a personal folder,
```
ssh cvut_login@postel.felk.cvut.cz
```

After you successfully connect to the server, choose one of these boards and
create your personal folder (not necessarry, but highly recommended):
```
ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.203
ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.212
ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.207
ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.127
ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.211
ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.204
ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.213
```
```
mkdir folder_name
```

To copy the files from local PC via PC in the lab to mzapo board do the followings
in the separate command window. The first one leaves open.
```
mkdir $HOME/Desktop/mzapo
sshfs username@postel.felk.cvut.cz:$HOME/Desktop/mzapo $HOME/Desktop/mzapo
```

#### Second part
Now we are successfully connected to PC in the laboratory. Now we must connect
this PC with the mzapo board
```
ssh cvut_login@postel.felk.cvut.cz
mkdir $HOME/Desktop/mzapo
eval `ssh-agent -s` //evaluate the ssh client
ssh-add /opt/zynq/ssh-connect/mzapo-root-key //add the private key
ssh-add-mzapo-key //command for quick key loading
sshfs root@192.168.202.xxx:/root/username $HOME/Desktop/mzapo //no password needed
```
Now you are successfully connected, and you can transfer the files from your
computer directly to mzapo. You need to place your data to the mounted folder
which you can find on the left bar in files.

#### Third part
When you are finished you want to exit the board and cancel the sshfs
connection, do the following:
1) type exit in the cmd window with the board
2) write sudo umount $HOME/Desktop/mzapo in the same window
3) write exit again
4) now type sudo umount $HOME/Desktop/mzapo

## Connect to the physical board
There are two possible ways how to do it. You can use the serial port or you
please connect to the modem and access it via ssh as a local server.

#### Serial port
To access the port use gtkterm or tio -b 115200 /dev/ttyUSB0 from the terminal. In
gtkterm set up the following parameters:
```
Device: /dev/ttyUSB0
Baudrate: 115200
Zero parity, one stop bit
Login: root
Password: mzAPO35
```

#### SSH
To use the SSH it is necessary to download the mzapo-root key from postel.felk.cvut.cz
```
scp username@postel.felk.cvut.cz:/opt/zynq/ssh-connect/mzapo-root-key .
chmod go-rwx mzapo-root-key
ssh-add mzapo-root-key
```
Now the key is added and you can connect to the board using IP address, which you will
see on display. It is written in binary in format
```
firstline.secondline.thirdline.forthline
For example: 192.168.7.101
```
To connect to the board via ssh, use the following command.
```
ssh root@192.168.xxx.xxx
```
When you are connected to the board, it is a good idea to create your personal
folder.
```
mkdir folder_name
```
Now I recommend creating a desktop folder which is used for this connection.
```
mkdir $HOME/Desktop/mzapo
```
To start the sshfs connection to transfer data between computer and mzapo
board tun this command:
```
sshfs root@192.168.xxx.xxx:/root/folder_name $HOME/Desktop/mzapo
```

## Running the programs
To run the programs, use the ssh connection to connect to the board.
```
ssh root@192.168.xxx.xxx
```
If you want to check whether the connection is successful, using this command,
the board should blink a few times. If it does, everything is set up correctly.
```
/opt/zynq/tests/test-spi
```
At this moment, find the compiled program you want to run. For example for
main it would be:
```
./main
```

## Built With
* [VS Code](https://code.visualstudio.com/) - The text editor used

## Authors
* **Lukas Malek** - *Initial work* - [CTU](https://gitlab.fel.cvut.cz/maleklu6)
* **Katerina Hobzova** - *Initial Work* - [CTU](https://gitlab.fel.cvut.cz/hobzokat)

See also the list of [contributors](https://gitlab.fel.cvut.cz/maleklu6/apo-semester-project/-/graphs/master)
who participated in this project.

## Copyright
Copyright 2017 by Pavel Pisa
e-mail:   pisa@cmp.felk.cvut.cz
homepage: http://cmp.felk.cvut.cz/~pisa
company:  http://www.pikron.com/
license:  any combination of GPL, LGPL, MPL or BSD licenses

## Acknowledgments
* Pavel Pisa - our tutor and lecturer - [CTU, PiKRON](https://gitlab.fel.cvut.cz/pisa)
* Petr Porazil - hardware desing - [PiKRON](https://gitlab.com/pikron/projects/mz_apo/microzed_apo)
