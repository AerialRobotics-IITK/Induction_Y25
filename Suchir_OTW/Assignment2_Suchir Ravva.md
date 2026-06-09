Assignment2_Suchir Ravva

Level0 - bandit0
For level0 to level1 I used the command cat ./- as the password was in dashed file.
Level1 - ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If

For level1 to level2 I used the command cat ./-
Level2 - 263JGJPfgU6LtdEvgfWU1XP5yac29mFx

For level2 to level3 I used the command cat ./"--spaces in this filename--"
Level3 - MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx

For level3 to level4 I used the commands-
cd inhere, to change the working directory to inhere
ls -a, which shows all the files
cat ...Hiding-From-You
Level4 - 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ

For level4 to level5 I used the commands-
cd inhere, to change the working directory to inhere
ls -a, which shows all the files
file ./* which shows the contents in all the files
cat ./-file07, as it was the only file containing ASCII text
Level5 - 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw

For level5 to level6 I used-
cd inhere, to change the working directory to inhere
find . -type f -size 1033c ! -executable, find a file, of size 1033 bytes, not executable.
Level6 - HWasnPhtq9AVKe0dmk45nxy20cvUa6EG

Fro level6 to level7 I used
find / -type f -user bandit7 -group bandit6 -size 33c, to search for a file, owner bandit7, group bandit6, size 33 bytes.
Level7 - morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj

Level7 to level8 -
cat data.txt
whatis strings
strings data.txt | grep millionth
Level 8 - dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc

Level8 to level9 -
cat data.txt
sort data.txt | uniq -c
Level9 - 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM

Level9 to Level10
Strings data.txt | grep “=“
Level10 = FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey

Level10 to level 11
cat data.txt
base64 -d data.txt
Level11 = dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr

Level11 to level12
Cat data.txt
Go to cyberchef apply rot13 -> paste
Level 12 = 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4

Level 12 to level 13
cat data.txt
file data.txt
 mkdir /tmp/suchir (Create a working directory in /tmp and copy data.txt)
cp data.txt /tmp/suchir
cd /tmp/suchir
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
cat dataLevel 13 = FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn

For level13 to level14
ls
ssh -i Ashley.private bandit14@localhost
cat /etc/bandit_pass/bandit14
Level 14 = MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS

For level14 to level15
nc localhost 3000
Then enter the password of the previous level.
Level 15 = 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo

For level15 to level16
man ncat | grep ssl
Enter the password of the previous level.
Level 16 = kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx

For level16 to level17
nmap localhost -p 31000-32000
ncat --ssl localhost 31790
Enter the password

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
vim key
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

chmod 400 key
ssh -i key bandit17@bandit.labs.overthewire.org -p 2220

Level17 to level18-
diff passwords.old passwords.new
Level18 = x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO

Level18 to level19-
ssh bandit18@bandit.labs.overthewire.org -p 2220 
cat readme
level 19 = cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8

level19 to level20-
ssh bandit19@bandit.overthewire.org -p 2220
./bandit20-do cat/etc/bandit_pass/bandit20
Level20 = 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO

level20 to level21-
./suconnect 1234
level21 = EeoULMCra2q0dSkYj561DX7s1CpBuOBt

level21 to level22-
cd /etc/cron.d
ls
cat cronjob_bandit22
cat /usr/bin/cronjob_bandit22.sh
level22 = tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q

level22 to level23-
cd /etc/cron.d
ls
cat cronjob_bandit23
cat /usr/bin/cronjob_bandit23.sh
echo I am user bandit23 | md5sum
cat /tmp/<hash>
cat /tmp/8ca319486bfbbc3663ea0fbe81326349
level23 = 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga

level23 to level24-
cd /etc/cron.d
cat cronjob_bandit24
cat /usr/bin/cronjob_bandit24.sh
cd /tmp
mkdir myhack
cd myhack
nano script.sh
#!/bin/bash
cat /etc/bandit_pass/bandit24 > /tmp/myhack/password.txt
cp script.sh /var/spool/bandit24/foo/
cat /tmp/myhack/password.txt
level24 = gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8

level24 to 25-
level25 = iCi86ttT4KSNe1armKiwbQNmB3YJP3q4

level25 to 26-
level26 = s0773xxkk0MXfdqOfPRVr9L3jJBUOgCZ

level26 to level27-
git clone ssh://bandit27-bit@bandit.labs.overthewire.org:2220/home/bandit27-git/repo
cat README
level 27 = upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB

level27 to level28-
git clone ssh://bandit28-git@bandit.labs.overthewire.org:2220/home/bandit28-git/repo
git log -p
level 28 = Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN

level28 to level29-
git clone ssh://bandit29-git@bandit.labs.overthewire.org:2220/home/bandit29-git/repo
git checkout dev
git log -p
level29 = 4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7

level29 to level30-
git clone ssh://bandit30-git@bandit.labs.overthewire.org:2220/home/bandit30-git/repo
git tag
git show secret
level30 = qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL
