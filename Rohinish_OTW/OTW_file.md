Level 0
using the format of ssh to login into the game in windows terminal as
ssh username@host -p port
writing the given password – bandit0

level 0 -> 1
code for finding the password
ls                   (list of all the files in home directory)
cat readme          (reading the file - readme)
password - ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If
then use the same procedure as level 0

level 1 -> 2
file name = -
pass - 263JGJPfgU6LtdEvgfWU1XP5yac29mFx
here using command for – named file is as follows cat ./-

level 2 -> 3
file name =  --spaces in this filename--
pass - MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx
here using command for – named file is as follows cat  --“—spaces in  this filename—”

level 3 -> 4
pass - 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ
Commands
Ls
cd inhere
find
cat .
cat ./...Hiding-From-You

level 4 -> 5
pass - 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw
Commands
cd inhere
file ./-* 
cat ./-file07

level 5 -> 6
pass - HWasnPhtq9AVKe0dmk45nxy20cvUa6EG
Commands
find . -type f -size 1033c ! -executable
cat ./inhere/maybehere07/.file2

level 6 -> 7
pass - morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj
Commands
find / -type f -user bandit7 -group bandit6 -size 33c 
cat /var/lib/dpkg/info/bandit7.password

level 7 -> 8
pass - dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc
Commands
ls -alps
ls data.txt
strings data.txt
strings data.txt | grep “millionth”

level 8 -> 9
pass - 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM
Commands
cat data.txt
sort data.txt | uniq -c

level 9 -> 10
pass - FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey
Commands
Strings data.txt | grep “=”

level 10 -> 11
pass - dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr
Commands
cat data.txt
base64 -d data.txt

level 11 -> 12
pass - 7x16WNeHIi5YkIhWsfFIqoognUTyd4
Commands
cat data.txt
echo "7k16JArUVv5LxVuJfsSVdbbtaHGlw9D4" | tr 'A-Za-z' 'N
-ZA-Mn-za-m'

level 12 -> 13
pass - FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn
Commands
# 2. Inspect the hex dump file (shown earlier in series)
cat data.txt
file data.txt
 mkdir /tmp/rohit (Create a working directory in /tmp and copy data.txt)
cp data.txt /tmp/rohit
cd /tmp/rohit
ls
xxd -r data.txt > data (Reverse the hex dump with xxd)
ls
file data     
mv data file.gz (Decompress gzip file)
gzip -d file.gz
ls
file file        # shows bzip2 compressed data
mv file file.bz2  ( Decompress bzip2 file)
bzip2 -d file.bz2
ls
file file       
mv file file.gz (Decompress gzip again)
gzip -d file.gz
ls
file file       
mv file file.tar ( Extract tar archive )
tar -xvf file.tar
ls
file data5.bin   
file data5.bin   
rm file.tar
mv data5.bin data.tar
tar -xvf data.tar
ls
file data6.bin   # new extracted file

file data6.bin   ( Decompress bzip2 again)
mv data6.bin data.bzip2
bzip2 -d data.bzip2
ls
file data    
mv data data.tar ( Extract tar again)
tar -xvf data.tar
ls
file data8.bin   
file data8.bin   ( Decompress gzip again)
mv data8.bin data.gz
gzip -d data.gz
ls
file data     
cat data

Level 13->14
Pass - MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS
Ls
scp -P 2220 bandit13@bandit.labs.overthewire.org:~/sshkey.private . 
ssh -i sshkey.private -p 2220 bandit14@bandit.labs.overthewire.org
cat /etc/bandit_pass/bandit14

Level 14->15
Pass - MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS
Ls
scp -P 2220 bandit13@bandit.labs.overthewire.org:~/sshkey.private . 
ssh -i sshkey.private -p 2220 bandit14@bandit.labs.overthewire.org
cat /etc/bandit_pass/bandit14
level 15-> 16
pass - 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo
nc localhost 30000
level 16->17
pass - kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx
ncat –ssl localhost 30001
level 17->18	
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAvmOkuifmMg6HL2YPIOjon6iWfbp7c3jx34YkYWqUH57SUdyJ
imZzeyGC0gtZPGujUSxiJSWI/oTqexh+cAMTSMlOJf7+BrJObArnxd9Y7YT2bRPQ
Ja6Lzb558YW3FZl87ORiO+rW4LCDCNd2lUvLE/GL2GWyuKN0K5iCd5TbtJzEkQTu
DSt2mcNn4rhAL+JFr56o4T6z8WWAW18BR6yGrMq7Q/kALHYW3OekePQAzL0VUYbW
JGTi65CxbCnzc/w4+mqQyvmzpWtMAzJTzAzQxNbkR2MBGySxDLrjg0LWN6sK7wNX
x0YVztz/zbIkPjfkU1jHS+9EbVNj+D1XFOJuaQIDAQABAoIBABagpxpM1aoLWfvD
KHcj10nqcoBc4oE11aFYQwik7xfW+24pRNuDE6SFthOar69jp5RlLwD1NhPx3iBl
J9nOM8OJ0VToum43UOS8YxF8WwhXriYGnc1sskbwpXOUDc9uX4+UESzH22P29ovd
d8WErY0gPxun8pbJLmxkAtWNhpMvfe0050vk9TL5wqbu9AlbssgTcCXkMQnPw9nC
YNN6DDP2lbcBrvgT9YCNL6C+ZKufD52yOQ9qOkwFTEQpjtF4uNtJom+asvlpmS8A
vLY9r60wYSvmZhNqBUrj7lyCtXMIu1kkd4w7F77k+DjHoAXyxcUp1DGL51sOmama
+TOWWgECgYEA8JtPxP0GRJ+IQkX262jM3dEIkza8ky5moIwUqYdsx0NxHgRRhORT
8c8hAuRBb2G82so8vUHk/fur85OEfc9TncnCY2crpoqsghifKLxrLgtT+qDpfZnx
SatLdt8GfQ85yA7hnWWJ2MxF3NaeSDm75Lsm+tBbAiyc9P2jGRNtMSkCgYEAypHd
HCctNi/FwjulhttFx/rHYKhLidZDFYeiE/v45bN4yFm8x7R/b0iE7KaszX+Exdvt
SghaTdcG0Knyw1bpJVyusavPzpaJMjdJ6tcFhVAbAjm7enCIvGCSx+X3l5SiWg0A
R57hJglezIiVjv3aGwHwvlZvtszK6zV6oXFAu0ECgYAbjo46T4hyP5tJi93V5HDi
Ttiek7xRVxUl+iU7rWkGAXFpMLFteQEsRr7PJ/lemmEY5eTDAFMLy9FL2m9oQWCg
R8VdwSk8r9FGLS+9aKcV5PI/WEKlwgXinB3OhYimtiG2Cg5JCqIZFHxD6MjEGOiu
L8ktHMPvodBwNsSBULpG0QKBgBAplTfC1HOnWiMGOU3KPwYWt0O6CdTkmJOmL8Ni
blh9elyZ9FsGxsgtRBXRsqXuz7wtsQAgLHxbdLq/ZJQ7YfzOKU4ZxEnabvXnvWkU
YOdjHdSOoKvDQNWu6ucyLRAWFuISeXw9a/9p7ftpxm0TSgyvmfLF2MIAEwyzRqaM
77pBAoGAMmjmIJdjp+Ez8duyn3ieo36yrttF5NSsJLAbxFpdlc1gvtGCWW+9Cq0b
dxviW8+TFVEBl1O4f7HVm6EpTscdDxU+bCXWkfjuRb7Dy9GOtt9JPsX8MBTakzh3
vBgsyi/sN3RqRBcGU40fOoZyfAMT8s1m/uYv52O6IgeuZ/ujbjY=
-----END RSA PRIVATE KEY-----
nmap localhost -p 31000-32000
apply ncat –ssl localhost for the provided files
level 18->19
pass - x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO (diff passwords.old passwords.new)
Level 19->20: cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8 (passed "cat readme" along with the ssh command only to tunnel through the .bashrc logout)

qLevel 20->21: 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO (This was really crazy! bash: ./bandit20-do cat /etc/bandit_pass/bandit20)
          This bandit20-do file had UID for the user bandit20 which works like a uniform that allows to access things that are permitted to bandit20
          
Level 21->22: EeoULMCra2q0dSkYj561DX7s1CpBuOBt (sent data to a random port via nc; bash: echo "0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO" | nc -lp 1234 &)
          -l -> listen (wait); -p -> (specifying port); & -> sends it to background
          then ./suconnect 1234 --> this connects to the same port gets the password sent through the port via echo, verifies it and returns the next password to the terminal
          
Level 22->23: tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q (cron is like a butler who autommates the stuff and doesn't do anything until specified)
          ls /etc/cron.d/ -> checked the present files
          cat /etc/cron.d/cronjob_bandit22
          @reboot bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null
          * * * * * bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null (* * * * * -> tells to apply this every minute)
          "/usr/bin/cronjob_bandit22.sh" -> is the script that cron is running
          bash: cat /usr/bin/cronjob_bandit22.sh
          #!/bin/bash
          chmod 644 /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv
          cat /etc/bandit_pass/bandit22 > /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv (This /tmp/.... is the file where the cron is storing the password to)
          finally: cat t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv
    
Level 23-> 24: 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga (with basic above steps only:)
          Got $mytarget = echo I am user $myname | md5sum | cut -d ' ' -f 1 ($myname is a placeholder)
          Hash returned
          cat hash
          
Level 24->25: gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 (Ran a shellscript file that transferred the cronjob_bandit24 tasks to our own txt file)

Level 25->26: iCi86ttT4KSNe1armKiwbQNmB3YJP3q4 (Created own scrip file in which looping was done to try every single permutation while being connected)

Level 26->27: *check bandit26key.txt (has the ssh key) -> explored a whole new weird method: shrunk down the terminal and entered
          :set shell= /bin/bash
          :shell
          and on maximising i got into bandit26

Level 27->28: upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB (Nothing new, a previous tactic only)

Level 28->29: Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN (git cloning to the local machine)

Level 28->29: 4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7 (git log history) using git log -p
Level 29->30: 
•	cd repo
•	ls
cat readme
git branch
git branch -r
git checkout dev
cat readme








