Level0: used ssh bandit0@[host name] -p [port] and the entered the password to clear this level

Level1: pw: ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If accesed using cat [filename]

Level2: pw: 263JGJPfgU6LtdEvgfWU1XP5yac29mFx accessed using cat ./-

Level3: pw: MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx accessed using cat ./--[then pressed tab]

Level4: pw: 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ accessed using using ls -a

Level5: pw: 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw used file ./-* to check content of each file then accessed the one with ASCII text

Level6: pw: HWasnPhtq9AVKe0dmk45nxy20cvUa6EG used command find inhere -type f -size 1033c ! -executable -exec file {} + | grep text

Level7: pw: morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj used command find / -type f -user bandit7 -group bandit6 -size 33c 2>/dev/null

Level8: pw: dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc using grep "millionth" data.txt

Level9: pw: 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM using sort data.txt | unq -u

Level10: pw: FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey using strings data.txt | grep "="

Level11: pw: dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr using base64 -d data.txt

Level12: pw: 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4 using cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m'

Level13: pw: FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn using tar, gunzip and buzip2

Level14: pw: MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS cat sshkey.private exit echo "-key-" > private.key chmod 600 private.key, ssh -i private.key bandit14@bandit.labs.overthewire.org -p 2220 then cat /etc/bandit_pass/bandit14

Level15: pw: 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo cat /etc/bandit_pass/bandit14 | nc localhost 30000 (nc sends data to port)

Level16: pw: kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx openssl s_client -connect localhost:30001

Level17: pw: 390zFj2NETFVZkqYw8UEFdN6h40oGVtT nmap -sV -p 31000-32000 localhost, openssl s_client -connect localhost:31790, then same as level14

Level18: pw: x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO diff passwords.old passwords.new

Level19: pw: cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8  ssh bandit18@bandit.labs.overthewire.org -p 2220 "cat readme"

Level20: pw: 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO ~/bandit20-do cat bandit20

Level21: pw: EeoULMCra2q0dSkYj561DX7s1CpBuOBt  echo "0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO" | nc -l -p 1234 & , then ./suconnect 1234

Level22 pw: tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q  cat cronjob_bandit22(find whats running), cat /usr/bin/cronjob_bandit22.sh then check the temp file being written to with cat /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv

Level23: pw: 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga  cat cronjob_bandit23, then cat /usr/bin/cronjob_bandit23.sh. Reading shell file and figuring out file location using echo I am user bandit23 | md5sum | cut -d ' ' -f 1, then cat /tmp/8ca319486bfbbc3663ea0fbe81326349

Level24: pw:  gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8  echo -e '#!/bin/bash\ncat /etc/bandit_pass/bandit24 > /tmp/lessgo' > /tmp/firstshell.sh(write shell file that copies to tmp, then copy to correct folder executed by cron),cp /tmp/firstshell.sh /var/spool/bandit24/foo/myscript.sh
