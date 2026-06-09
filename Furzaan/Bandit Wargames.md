# Bandit Wargames solutions

#      To start level 0, typed ssh bandit0@bandit.labs.overthewire.org \-p 2220 in the terminal. 

#      LEVEL 0 : Password for level 0 was bandit0 (on instructions page). Used ls to see all the files present. Used “cat readme” to get pass for level 1 from readme.

LEVEL 1 : Now used ssh [bandit1@bandit.labs.overthewire.org](mailto:bandit1@bandit.labs.overthewire.org) \-p 2220 to move to level1. ls showed a file “-”. Used cat ./- to see the contents in the file. Got a password for level 2 from there.

LEVEL 2 : ls showed a file “--spaces in the filename–”. Used cat ./\* in order to extract the contents of the file.

LEVEL 3 : Now moving to level 3, the files here were hidden, so I had to use ls \-a which showed something like “ . .. …Hiding-From-You”.  Found the pass for level 4 in the file named “...Hiding-From-You”.

LEVEL 4 : Level 4 also had a directory named “inhere”. “cd inhere”, then ls showed 10 files like “-file00”, ….. , “-file09”. Used a command “file ./\*” which showed which file was a human-readable file.Used cat ./”-file07” to get password for level 5\.   
LEVEL 5 : For level 5, the inhere directory contained many sub-directories. Used the specifications given about the file and find command to get the required file and eventually found the password. 

LEVEL 6 : For level 6, used the find command with all the specs given about the pass word and eventually made it to level 7\. 

LEVEL 7 : Level 7 required the grep command. Used grep “millionth” data.txt which gave the line containing the password for level 8\. 

LEVEL 8 : In level 8, to extract the unique line from the file data.txt, used the uniq command but before that required to sort the contents. Exact command : sort data.txt | uniq \-u

LEVEL 9 : used the strings command which gave the strings present in the data.txt file omitting all binary code. Then, used grep “==” to find the password in the file. 

LEVEL 10 : Needed to convert base64 encoded data into readable string. Used “base64 \-d data.txt”

LEVEL 11 : Used the “tr” command in order to move all letters by 13 alphabets. Exact command :-  cat data.txt | tr ‘A-Za-z’ ‘N-ZA-Mn-za-m’

LEVEL 12 :  mkdir /tmp/work71/ created a new directory. cp data.txt  /tmp/work71 copied the file data.txt into the new directory.xxd \-r data.txt \> data converted the hex dump file into compressed file. Used file command to check the type of file and accordingly  
gunzip [filename.gz](http://filename.gz)  
bunzip2 filename.bz2  
tar \-xf filename

LEVEL 14 : used nc command to connect to localhost port 30000 and pasted the previous pass to get the one for next level.

LEVEL 15 : Used   
openssl s\_client \-connect localhost:30001 to connect to localhost port 30001 through an encrypted network.

LEVEL 16 : nmap \-p 31000-32000 localhost helped to findout the open ports.   
openssl s\_client \-connect localhost:\[PORT\_NUMBER\] \-quiet helped to get the RSA key for logging into level 17\.

LEVEL 17 : Used diff passwords.old passwords.new to see the changes made and got pass for level 18\.

LEVEL 18 : Used ssh bandit18@bandit.labs.overthewire.org \-p 2220 cat readme to read the file in level 18\.

LEVEL 19 :   
./bandit20-do cat /etc/bandit\_pass/bandit20  
allowed the user to act as user bandit20-do for this specific command and hence cat /etc/bandit\_pass/bandit20 gave the contents of the file.

LEVEL 20 : echo “\[Password to enter level20\]” | nc –l \-p 9000 & connects to the port and then ./suconnect followed by the pass for level 20 gave the pass for level 21\.

LEVEL 21 :   
ls /etc/cron.d/ showed the files that were executed regularly.   
cat /etc/cron.d/cronjob\_bandit22 gave details about the execution and its frequency.   
cat /usr/bin/cronjob\_bandit22.sh gives the details about the execution process.   
cat /tmp/t7O6vS3H19XnS3S7uO6vS3H19XnS3S7u  
where /tmp/t7O6vS3H19XnS3S7uO6vS3H19XnS3S7u  
Is the public file in which the password was copied into from the protected file.

LEVEL 22 \- ls/etc/cron.d/ showed the files that were being executed by cron.   
Cat /usr/bin/cronjob\_bandit22.sh gave detail about the execution.  
echo I am user bandit 23 | md5sum | cut \-d ‘ ‘ \-f 1  
gave the public file name into which the password was being copied.

Level 23 \- mkdir \-p /tmp/wrk780  
chmod 777 /tmp/wrk780  
created  a directory and made it readable, write-able and executable by anyone.   
touch /tmp/work780/pass.txt  
chmod 666 /tmp/work780/pass.txt  
creates a file and makes it readable as well as writable.  
echo '\#\!/bin/bash' \> /tmp/wrk780/solve.sh  
echo "cat /etc/bandit\_pass/bandit24 \> /tmp/work123/pass.txt" \>\> /tmp/work123/[solve.sh](http://solve.sh)  
This asks the computer to take the secret password and write it into /tmp/wrk780/pass.txt  
chmod \+x /tmp/wrk780/[solve.sh](http://solve.sh)  
Makes the script runnable.  
cp /tmp/wrk780solve.sh /var/spool/bandit24/foo/  
cat /tmp/wrk780/pass.txt  
This gives the password for level 24\.

Level 24 \- mkdir \-p /tmp/task504  
   cd /tmp/task504  
These 2 command s created a file named task504.  
for i in {0000..9999}; do echo "\[pass for level 24\] $i" \>\> combinations.txt; done wrote all the 10000 PINs possible.(cat combinations.txt; sleep 2\) | nc localhost 30002 | grep \-v "Wrong"  
This command fed all the combinations into the localhost and filtered out the 10000 lines of output to show the correct pass for level 25\.

