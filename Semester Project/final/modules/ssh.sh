scp maleklu6@postel.felk.cvut.cz:/opt/zynq/ssh-connect/mzapo-root-key .
chmod go-rwx mzapo-root-key
ssh-add mzapo-root-key
sshfs root@192.168.1.35:/root/sem_prac /home/brekkit/Desktop/APO_Semprac/sshfs
ssh root@192.168.1.35
