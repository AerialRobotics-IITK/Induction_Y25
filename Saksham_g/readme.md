 Whenever you find a password for a level, use SSH (on port 2220) to log into that level and continue the game.
 ssh bandit0@bandit.labs.overthewire.org -p 2220
 nano readme
 found password
 ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If is the password for level 0 to 1
 exit level 0
 
 ssh bandit1@bandit.labs.overthewire.org -p 2220
 level 1 - file is ascii text type - used file command
 used cat command
 also - cannot be used directly in terminal, so used./-
 263JGJPfgU6LtdEvgfWU1XP5yac29mFx password for level 1 to 2
 
 ssh bandit2@bandit.labs.overthewire.org -p 2220
 used cat ./--spa then tab button 
 MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx password for level 2 to 3
 
 ssh bandit3@bandit.labs.overthewire.org -p 2220
 cd inhere
 ls -a  shows all files
 cat ...Hiding-From-You
 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ password for level 3 to 4
 
 ssh bandit4@bandit.labs.overthewire.org -p 2220
 cd inhere
 file ./*
 ./-file07: ASCII text
 cat ./-file07
 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw is the password
 
 ssh bandit5@bandit.labs.overthewire.org -p 2220
 cd inhere
 du -a (didnt get any useful info)
 find . -type f -size 1033c ! -executable
 cat ./maybehere07/.file2
 HWasnPhtq9AVKe0dmk45nxy20cvUa6EG is the password
 
 ssh bandit6@bandit.labs.overthewire.org -p 2220
 find / -user bandit7 -group bandit6 -size 33c
 /var/lib/dpkg/info/bandit7.password
 cat /var/lib/dpkg/info/bandit7.password
 morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj is the password
 
 ssh bandit7@bandit.labs.overthewire.org -p 2220
 nano data.txt
 ctrl w to search millionth
 dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc is the password
 
 ssh bandit8@bandit.labs.overthewire.org -p 2220
 ls
 sort data.txt | uniq -u
 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM is the password
 
 ssh bandit9@bandit.labs.overthewire.org -p 2220
 strings data.txt
 FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey is the password
 
 ssh bandit10@bandit.labs.overthewire.org -p 2220
 base64 -d data.txt
 dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr is the password
 
 ssh bandit11@bandit.labs.overthewire.org -p 2220
 cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m'
 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4 is the password

 ssh bandit12@bandit.labs.overthewire.org -p 2220
 cd /tmp
 cd bandit/
 ls
 cat data8.bin
 FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn is the password
 
 ssh bandit13@bandit.labs.overthewire.org -p 2220
 ssh -i sshkey.private bandit14@localhost -p 2220
 cat /etc/bandit_pass/bandit14
 MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS is the password

 ssh bandit14@bandit.labs.overthewire.org -p 2220
 nc localhost 30000
 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo is the password 

 ssh bandit15@bandit.labs.overthewire.org -p 2220
 ncat localhost --ssl 30001
 submitted 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo 
 kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx is the password

 ssh bandit16@bandit.labs.overthewire.org -p 2220
 nmap -p 31000-32000 localhost
 openssl s_client -connect localhost:31790
 kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx
 
 MIIFBzCCAu+gAwIBAgIUBLz7DBxA0IfojaL/WaJzE6Sbz7cwDQYJKoZIhvcNAQEL
BQAwEzERMA8GA1UEAwwIU25ha2VPaWwwHhcNMjQwNjEwMDM1OTUwWhcNMzQwNjA4
MDM1OTUwWjATMREwDwYDVQQDDAhTbmFrZU9pbDCCAiIwDQYJKoZIhvcNAQEBBQAD
ggIPADCCAgoCggIBANI+P5QXm9Bj21FIPsQqbqZRb5XmSZZJYaam7EIJ16Fxedf+
jXAv4d/FVqiEM4BuSNsNMeBMx2Gq0lAfN33h+RMTjRoMb8yBsZsC063MLfXCk4p+
09gtGP7BS6Iy5XdmfY/fPHvA3JDEScdlDDmd6Lsbdwhv93Q8M6POVO9sv4HuS4t/
jEjr+NhE+Bjr/wDbyg7GL71BP1WPZpQnRE4OzoSrt5+bZVLvODWUFwinB0fLaGRk
GmI0r5EUOUd7HpYyoIQbiNlePGfPpHRKnmdXTTEZEoxeWWAaM1VhPGqfrB/Pnca+
vAJX7iBOb3kHinmfVOScsG/YAUR94wSELeY+UlEWJaELVUntrJ5HeRDiTChiVQ++
wnnjNbepaW6shopybUF3XXfhIb4NvwLWpvoKFXVtcVjlOujF0snVvpE+MRT0wacy
tHtjZs7Ao7GYxDz6H8AdBLKJW67uQon37a4MI260ADFMS+2vEAbNSFP+f6ii5mrB
18cY64ZaF6oU8bjGK7BArDx56bRc3WFyuBIGWAFHEuB948BcshXY7baf5jjzPmgz
mq1zdRthQB31MOM2ii6vuTkheAvKfFf+llH4M9SnES4NSF2hj9NnHga9V08wfhYc
x0W6qu+S8HUdVF+V23yTvUNgz4Q+UoGs4sHSDEsIBFqNvInnpUmtNgcR2L5PAgMB
AAGjUzBRMB0GA1UdDgQWBBTPo8kfze4P9EgxNuyk7+xDGFtAYzAfBgNVHSMEGDAW
gBTPo8kfze4P9EgxNuyk7+xDGFtAYzAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3
DQEBCwUAA4ICAQAKHomtmcGqyiLnhziLe97Mq2+Sul5QgYVwfx/KYOXxv2T8ZmcR
Ae9XFhZT4jsAOUDK1OXx9aZgDGJHJLNEVTe9zWv1ONFfNxEBxQgP7hhmDBWdtj6d
taqEW/Jp06X+08BtnYK9NZsvDg2YRcvOHConeMjwvEL7tQK0m+GVyQfLYg6jnrhx
egH+abucTKxabFcWSE+Vk0uJYMqcbXvB4WNKz9vj4V5Hn7/DN4xIjFko+nREw6Oa
/AUFjNnO/FPjap+d68H1LdzMH3PSs+yjGid+6Zx9FCnt9qZydW13Miqg3nDnODXw
+Z682mQFjVlGPCA5ZOQbyMKY4tNazG2n8qy2famQT3+jF8Lb6a4NGbnpeWnLMkIu
jWLWIkA9MlbdNXuajiPNVyYIK9gdoBzbfaKwoOfSsLxEqlf8rio1GGcEV5Hlz5S2
txwI0xdW9MWeGWoiLbZSbRJH4TIBFFtoBG0LoEJi0C+UPwS8CDngJB4TyrZqEld3
rH87W+Et1t/Nepoc/Eoaux9PFp5VPXP+qwQGmhir/hv7OsgBhrkYuhkjxZ8+1uk7
tUWC/XM0mpLoxsq6vVl3AJaJe1ivdA9xLytsuG4iv02Juc593HXYR8yOpow0Eq2T
U5EyeuFg5RXYwAPi7ykw1PW7zAPL4MlonEVz+QXOSx6eyhimp1VZC11SCg==
Wrong! Please enter the correct current password.

 mkdir tmp/hello
 cd hello
 touch pass.txt
 nano pass.txt 
 paste the above key
 chmod 600 pass.txt
 ssh -i sshkey.private bandit17@localhost -p 2220
 you are in level 17 now
 diff passwords.old passwords.new
 x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO is the password

 ssh bandit18@bandit.labs.overthewire.org -p 2220
 ssh bandit18@bandit.labs.overthewire.org -p 2220 -t 'cat readme'
 cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8 is the password

 ssh bandit19@bandit.labs.overthewire.org -p 2220
 ./bandit20-do cat /etc/bandit_pass/bandit20
 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO is the password

 ssh bandit20@bandit.labs.overthewire.org -p 2220
 echo "VxCazJaVykI6W36BkBU0mJTCM8rR95XT" | nc -l -p 2424 &
 ./suconnect 2424
 EeoULMCra2q0dSkYj561DX7s1CpBuOBt is the password

 ssh bandit21@bandit.labs.overthewire.org -p 2220
 cat /etc/crontab
 cat /etc/cron.d/cronjob_bandit22
 cat /usr/bin/cronjob_bandit22.sh
 cat /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7go3
 tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q is the password

 ssh bandit22@bandit.labs.overthewire.org -p 2220
 cat /etc/cron.d/cronjob_bandit23
 cat /usr/bin/cronjob_bandit23.sh
 echo I am user bandit23 | md5sum | cut -d ' ' -f 1
 cat /tmp/8ca319486bfbbc3663ea0fbe81326349
 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga is the password

 
