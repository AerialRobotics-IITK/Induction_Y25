level 0 - ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If - cat

level 1 - 263JGJPfgU6LtdEvgfWU1XP5yac29mFx - cat ./-

level 2 - MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx - cat ./"--spaces in this filename--"

level 3 - 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ - ls -a to find hidden file then cat ./"...Hiding-From-You"

level 4 - 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw - go inside inhere with cd, use file ./file0x from 1 to 9 to find human readable file, then use cat ./file07

level 5 - HWasnPhtq9AVKe0dmk45nxy20cvUa6EG - cd inhere, find . -size 1033c ! -executable then cat ./maybehere07/.file2

level 6 - morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj - find / -user bandit7 -group bandit6 -size 33c 2>/dev/null (last part to avoid seeing permission denied) , then cat /var/lib/dpkg/info/bandit7.password

level 7 - dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc - grep "millionth" data.txt

level 8 - 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM - sort data.txt | uniq -u (first sort, then use | to give input to uniq which with -u prints only unique)

level 9 - FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey - strings data.txt | grep "=" (extract human readable data, then search near "=")

level 10 - dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr - base64 -d data.txt (-d to decode)

level 11 - 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4 - tr 'A-Za-z' 'N-ZA-Mn-za-m' < data.txt (this is rot13 cipher code, < feeds file directly to tr)

level 12 - FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn
Use the following repeatedly-
file decoded - to check the file compression type
rename file appropriately using mv decoded decoded.(gz/tar/bz2)
gunzip decoded.gz / tar -xf decoded.tar / bunzip2 decoded.bz2
Repeatedly apply, then cat for final text.

level 13 - MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS
cat sshkey.private
exit
echo "-key-" > private.key
chmod 600 private.key (other will get permission denied due to file being too unprotected)
ssh -i private.key bandit14@bandit.labs.overthewire.org -p 2220
then cat /etc/bandit_pass/bandit14

level 14 - 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo - cat /etc/bandit_pass/bandit14 | nc localhost 30000 (nc sends data to port)

level 15 - kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx - openssl s_client -connect localhost:30001

level 16 - 390zFj2NETFVZkqYw8UEFdN6h40oGVtT- nmap -sV -p 31000-32000 localhost, openssl s_client -connect localhost:31790, then same as level 13

level 17 - x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO - diff passwords.old passwords.new

level 18 - cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8 - ssh bandit18@bandit.labs.overthewire.org -p 2220 "cat readme"

level 19 - 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO- ~/bandit20-do cat bandit20

level 20 - EeoULMCra2q0dSkYj561DX7s1CpBuOBt - echo "0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO" | nc -l -p 1234 & , then ./suconnect 1234

level 21 - tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q - cat cronjob_bandit22(find whats running), cat /usr/bin/cronjob_bandit22.sh then check the temp file being written to with cat /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv

level 22 - 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga - cat cronjob_bandit23, then cat /usr/bin/cronjob_bandit23.sh. Reading shell file and figuring out file location using echo I am user bandit23 | md5sum | cut -d ' ' -f 1, then cat /tmp/8ca319486bfbbc3663ea0fbe81326349

level 23 - gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 - echo -e '#!/bin/bash\ncat /etc/bandit_pass/bandit24 > /tmp/lessgo' > /tmp/firstshell.sh(write shell file that copies to tmp, then copy to correct folder executed by cron),cp /tmp/firstshell.sh /var/spool/bandit24/foo/myscript.sh

level 24 - iCi86ttT4KSNe1armKiwbQNmB3YJP3q4 - for i in {0000..9999}; do echo "gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 $i"; done | nc localhost 30002

level 25 - s0773xxkk0MXfdqOfPRVr9L3jJBUOgCZ - bypass more shell with small terminal height + v

level 26 - upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB ./bandit27-do cat /etc/bandit_pass/bandit27

level 27 - Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN - git clone ssh://bandit27-git@bandit.labs.overthewire.org:2220/home/bandit27-git/repo, then cat README

level 28 - 4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7 - git clone ssh://bandit28-git@bandit.labs.overthewire.org:2220/home/bandit28-git/repo, then git log -p

level 29 - qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL - git clone ssh://bandit29-git@bandit.labs.overthewire.org:2220/home/bandit29-git/repo, then git checkout dev, then git log -p

level 30 - fb5S2xb7bRyFmAvQYQGEqsbhVyJqhnDy - git clone ssh://bandit30-git@bandit.labs.overthewire.org:2220/home/bandit30-git/repo, then git tag, git show secret
