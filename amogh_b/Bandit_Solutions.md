BANDIT


lvl 0 did ls and used cat to read it 
cat readme
pwd ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If

lvl 1 used ./ to signify current directory before file name so that it doesn't mistake it for a flag 
cat ./-
pwd 263JGJPfgU6LtdEvgfWU1XP5yac29mFx

lvl 2 used -- to tell cat not to think of it as options and as filenames and wrapped the filename in double quotation marks
cat -- "--spaces in this filename--"
pwd MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx

lvl 3 used find to get the hidden filename and did cat. 
find
cat ...Hiding-From-You
pwd 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ

lvl 4 kept reading files till i got readable 
cat -- -file07
pwd 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw

lvl 5 used find command with -size option
find -size 1033c
pwd HWasnPhtq9AVKe0dmk45nxy20cvUa6EG

lvl 6 find / -size 33c -group bandit6 -user bandit7 2>&1
pwd morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj

lvl 7 grep "millionth" data.txt
pwd dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc

lvl 8 sort data.txt | uniq -u
pwd 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM

lvl 9 strings data.txt
pwd FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey

lvl 10 echo VGhlIHBhc3N3b3JkIGlzIGR0UjE3M2ZaS2IwUlJzREZTR3NnMlJXbnBOVmozcVJyCg== | base64 -d
pwd dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr

lvl 11  cat data.txt (and then decode the rot13 cypher)
pwd 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4

lvl 12 It had repeated compression, used gzip -d for gzip, bzip2 -d for bzip2, tar -xf for POSIX tar(GNU)
pwd FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn

lvl 13 used scp to download ssh private key and logged in using it by ssh -i sshkey.private
no pwd

lvl 14  cat bandit14 | nc localhost 30000
pwd: 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo

lvl 15 openssl s_client -connect localhost:30001 -quiet
pwd: kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx

lvl 16 first i did  nmap -p 31000-32000 --open localhost
then tested the ports for ssl encryption
openssl s_client -connect localhost:31790 -quiet
chmod 600 ~/bandit17.key
ssh -i ~/bandit17.key bandit17@bandit.labs.overthewire.org -p 2220

no pwd




