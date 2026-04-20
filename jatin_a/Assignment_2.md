OvertheWire game

SSH: Basically a  way to access some other computer, using an unsecured network, which is encrypted.
     In the game, for each level we will log in to some other device, and solve on its linux terminal.
     For this we require: username = banditX
                          server = bandit.labs.overthewire.org
                          port = 2220
                          password = changes every level

command: "ssh banditX@bandit.labs.overthewire.org -p 2220"
'exit' to return to our own system
for level0, tped the above command, entered password, and got into the other system.

LEVEL0:
cat readme
pass: ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If

LEVEL1:
We face issues handling the file whose name starts with '-'.
To fix this we have two options:
1. use '--' before filename. '--' is used to indicate end of command, everthing written after '--' is filename.
2. give absolute path of file. eg:cat ./-filename (The ./ refers to the current directory).

cat ./-
pass: 263JGJPfgU6LtdEvgfWU1XP5yac29mFx

LEVEL2:
The filenames now have spaces, the computer will read it to be multiple filenames
to fix:
1. write the filename inside "".
2. put '\' before each space. eg: cat spaces\ in\ this\ filename

cat ./--spaces\ in\ this\ filename--
pass: MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx

LEVEL3:
Any filename that starts with '.' gets hidden. to access:
1. ls -a:this lists all file, including hidden

cd inhere
ls -a
pass: 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ

LEVEL4:
'file' : this command tells you the filetype
'./*' : all files in current directory

cmd inhere
file ./*
cat ./-file07
pass: 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw

LEVEL5:
'du' : tells the file size
'find [path] [options] [expression]': helps finding a file
'find . -type f -size 1033c': finds (.) in current directory, (-type f) only file, (-size 1033c) exactly 1033 bytes

cd inhere
find . -type f -size 1033c
cat ./maybehere07/.file2
pass: HWasnPhtq9AVKe0dmk45nxy20cvUa6EG

LEVEL6:
use '/' to search in the entire system
use the option '-user/-group' to filter by the owner of the file
'2>/dev/null' removes the spam message 'permission denied'
(2 refers to stderr (errors), > means rederict, /dev/null is a trashcan in linux, where any thing that goes is deleted forever

find / -type f -user bandit7 -group bandit6 -size 33c 2>/dev/null
cat /var/lib/dpkg/info/bandit7.password
pass; morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj

LEVEL7:
'grep <pattern in ""> <filename>':prints the line containg the patternm

grep "millionth" data.txt
pass: dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc

LEVEL8:
redirecting: '>' this redirectrs whaterver the stdout is to the location
piping: '|' this helps in using the output of the cmd written on left to be used as the input of cmd written on the write
'sort': this sorts the entire file (lexicographically here)
'uniq': compare the adjacent files and (-c) count the number of occurences, (-u) print the unique line, (-d) print only duplicate lines

sort data.txt | uniq -u
pass: 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM

LEVEL9:
'string': stdout the sequences of printable characters

string data.txt | grep "===="
pass: FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey

LEVEL10:
"base64 encoding: binary is encoded to ascii characters
'-d' is the option decode
base64 -d data.txt
pass: dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr

LEVEL11:
wrtie code in c;
'tr': translates the characters. it takes 2 arguments, <arg1> is replaced by <arg2>
eg: echo "hello" | tr 'h' 'y', prints yello

cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m'
pass: 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4

LEVEL12:
cp: copy file form source to dest
gzip: compress or expand files(faster, lower compression)
bzip2: compresses file(slower, better compression ratio)
xxd: make hexdump or do reverse(-r)
mv: rename file
bunzip2: decompress a file
gunzip: decompress
tar file: basically a collection (archive) of files
tar -xf: extracts file from the tar archive and (f) specifoes the filename of the process

mkdir /tmp/jatine
cd /tmp/jatin
cp ~/data.txt /tmp/jatin
xxd -r data.txt > file
file file

(if you see gzip file)
mv file file.gz
gunzip file.gz

(if you see bzip2 file)
mv file file.bz2
bunzip2 file.bz2

(if you see tar archive)
mv file file.tar
tar -xf file.tar

(loop jab tak you don't find ascii type file)file file
→ identify type
→ rename
→ extract
→ repeat

pass: FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn

LEVL13
scp: copies files between hosts on a network
chmod: changes file permission(who can read(r/4) write(w/2) execute(e/1)), 3types of users (u= owner(you), g=group,o=others)
chmod 600: (4+2)= read and write permission to owner, 0,0 no permission to group and others
we will restrict the acces only to us first

ssh bandit13@bandit.labs.overthewire.org -p 2220 "cat sshkey.private"
nano key.txt
chmod 600 key.txt
ssh -i key.txt bandit14@bandit.labs.overthewire.org -p 2220

now we are in bandit14

LEVEL14:
get current level pass: cat /etc/bandit_pass/bandit14
pass 14: MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS

netcat (nc) helps you connect your system (loclahost) to another port(Any server machine makes its services available to the Internet using numbered ports)

nc localhost 30000
<paste bandit14 pass>
pass obtained: 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo

LEVEL15:
ssl/tls: a protocol that encrypts data, to provide secure communication
openssl: netcat + ssl, helps in communicatng, but firsts encrypts
s_client: act as a client and connect to---localhost, port 30001

openssl s_client -connect localhost:30001
enter password 15
pass obtained: kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx

LEVEL16:
portscanner: basically a tool which knocks on every port(door) and check what services are happening (behindth door)
nmap: a portscanner, sacns all ports on localhost from 31000-32000
nmap -p 31000-32000 localhost
now we will get some open ports, to identify which port does ssl, use '-sv'
nmap -p 31000-32000 -sV localhost / or try manually connecting each port like level 15

ssl ports: 31518, 31790

cat /etc/bandit_pass/bandit16 | openssl s_client -quiet -connect localhost:31790
key:

LEVEL17:
diff passwords.old passwords.new
pass: x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO

LEVEL18:
before starting a shell, just read the readme while logging in
ssh bandit18@bandit.labs.overthewire.org -p 2220 cat readme
pass: cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8

LEVEL19:
you donot have the permission to read the file, but the program has, you use the ptrogram to get pass
./bandit20-do cat /etc/bandit_pass/bandit20
pass: 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO


LEVEL20:
we have to set up the talker listner model.
(listener node)in T1: nc -l -p 12345
in T2: ./suconnect 12345, then paste bandit20 pass
pass: EeoULMCra2q0dSkYj561DX7s1CpBuOBt

LEVEL21:
cd /etc/cron.d
ls
cat cronjob_bandit22
cat /usr/bin/cronjob_bandit22.sh
pass: tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q

LEVEL22:
cd /etc/cron.d
ls
cat cronjob_bandit23
cat /usr/bin/cronjob_bandit23.sh
echo I am user bandit23 | md5sum
cat /tmp/<hash>
cat /tmp/8ca319486bfbbc3663ea0fbe81326349
pass: 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga

LEVEL23:
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
(wait)

cat /tmp/myhack/password.txt
pass: gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8


LEVEL24:
pass: iCi86ttT4KSNe1armKiwbQNmB3YJP3q4

LEVEL25:
pass: s0773xxkk0MXfdqOfPRVr9L3jJBUOgCZ

LEVEL26:
pass: upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB

LEVEL27:
pass: Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN

LEVEL28:
pass :4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7

LEVEL29:
pass : qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL

LEVEL30:
pass: fb5S2xb7bRyFmAvQYQGEqsbhVyJqhnDy
