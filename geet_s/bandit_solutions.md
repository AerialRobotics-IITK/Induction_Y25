level	pass	command

level 0  -> bandit0 given from question
level 1  -> ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If  cat readme
level 2  -> 263JGJPfgU6LtdEvgfWU1XP5yac29mFx  cat ./-
level 3  -> MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx  cat "./ --spaces in this filename--"
level 4  -> 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ  cd inhere, ls -a, cat ...Hiding-From-You
level 5  -> 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw  cd inhere, file ./*, cat ./-file07
level 6  -> HWasnPhtq9AVKe0dmk45nxy20cvUa6EG  find . -type f -size 1033c ! -executable, cat ./maybehere07/.file2
level 7  -> morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj  find / -user bandit7 -group bandit6 -size 33c
level 8  -> dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc  grep millionth data.txt
level 9  -> 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM  sort data.txt | uniq -u
level 10 -> FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey  strings data.txt | grep '=='
level 11 -> dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr  base64 -d data.txt
level 12 -> 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4  cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m'
level 13 -> FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn  xxd -r, gunzip, bunzip2, tar
level 14 -> fGrHPx402xGC7U7rXKDaxiWFTOiF0ENq  ssh -i sshkey.private bandit14@localhost -p 2220
level 15 -> jN2kgmIXJ6fShzhT2avhotn4Zcka6tnt  echo pass | nc localhost 30000
level 16 -> JQttfApK4SeyHwDlI9SXGR50qclOAil1  openssl s_client -connect localhost:30001
level 17 -> VwOSWtCAcEBotclCQideJDuZnCB14Tio   nmap scan, openssl s_client -connect localhost:31790
level 18 -> kfBf3eYk5BPBRzwjq1MbptlzIlvnLVSj  diff passwords.old passwords.new
level 19 -> awhqfNnAbc1nauvgGOq50Bdpe0rQ1qIt  ssh bandit18@bandit.labs.overthewire.org -p 2220 -t 'cat readme'
level 20 -> VxCazJaVykI6W36BkBU0mJTCM8rR95XT  ./bandit20-do cat /etc/bandit_pass/bandit20
level 21 -> NvEJF7oVjkddltPSrdKEFOllh9V1iEho  echo pass | nc -l -p 4444 &, ./suconnect 4444
level 22 -> WdDozAdTM2z9DiFEQ2mGlwngMfj4GVrR  cat /tmp/... cronjob
level 23 -> QYw0Y2aiA672PsMmh9puTQuhoz8SyR2G  md5sum of "I am user bandit23"
level 24 -> VAfGXJ1PBSsPSnvsjI8p759leLZ9GGar  cronjob in /var/spool/bandit24/foo/
level 25 -> p7TaowXH24LWeFpINb0t52Qp3z3oU0T3  brute-force 4-digit PIN on port 30002
level 26 -> c7GvcKlw9mC7aUQaPx7nwFstuAIBw1o1  bypass more shell with small terminal height + v
level 27 -> YnQpBuifNMas1hcUFk70Zmq862k1oKwg  ./bandit27-do cat /etc/bandit_pass/bandit27
level 28 -> 0ef186ac70e04ea33b4c1853d2526fa2  git clone local README
level 29 -> bbc96594b4e001778eee9975372716b2  git log -p found in history
level 30 -> 5b90576bedb2cc04c86a9e924ce42faf  git log --all found in origin/dev branch
level 31 -> 47e603bb428404d265f59c42920d81e5  git show secret tag
level 32 -> 3O9RfhqyAlVBEZpVb6LYStshZoqoSx5K  push key.txt to git
level 33 -> tQdtbs5D5i2vJwkO8mEyYEyTL8izoeJ0  escape \$0 from uppercase shell
