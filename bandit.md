LEVEL 0
used ssh username@server -p(PORT) to conncet with the server 
LEVEL 1
LS to see all files and folder present only readme.md is present
used cat to see the file data cat readme which gave me password (ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If)
exit the conncection and now going to connced to bandit1 with above written password 
LEVEL 2
the file name is - we cant directly read the file because command line interpert it as a command option like in above before p we use dash so we have two ways to solve the issue either use deeliminator -- double dash or just ./ directory you know 
so cat -- - or ls ./- (263JGJPfgU6LtdEvgfWU1XP5yac29mFx)
LEVEL 3
TO ESCAPE SPACES WE USE \
MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx
LEVEL 4
the hidden files are seen using ls -a 
2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ
LEVEL 5
there were certain kinds of files which were human readable only which implies that there file type was data so i checked file type of each and found that file type of 7th file is ASCII char and i checked the file and it gave the password (4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw)
LEVEL 6
in the dirc there were 17 more dirc with alot of files in those so manual checking was not possible so with the help of find command we find the exact file with the size of 1033c where c represent the bytes and then read it using cat to get the password 
(HWasnPhtq9AVKe0dmk45nxy20cvUa6EG)
LEVEL7
This was good one i think, because i had to find and i did not had anything direct now there were three conditions to see 1.) owned by user bandit7 group bandit6 and its size it 33c now to solve it i had to use find and for checking user owned we use -user then for group we use -group and then for -size we know size now it gave alot of most of them were permission denied but one on path (/var/lib/dpkg/info/bandit7.password)  gave me password as (morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj), now in linux we have 3 streams 0 for input 1 for output and 2 for error now by doing 2>/dev/null we redirect all the error to /dev/null which is like a blackhole
LEVEL8
simply used grep the find the pattern or the same word and to print the line we get password as 
(dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc)
LEVEL9
so basically it had alot of line and duplicates too we can solve it easly just by sort and uniq so what does sort does it does sorting and arrange it alphabateclly or you can say numerically so yea we did and then only doing -uniq we will remove duplicates give and all the values but doing uniq -u gives me just uniqe value which came once now the question is why we sorted it, we sorted it uniq only works on adjecent elements so it just remove the adjacent duplicates so by using the command sort "data.txt" | uniq -u does the work and we get pass 
(4CKMh1JI91bUIZZPXDqGanal4xvAg0JM)
LEVEL10
so bascially we know that the password is kinda in huaman readable string between alot of human non readable things so we used the strings and get the text it was all written that the password starts with === so i just did grep "===" too and got the answer as 
(FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey)
LEVEL11
SIMPLY IT WAS WRITTEN IS WAS BASE64 and we had to decode it bas64 -d
(dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr)
LEVEL12
In this level the characters were shifted a bit so we have to transform each letter, back by 13 positions now lets say we have A after 13 chars we have N and and for Z we have M so if we had to shift 'a-z' to 'n-za-m'but here we have to reverse it so we will do tr 'n-za-m' 'a-z' same for upper case letters too cat data.txt | tr 'n-za-m' 'a-z' | tr 'N-ZA-M' 'A-Z' and pass is (7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4)
LEVEL13
SO basically to solve it we see it was in hex format then we stored it in another .bin file check its type and unizp till end and we get the password (FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn)
LEVEL14
To solve this level we only had the local key on badnit13 and the actual pass was on bandit14 so to access it i have to use it by extracting the key from the server to my local device using scp and then i tried using the key ssh -i key username@host -p but the key was tooo open so i had to secure it by using chmod to 600 and then again i tried and yup got entry in bandit14 now i extact the key from there and the password for the bandit14 user is (MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS)
LEVEL15
So we conncet with the localhost and send the data using nc so it is like nc ip port (whaterver you want to send) and then it sends and conncets and give use the pass (8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo)
LEVEL16 
Same as above but using SSL/TLS connection so openssl s_client -connect localhost:30001 it will share encriyption to each other, private pass key and the we put the pass and get next level pass (kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx)
LEVEL17
first we map all the ports and check which are active after that we get active ports and then check each which have SSL/TLS encripytion and then we put the key and get the private key data we make a new file on local system convert to private type and then chomd to give owner 6 (4+2) and the using private key we enter next level
LEVEL18
we check difference in each file and we get change and by that we get pass (x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO)
LEVEL19
When we login using SSH we it run 3 files /etc/profile , /.bash_profile , /.bashrc but someone modified bashrc so that it logout from the ssh server so we login using ssh user@login -p /bin/bash -norc. we get terminal and cat readme get password 
(cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8)
LEVEL20
So we have a binary file we execute it and it says run it as another user and then we run it it says bandit20 so basically we are sudoing bandit 20 to do some task of reading the file a getting the password as (0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO)


