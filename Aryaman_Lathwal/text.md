Main command:
ssh -p 2220 bandit13@bandit.labs.overthewire.org



Passwords to levels:

To 1: ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If  

To 2: 263JGJPfgU6LtdEvgfWU1XP5yac29mFx had to use './' with cat to prevent shit with -

To 3: MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx had to use './' to prevent shit with - and also '\ ' in place of simply ' ' to not mess with spaces.

To 4: 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ had to use 'ls -a' to show hidden fines.

To 5: 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw had to check all basically.

To 6: HWasnPhtq9AVKe0dmk45nxy20cvUa6EG Had to use find command.'find -type f -size 1033c ! -executable' type f means file type and c is for bytes and ! - executable means not executable

To 7: morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj Had to use the find command in the '/' base directory and -user bandit7 and -group bandit6 in the filters.

To 8: dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc had to use grep command which searches inside files.

To 9: 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM had to use sort data.txt for sorting the whole data and in addition give a uniq -u command in the same line using pipe '|'.

To 10: FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey found this by looking at the gibberish myself xD
Anyway the correct way is to use 'strings data.txt', this prints printable characters in the output. To further filter stuff, 'strings data.txt | grep ==' to search only for those which has multiple ==.

To 11: dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr had to decode base64 data, using 'base64 -d *filename*'.

To 12: 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4 had to do a rotation by 13. How?
Using tr command which translates by mapping from one string to another. So if we give it 'abcde' 'efghi' then it makes all a -> e and so on. For this case we had to use this command: cat data.txt | tr 'a-zA-Z' 'n-za-mN-ZA-M'

To 13: FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn had to do shitton of work! Commands used: mktemp to make a temporary directory in /tmp and then cp to copy the file over there. 'xxd -r > banana' for reversing the hexadump and copying the contents to a file called banana. Then gzip -d to unzip also bzip2 -d to unzip and tar -xf to unarchive. Also renaming the files according to their file format so that unzip would give proper file

To 14: for this one, had to copy the contents of the private key (lot of text) into a file in my own local machine using 'nano bandit14' command to paste the text. Bandit14 simply being the name of the file. Then in ssh, extra '-i bandit14' to use the key.

To 15: retrieved lvl14pass: MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS. 
