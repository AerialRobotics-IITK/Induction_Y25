// Aryaman Singh
Level 0:
ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If
Level 1:
Just had to learn how to access dashed files : 263JGJPfgU6LtdEvgfWU1XP5yac29mFx
Level 2:
Nothing really, just use quotes : MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx
Level 3:
Just use ls -a : 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ
Level 4:
Just use file ./* followed by cat : file ./*: 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw
Level 5:
Just use find ./* -type f -size 1033c -readable ! -executable : HWasnPhtq9AVKe0dmk45nxy20cvUa6EG
Level 6:
Had to google how to use /dev/null except for that pretty simple : morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj
Level 7: 
Just use grep "millionth" data.txt to go line by line : dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc
Level 8: 
As uniq requires the data to be sorted; use the command : sort data.txt before using uniq : 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM
Level 9:
Use strings to extract human readable text and pipe it over to grep "=" : FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey
Level 10:
Just base64 : dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr
Level 11:
Frankly had no idea how to solve this, and used GPT to figure out how to traverse 13 characters to find the password : 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4
Level 12:
Basically had to do a lot of "majdoori" constantly using file data_file.txt to find it's compression type then using gzip,bzip2 or tar to decompress; initially had to use xxd for hexadump to eventually at the end get data8 in ASCII text form : FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn
Level 13:
Just had to copy the sshkey and chmod to change permissions of the made key : MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS
Level 14:
Just had to learn how to use netcat to access a port on a local network : 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo
Level 15:
Had to use openssl for the specific type of encryption; openssl s_client -connect localhost:30001 : kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx
Level 16:
Kind of just had to learn how to nmap to find which servers are listening then I manually checked each using openssl s_client -connect localhost:port to find the correct port to be 31790(had to use -quiet as I was getting a KEYUPDATE response whenever I put in the password), at the end I got the rsa key:
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
Level 17:
I initially tried using sort to find some way to then maybe use uniq or grep to find the solution but that didn't end up working out, so just used diff to find the solution : x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO
Level 18:
Just had to run the command required directly through ssh to get the password info just before getting logged out through the protocol in .bashrc : cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8
Level 19:
Basically had to realise that running ./bandit20-do allows us to act as bandit20; which in turn would have access to the password to the bandit20 ip which is stored in /etc/bandit_pass/bandit20 : 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO
Level 20:
Okay, so in this we had to use netcat as we had previously used in level-14 to create a listening server which will listen for any connections coming in, and as the password for level-20 has to match with the incoming signal we pipe echo 'the_password' to this server to get the next password, then it is as simple as using ./suconnect with our port to get the password back : EeoULMCra2q0dSkYj561DX7s1CpBuOBt
Level 21:
In this problem, we first of all had to find out what even are cronjobs, so like told by the PS, they're programs which run on regular intevals doing "something", first of all we run a file check on /etc/cron.d/ to find which cronjob might contain the job which would give us the password, and coincidentally there is a cronjob convenientally named cronjob_bandit22 so we just make use of it to thenceforth discover the tmp directory that has the password : tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q
Level 22:
This is very similar to level-21, where we had to read /usr/bin/cronjob_bandit23.sh to see what whahast was the cronjob being executed, then we had to essentially execute the same logic as the bash script, replacing $whoami with bandit23 to then get the hash called as $mytarget, and then just use cat /tmp/$mytarget noticing that the script just copies the password to this file : 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga
Level 23:
For this level, I kept getting no output from my bash script, to be completely honest, I still don't know what I changed such that it ended up working, functionally my code that didn't work and the one that did work should have been the same, the only difference being using chmod 777 and chmod o+w and chmod o+x, this shouldn't make a difference, coming to my approach it was again to just go through the cronjob, then to create a bash script that would take the password generated by this cronjob and copying that into my own .txt file : gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8
Level 24:
This was quite easy compared to the brainfu the last level was, just had to google how to run a for loop in a bash script and it's quite easy moving forward from what we learned from previous levels(netcat and basic bash scripting) : iCi86ttT4KSNe1armKiwbQNmB3YJP3q4
Level 25:
Okay so first of all we just had to get this RSA key from the main directory in bandit25, now to be honest, I was not able to figure out any way to go forward, but the solution is genuinely crazy, so basically we look for bandit26 in /etc/passwd; this leads us to a file in /usr/bin called "showtext"; upon further inspection using cat we find that it is a bash script ending with more ~/text.txt; this is the reason why whenever we try to login using the ssh key the connection immediately closes, as basically when the terminal window is above a certain size the text immediately shows and the connection doesn't go to a mode with which we can interact to find the password; BASICALLY, WE HAVE TO PHYSICALLY DECREASE THE SIZE OF OUR TERMINAL WINDOW! That is crazy! Then it's quite easy to just find the password after typing v to use vim : s0772xxkk0MXfdqOfPRVr9L3jJBUOgCZ
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEApis2AuoooEqeYWamtwX2k5z9uU1Afl2F8VyXQqbv/LTrIwdW
pTfaeRHXzr0Y0a5Oe3GB/+W2+PReif+bPZlzTY1XFwpk+DiHk1kmL0moEW8HJuT9
/5XbnpjSzn0eEAfFax2OcopjrzVqdBJQerkj0puv3UXY07AskgkyD5XepwGAlJOG
xZsMq1oZqQ0W29aBtfykuGie2bxroRjuAPrYM4o3MMmtlNE5fC4G9Ihq0eq73MDi
1ze6d2jIGce873qxn308BA2qhRPJNEbnPev5gI+5tU+UxebW8KLbk0EhoXB953Ix
3lgOIrT9Y6skRjsMSFmC6WN/O7ovu8QzGqxdywIDAQABAoIBAAaXoETtVT9GtpHW
qLaKHgYtLEO1tOFOhInWyolyZgL4inuRRva3CIvVEWK6TcnDyIlNL4MfcerehwGi
il4fQFvLR7E6UFcopvhJiSJHIcvPQ9FfNFR3dYcNOQ/IFvE73bEqMwSISPwiel6w
e1DjF3C7jHaS1s9PJfWFN982aublL/yLbJP+ou3ifdljS7QzjWZA8NRiMwmBGPIh
Yq8weR3jIVQl3ndEYxO7Cr/wXXebZwlP6CPZb67rBy0jg+366mxQbDZIwZYEaUME
zY5izFclr/kKj4s7NTRkC76Yx+rTNP5+BX+JT+rgz5aoQq8ghMw43NYwxjXym/MX
c8X8g0ECgYEA1crBUAR1gSkM+5mGjjoFLJKrFP+IhUHFh25qGI4Dcxxh1f3M53le
wF1rkp5SJnHRFm9IW3gM1JoF0PQxI5aXHRGHphwPeKnsQ/xQBRWCeYpqTme9amJV
tD3aDHkpIhYxkNxqol5gDCAt6tdFSxqPaNfdfsfaAOXiKGrQESUjIBcCgYEAxvmI
2ROJsBXaiM4Iyg9hUpjZIn8TW2UlH76pojFG6/KBd1NcnW3fu0ZUU790wAu7QbbU
i7pieeqCqSYcZsmkhnOvbdx54A6NNCR2btc+si6pDOe1jdsGdXISDRHFb9QxjZCj
6xzWMNvb5n1yUb9w9nfN1PZzATfUsOV+Fy8CbG0CgYEAifkTLwfhqZyLk2huTSWm
pzB0ltWfDpj22MNqVzR3h3d+sHLeJVjPzIe9396rF8KGdNsWsGlWpnJMZKDjgZsz
JQBmMc6UMYRARVP1dIKANN4eY0FSHfEebHcqXLho0mXOUTXe37DWfZza5V9Oify3
JquBd8uUptW1Ue41H4t/ErsCgYEArc5FYtF1QXIlfcDz3oUGz16itUZpgzlb71nd
1cbTm8EupCwWR5I1j+IEQU+JTUQyI1nwWcnKwZI+5kBbKNJUu/mLsRyY/UXYxEZh
ibrNklm94373kV1US/0DlZUDcQba7jz9Yp/C3dT/RlwoIw5mP3UxQCizFspNKOSe
euPeaxUCgYEAntklXwBbokgdDup/u/3ms5Lb/bm22zDOCg2HrlWQCqKEkWkAO6R5
/Wwyqhp/wTl8VXjxWo+W+DmewGdPHGQQ5fFdqgpuQpGUq24YZS8m66v5ANBwd76t
IZdtF5HXs2S5CADTwniUS5mX1HO9l5gUkk+h0cH5JnPtsMCnAUM+BRY=
-----END RSA PRIVATE KEY-----
Level 27:
The password to level 27 can straight up be accessed in level-26 using bandit27-do in the directory we access after the whole RSA+Windows Resizing stuff, thenceforth we use that to access /etc/bandit_pass/bandit27 to get the password : upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB.
To get the password to level 28 was pretty easy as it was just the use of git clone, basically the same way as downloading any package on Arch : Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN
Level 28:
Again the same thing as before, except we had to use git log and git show to see the logs of the README.md file to see it's commit history to find the password before it was scrubbed : 4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7
Level 29:
Okay the final level, this is where the Learn Git Branching Game came in clutch, we had to basically see if README.md existed in any other branch, the branches being dev, master, and spoilts-dev; the password existed in the dev branch giving us : qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL