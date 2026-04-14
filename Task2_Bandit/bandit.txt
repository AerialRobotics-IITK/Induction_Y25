0. ssh into bandit.overthewire.org with username pass both as bandit0, use cat to read the pass from readme file
ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If

1. use cat ./- to get pass, as cat - will fail as - is used for passing cmd line flags, . is for pwd
263JGJPfgU6LtdEvgfWU1XP5yac29mFx

2. same as before but use "" to enclose it making it a single str
MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx

3. cd into inhere then ls -a shows all files including hidden, in linux a file with name beginning with . is hidden
2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ

4. cd into inhere then ls shows 10 files, file cmd shows filetype and metadata, file ./* gives this for all the files in pwd, only one file has ascii text, pass was in that
4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw

5. given that the target file is 1033 bytes in size, cd into inhere, use find . -type f -size 1033c to get the file, f flag for type gives only files skipping links, dir, pass was in ./maybehere07/.file2
HWasnPhtq9AVKe0dmk45nxy20cvUa6EG

6. use find with size flag with file path of root dir, one of these is named bandit7 password which contains the password [/var/lib/dpkg/info/bandit7.password]
morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj

7. pipe the output of cat data.txt into grep with "millionth"
dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc

8. pipe the output of cat data.txt into sort then uniq -u, cause uniq only removes adjacent duplicates
4CKMh1JI91bUIZZPXDqGanal4xvAg0JM

9. file data.txt shows that data.txt is actually just binary data not any standard file type, strings on it gives the ascii strings only, pipe it into grep to search for ===
FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey

10. use base64 with -d flag to decode data.txt
dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr

11. use tr to translate A-Za-z to N-ZA-Mn-za-m which is basically rot13
7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4

12. use xxd to convert hex into binary file, use file cmd to check file extract it accordingly using gzip, bzip2, tar, finally you'll get a text file with password, you have to do this in /tmp cause you don't have write permission anywhere else
FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn

13. there's a ssh private key, download it using scp to local dir, then use it to ssh as bandit14, password can now be read from /etc/bandit_pass/bandit14
MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS

14. use nc to echo password to the listener on the same server on port 30000, it'll give the next pass in resp
8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo

15. use openssl to connect with ssl encyrption on the same server on port 30001, type the password to, it'll give the next one
kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx

16. use nmap to scan the server in given port range, use service detection to find ssl ones
bandit16@bandit:~$ nmap localhost -p 31000-32000 -sV
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-04-12 10:26 UTC
Nmap scan report for localhost (127.0.0.1)
Host is up (0.00011s latency).
Not shown: 996 closed tcp ports (conn-refused)
PORT      STATE SERVICE     VERSION
31046/tcp open  echo
31518/tcp open  ssl/echo
31691/tcp open  echo
31790/tcp open  ssl/unknown
31960/tcp open  echo

31790 does not simply echo so that's the port, use openssl to connect and type the pass, it'll give rsa private key to ssh as bandit17, once logged in get the pass from /etc/bandit_pass/bandit17
EReVavePLFHtFlFsjn3hyzMlvSuSAcRD

17. use diff to show the different line which is the password
x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO

18. .bashrc will log out everytime will login, so just add the cmd to ssh cmd, which will exec and return the output so
└─[$]> ssh bandit18@bandit.labs.overthewire.org -p 2220 cat readme
cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8 

19. exec the binary tells that it runs any arg as diff user, doing whoami using it shows that it execs the cmd as bandit20 so just cat the pass from /etc/bandit_pass using it
0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO

20. set up a nc listner on some random port that echos passoword back when connected, and specify this port as arg to the given binary
EeoULMCra2q0dSkYj561DX7s1CpBuOBt

21. look into the cronjob dir, the bandit22 cronjob is executing a bash script, check the script, it saves the pass of 22 into some file in tmp dir, and also gives read access of it to everyone, so read it to get the pass
tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q

22. read the bash scipt as above, it computes md5 hash of some text and saves the pass in tmp dir with filename as that hash, compute hash using the same cmd in the script just replacing $myname with bandit23, then read the file
0Zf11ioIjMVN551jX3CmStKLYqjk54Ga

23. read the bash script as above, now it execs all the scripts in /var/spool/bandit24/foo as bandit24 with some timeout and deletes the script, so write a script that gets the pass of 24 and puts it in a dir where it has write access and we have read access, which is tmp.
script used - 
#!/bin/bash
cat /etc/bandit_pass/bandit24 > /tmp/sid1803/password24
you'll also give perms to exec and write perms to your script, and dir under tmp, for bandit24 to be able to exec and copy pass in your dir
gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8

24. just make a script to bruteforce all pins
#!/bin/bash
for i in {0000..9999}; do echo "gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 $i"; done | nc localhost 30002

iCi86ttT4KSNe1armKiwbQNmB3YJP3q4 

25. checking /etc/passwd tells that shell for bandit26 is not bash but some showtext script, checking it shows its using more to show text and exit, more will get activated incase the text overflows, to force the text to overflow, shrink terminal size. once in more, use v to enter vim, now set shell to /bin/bash and get the shell using :set shell=/bin/bash and then :shell, then get the pass from /etc/bandit_pass_bandit26
s0773xxkk0MXfdqOfPRVr9L3jJBUOgCZ

26. get shell similarly like above, then use ls to discover a binary file executing args as bandit27, so give arg as cat /etc/bandit_pass/bandit27
upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB

27. clone the repo the passwd is in readme file
Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN

28. clone the repo, check find hashes of prev commits using git log then checkout those to find passwd
4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7

29. clone the repo, check branches, pass is in the commit history of dev branch
qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL

30. clone the repo, check tags, passwd is in the taginfo of secret tag
fb5S2xb7bRyFmAvQYQGEqsbhVyJqhnDy
