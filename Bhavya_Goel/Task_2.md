ssh into banditX on port 2220, then just use basic stuff like ls cat

level 0-1
just log in and do cat readme

level 1-2
file name is -, so normal cat wont work, use cat ./-

level 2-3
filename has spaces, so put it in quotes 

level 3-4
file is hidden, so use ls -a then cat .hidden

level 4-5
there are many files, find out which one is readable using file ./* then open the ascii text one

level 5-6
find file by size using find . -type f -size 1033c

level 6-7
search whole system using find / -user bandit7 -group bandit6 -size 33c 2>/dev/null

level 7-8
search inside file using grep millionth data.txt

level 8-9
find the line that appears once using sort data.txt | uniq -u

level 9-10
pull readable text out of binary using strings data.txt | grep "="

level 10-11
decode base64 using base64 -d data.txt

level 11-12
rot13 decoding using tr 'A-Za-z' 'N-ZA-Mn-za-m'

level 12-13
file is compressed in layers, so keep doing xxd -r then check file type then unzip or untar again and again until you get text

level 13-14
use ssh key login ssh -i sshkey.private bandit14@..., if it complains fix permissions with chmod 600

level 14-15
send password to service using nc localhost 30000

level 15-16
use ssl connection instead of nc using openssl s_client -connect localhost:30001

level 16-17
scan ports using nmap -p 31000-32000 localhost, then test with openssl on the ssl port you find
