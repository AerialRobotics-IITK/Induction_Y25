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
Level14: 
