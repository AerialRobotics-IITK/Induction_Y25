## Level 0
 ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If

ran `ssh bandit0@bandit.overthewire.org -p 2220`, then just `cat README`



## Level 1
 263JGJPfgU6LtdEvgfWU1XP5yac29mFx

use `cat ./-` to open the - file, as just `cat -` would wait for input from keyboard



## Level 2
 MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx

file was called "spaces in this filename", used `cat 'spaces in this filename'` or tab complete handles it too



## Level 3
 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ

cd into inhere/ then `ls -a` to see dotfiles, found `...Hiding-From-You`



## Level 4
 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw

used `find ./*` to check all the files, -file07 was the only one that wasn't binary garbage



## Level 5
a HWasnPhtq9AVKe0dmk45nxy20cvUa6EG

`find . -type f -size 1033c ! -executable` then cd maybehere07/ and `cat ./.file2`



## Level 6
 morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj

`find / -user bandit7 -group bandit6 -size 33c 2>/dev/null`, had to add the 2>/dev/null to get rid of all the permission denied noise



## Level 7
 dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc

used `grep millionth data.txt`



## Level 8
 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM

`sort data.txt | uniq -u` - uniq -u only shows lines that appear exactly once, everything else appeared 10 times



## Level 9
 FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey

binary file so ran `strings data.txt | grep '==='` to pull out readable strings and find the one with === markers



## Level 10
 dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr

`base64 -d data.txt`, decodes base64 text



## Level 11
 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4

`cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m'` - rot13, shifts each letter by 13



## Level 12
 FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn

file was a hex dump, had to reverse it first with `xxd -r data.txt > out`. then ran `file out` to check what it was - kept being gzip, bzip2, tar over and over. made a temp dir in /tmp to work in. cycle was: check with file, rename with right extension, decompress, repeat. eventually got a plain text file with the password. 



## Level 13
 fGrHPx402xGC7U7rXKDaxiWFTOiF0ENq

used `ssh -i sshkey.private bandit14@localhost -p 2220` to log in as bandit14 directly



## Level 14
 MU4VWeTyJk8ROof1sCVBP5t5ZhKIoAfp

now logged in as bandit14. the password for this level is in `/etc/bandit_pass/bandit14` - can finally read it. then had to send it to port 30000 on localhost: `echo "thecurrentpassword" | nc localhost 30000`, it gave back the next password



## Level 15
 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo

same idea as 14 but port 30001 needs SSL. used `openssl s_client -connect localhost:30001`, pasted the password when connected, got next one back.


## Level 16
 (gives you an SSH private key instead)

had to find which port between 31000-32000 actually speaks SSL and has the next thing. ran `nmap -sV localhost -p 31000-32000` to scan. one of them responds with the private key instead of a password - save that key to /tmp/mykey, chmod 400 it, then use it to ssh into bandit17



## Level 17
 x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO

two files: passwords.old and passwords.new. just did `diff passwords.old passwords.new`, the line that's only in .new is the password



## Level 18
 cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8

logging in just kicks you right back out, .bashrc has an exit in it. trick is to pass the command directly through ssh without starting a shell: `ssh bandit18@bandit.overthewire.org -p 2220 cat readme`



## Level 19
 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO

there's a binary called bandit20-do with the setuid bit set, meaning it runs as bandit20. so just did `./bandit20-do cat /etc/bandit_pass/bandit20` and it read the file as that user



## Level 20
 EeoULMCra2q0dSkYj561DX7s1CpBuOBt

binary connects back to a port you specify and expects the current password, then gives the next one. had to run a listener in the background first: `echo "currentpassword" | nc -l -p 1234 &` then run `./suconnect 1234` - it connected, verified the password and sent back the new one



## Level 21
 tRae0UfB9v0UzbCdn9cY0gQnds9GmuBy

looked in `/etc/cron.d/` to see what's running, found a cronjob for bandit22. catted the script it was running, it was writing the password to a file in /tmp. just catted that file



## Level 22
 0Zf11ioIjMVN551jX3CmStKLYqjk54Pb

same deal, check cron.d. the script this time makes the filename using `md5sum` of a string with the username in it. ran that same md5sum command myself to figure out the exact filename in /tmp, then catted it



## Level 23
 gcUqRCutJbe7t7QvKXWD4l8PEhNNpMd5

cron runs everything dropped into /var/spool/bandit24/. wrote a little shell script that copies bandit24's password to a file i can read in /tmp, dropped it in that folder, waited a minute for cron to pick it up, then catted my output file. had to make sure the tmp file was world-writable beforehand



## Level 24
 iCi86ttT4KSNe1armKiwbQNmB3YJP3q4

daemon on port 30002 wants the password + a 4-digit pin. wrote a loop to generate all combinations from 0000-9999 into a file then `cat pins.txt | nc localhost 30002` and grepped for the line that wasn't "Wrong"



## Level 25
 s0773xxkk0MXfdqkPjAVVG5p89XbR3Rv

logging into bandit26 opens more instead of bash (set in /etc/passwd). shrunk the terminal window small so more actually pauses, then hit v to open vi, then in vi ran `:set shell=/bin/bash` and `:shell` to drop into a real shell



## Level 26
 eb0MDedznCnmXWfaqj7XsJt7tH60MgHR

once in the shell from level 25, there's another bandit27-do binary just like level 19. ran `./bandit27-do cat /etc/bandit_pass/bandit27`



## Level 27
 Yz9IpL06sBMX1LsDidu7kJ1xnz8RQ66n

made a dir in /tmp, then `git clone ssh://bandit27-git@localhost:2220/home/bandit27-git/repo`, cd in and cat README



## Level 28
 4pT1t5DENaYuqnqvadYs1oE4HS169sQa

cloned the repo, README had the password redacted. ran `git log` to check history, saw a previous commit. did `git show HEAD~1` or `git log -p` to see the diff - the password was there before it got blanked out



## Level 29
 qp7k7tYstETXTHBNMdqvTMrgzFJJLMhf

cloned repo, README said no passwords in prod. ran `git branch -a` to see all branches including remote ones. checked out the dev branch with `git checkout dev`, the README there had the actual password



## Level 30
 fb5S2xb7bRyFmAvQYQGEqsbhVyJqhnDy

cloned, README was empty basically. tried git log, git branch - nothing obvious. then tried `git tag` and there was one called "secret". ran `git show secret` and there was the password
