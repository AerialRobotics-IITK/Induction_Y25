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

To 14: for this one, had to copy the contents of the private key (lot of text) into a file in my own local machine using 'nano bandit14' command to paste the text, bandit14 simply being the name of the file. Then use 'chmod 600 <file>' to change the permissions of the file, first digit for owner, 6 meaning read and write perm, second digit group, 0 means no perm, third digit for others, 0 again no perm. Then in ssh, extra '-i bandit14' to use the key.

To 15: 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo retrieved lvl14pass: MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS. Used 'nc localhost 30000' command to connect to localhost on port 30000. Then it waits for password input to give back the next.

To 16: kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx we are doing a similar thing. Connecting to localhost on some specified port. But this time we are required to use encryption which is achieved by the command 'openssl s_client -connect localhost:<port>' against 'nc localhost <port>'. The arguments are different and must be looked at carefully.

To 17: Received a private key. Stored in ~/gitp . Used the command 'nmap -p 31000-32000 localhost', nmap means network mapping, -p tells which ports to scan for open status, and localhost to tell where to check the ports. Then used same openssl command but the output was not the true one so used an option -quiet to mute the protocol itself idk.

To 18: x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO easy enough. 'diff <file1> <file2>' shows whats different.

To 19: cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8 had to use 'cat readme' right next to the ssh command to read the file before logging out.

To 20: 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO the file bandit20.do allows us to access stuff which only the user bandit20 can. For example the file /etc/bandit_pass/bandit20 . So the command was './bandit20-do cat /etc/bandit_pass/bandit20' to open that file as bandit20.

To 21: EeoULMCra2q0dSkYj561DX7s1CpBuOBt for this one, the binary connects to a port, so we must act like a server and listen on a port. We use the nc command again. 'nc -l -p <any port>' here -l means we listen and wait for someone to connect. And -p is for specifying port.

To 22: tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q in the cron directory, bandit22 cronjob was running a script, which had location of the file it was modifying, which had the pass.

To 23: 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga checked what the script was trying to say.

To 24: gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 made a temp directory with mktemp, made a script with nano with the program: cat /etc/bandit_pass/bandit24 > /tmp/myoutput and then extracted pass.

To 25: iCi86ttT4KSNe1armKiwbQNmB3YJP3q4 coded a script which runs from 0000 to 9999 and eventually password came.

To 26: 










