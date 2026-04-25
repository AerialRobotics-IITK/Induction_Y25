<<<<<<< HEAD:bandit_solutions.md
Format of logging:
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level x : [Password/Key to log into Level x] [Workflow on Level x-1 which led to unlocking level x]
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Level 0: ssh bandit0@bandit.labs.overthewire.org -p 2220 (Password: bandit0)

Level 1: ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If

Level 2: 263JGJPfgU6LtdEvgfWU1XP5yac29mFx (//To open a '-' file use cat ./-filename.txt)

Level 3: MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx (//To open a file with spaces: cat ./--spaces\ in\ this\ filename--) or (cat ./"--spaces\ in\ this\ filename--")

Level 4: 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ (//To open a Hidden file use ls -a or ls -la)

Level 5: 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw (//Opened every file one by one , nothing special)

Level 6: HWasnPhtq9AVKe0dmk45nxy20cvUa6EG
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        |<- Till here was the password, it contained many spaces...and after applying the check the file returned was the hidden one so had to do ls -a and then it appeared as ".file2" which had this password.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
(//find . -type f -size 1033c ! -executable -exec sh -c 'file "$1" | grep -q "text"' _ {} \; -print))
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-exec sh -c 'file "$1" | grep -q "text"' _ {} \;:

This is the "human-readable" filter.

It runs the file command on every file that matched the size and permission rules.

grep -q "text" checks if the file description contains the word "text" (which identifies ASCII, UTF-8, etc.).
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
find . -type f -size 1033c ! -executable --> This will likely give you a very short list (since 1033 bytes is a very specific size). You can then run file on those results to see which one is the text file you're looking for.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 "Got to learn a new thing:

"Green -> executable file, Gray -> non-exec, blue -> directory, red -> .tar, .zip, .gz" || ls -F  -> (*) means exec file
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Level 7: morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj ( "Searched the whole house aka Root Directory represented by "/" ) 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bash: find / -user bandit7 -group bandit6 -size 33c 2>/dev/null

2>/dev/null does: In Linux, there are two types of output:

Standard Output (stdout): The successful results you want.

Standard Error (stderr): The error messages (Permission denied).

The 2> tells the computer: "Take all the errors (stream #2) and throw them into /dev/null (the digital trash can)."

This leaves only the successful find—the path to your password—on your screen
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 8: dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc (Searched for a string in a particular file -> grep "millionth" data.txt)

Level 9: 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM (Things learnt => sort: Takes the messy list and puts all identical lines next to each other.)
                                          (              => uniq -u:duplicates are bunched together, it can easily spot them, discard them, and print the only line that stands alone)
                                          (              => Piping( | ) : Hands that sorted list over to the next command.)
                                          (Final Command: sort data.txt | uniq -u)

Level 10: FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey (did bash: strings data.txt --> printed out all the human-readable strings out of the trashy data)

Level 11: dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr (bash: base64 -d data.txt)

Level 12: 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4 (bash: cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m') [ROT13 technique]

Level 13: FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn (Crazy thing happened right there: I hopped onto someone else's shared directory named "Level12" and got the the password without doing anything...Like Lmaoooooo)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Flow: 
Data.txt is just a text file that describes binary data. You can't "unzip" a text file.
By "xxd -r data.txt -> random_file"
random_file got created then its file type was checked and found out to be gzip
then name change: mv random_file random_file.gz
then : gunzip random_file.gz
then unzipped file type checked: bzip2 -> mv random_file random_file.bz2
then: bzip2 -d random_file.bz2
then subsequently after steps: when POSIX tar archive (GNU) was found then again mv and to unzip : tar -xf random_file.tar
And finally after many steps when data8.bin (even after gunzip) was found to have ASCII file type then it was read and the password was found
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 14: *Check file: bandit14.txt on Desktop
           Password: MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS (once entering, bash: cat /etc/bandit_pass/bandit14) : 
           The /etc directory is where the system stores configuration files and administrative data. It stands for "Editable Text Configuration" (or just "et cetera"). It’s the "brain" of the operating system's settings.
           bandit_pass: This is a specific folder created by the OverTheWire team. It acts as a digital vault. It contains one file for every level in the game.
           bandit_14: This is a simple text file that contains only one thing: the password for the next level.

Flow: Created a txt file at local machine with this SSH key then opening terminal in the file location and chmodding 600 to it and then used 
"ssh -i bandit14.txt bandit14@bandit.labs.overthewire.org -p 2220" to log in
-i : It told SSH "Don't ask me for a password. Look inside this file on my Desktop for my private key."
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 15: 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo (bash: nc localhost 30000) 
          nc -> networking utilitiy to establish two-way terminal
          localhost -> It tells "Don't look for this service on the internet; look for a program running right here on the same machine"
          port 30000 -> The Port is like an individual apartment number inside a building of IP address.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 16: kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx (bash: openssl s_client -connect localhost:30001) 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 17: EReVavePLFHtFlFsjn3hyzMlvSuSAcRD (Check Bandit17.txt on desktop)

Flow:     bash: nmap -p 31000-32000 -sV localhost
          nmap: Interacts with every port to check which one is open and which not
          -sV: tells the type of encryption ports hold (ssl/echo mean its useless it will just return anything that you'll enter) (ssl/unknown is mysterious)
          This fetches the list of ports that u wanna check
          Checked a particular port with openssl
          KEYUPDATE hanging was there hence used -> bash: echo "kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx" | openssl s_client -connect localhost:31790 -quiet
          echo also enters a newline character preventing the hanging in the output and quiet strips out the certificates and all that the port returns when connecting

Level 18: x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO (diff passwords.old passwords.new) 

Level 19: cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8 (passed "cat readme" along with the ssh command only to tunnel through the .bashrc logout)

Level 20: 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO (This was really crazy! bash: ./bandit20-do cat /etc/bandit_pass/bandit20)
          This bandit20-do file had UID for the user bandit20 which works like a uniform that allows to access things that are permitted to bandit20
          
Level 21: EeoULMCra2q0dSkYj561DX7s1CpBuOBt (sent data to a random port via nc; bash: echo "0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO" | nc -lp 1234 &)
          -l -> listen (wait); -p -> (specifying port); & -> sends it to background
          then ./suconnect 1234 --> this connects to the same port gets the password sent through the port via echo, verifies it and returns the next password to the terminal
          
Level 22: tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q (cron is like a butler who autommates the stuff and doesn't do anything until specified)
          ls /etc/cron.d/ -> checked the present files
          cat /etc/cron.d/cronjob_bandit22
          @reboot bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null
          * * * * * bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null (* * * * * -> tells to apply this every minute)
          "/usr/bin/cronjob_bandit22.sh" -> is the script that cron is running
          bash: cat /usr/bin/cronjob_bandit22.sh
          #!/bin/bash
          chmod 644 /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv
          cat /etc/bandit_pass/bandit22 > /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv (This /tmp/.... is the file where the cron is storing the password to)
          finally: cat t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv
    
Level 23: 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga (with basic above steps only:)
          Got $mytarget = echo I am user $myname | md5sum | cut -d ' ' -f 1 ($myname is a placeholder)
          Hash returned
          cat hash
          
Level 24: gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 (Ran a shellscript file that transferred the cronjob_bandit24 tasks to our own txt file)

Level 25: iCi86ttT4KSNe1armKiwbQNmB3YJP3q4 (Created own scrip file in which looping was done to try every single permutation while being connected)

Level 26: *check bandit26key.txt (has the ssh key) -> explored a whole new weird method: shrunk down the terminal and entered
          :set shell= /bin/bash
          :shell
          and on maximising i got into bandit26

Level 27: upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB (Nothing new, a previous tactic only)

Level 28: Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN (git cloning to the local machine)

Level 29: 4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7 (git log history) using git log -p
Level 30: qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL (git branch to see all the branches) -> Checked into every branch and its logs -> found the password in the remotes/origin/dev logs
																




                                          



=======
Format of logging:
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level x : [Password/Key to log into Level x] [Workflow on Level x-1 which led to unlocking level x]
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Level 0: ssh bandit0@bandit.labs.overthewire.org -p 2220 (Password: bandit0)

Level 1: ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If

Level 2: 263JGJPfgU6LtdEvgfWU1XP5yac29mFx (//To open a '-' file use cat ./-filename.txt)

Level 3: MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx (//To open a file with spaces: cat ./--spaces\ in\ this\ filename--) or (cat ./"--spaces\ in\ this\ filename--")

Level 4: 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ (//To open a Hidden file use ls -a or ls -la)

Level 5: 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw (//Opened every file one by one , nothing special)

Level 6: HWasnPhtq9AVKe0dmk45nxy20cvUa6EG
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        |<- Till here was the password, it contained many spaces...and after applying the check the file returned was the hidden one so had to do ls -a and then it appeared as ".file2" which had this password.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
(//find . -type f -size 1033c ! -executable -exec sh -c 'file "$1" | grep -q "text"' _ {} \; -print))
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-exec sh -c 'file "$1" | grep -q "text"' _ {} \;:

This is the "human-readable" filter.

It runs the file command on every file that matched the size and permission rules.

grep -q "text" checks if the file description contains the word "text" (which identifies ASCII, UTF-8, etc.).
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
find . -type f -size 1033c ! -executable --> This will likely give you a very short list (since 1033 bytes is a very specific size). You can then run file on those results to see which one is the text file you're looking for.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 "Got to learn a new thing:

"Green -> executable file, Gray -> non-exec, blue -> directory, red -> .tar, .zip, .gz" || ls -F  -> (*) means exec file
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Level 7: morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj ( "Searched the whole house aka Root Directory represented by "/" ) 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bash: find / -user bandit7 -group bandit6 -size 33c 2>/dev/null

2>/dev/null does: In Linux, there are two types of output:

Standard Output (stdout): The successful results you want.

Standard Error (stderr): The error messages (Permission denied).

The 2> tells the computer: "Take all the errors (stream #2) and throw them into /dev/null (the digital trash can)."

This leaves only the successful find—the path to your password—on your screen
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 8: dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc (Searched for a string in a particular file -> grep "millionth" data.txt)

Level 9: 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM (Things learnt => sort: Takes the messy list and puts all identical lines next to each other.)
                                          (              => uniq -u:duplicates are bunched together, it can easily spot them, discard them, and print the only line that stands alone)
                                          (              => Piping( | ) : Hands that sorted list over to the next command.)
                                          (Final Command: sort data.txt | uniq -u)

Level 10: FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey (did bash: strings data.txt --> printed out all the human-readable strings out of the trashy data)

Level 11: dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr (bash: base64 -d data.txt)

Level 12: 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4 (bash: cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m') [ROT13 technique]

Level 13: FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn (Crazy thing happened right there: I hopped onto someone else's shared directory named "Level12" and got the the password without doing anything...Like Lmaoooooo)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Flow: 
Data.txt is just a text file that describes binary data. You can't "unzip" a text file.
By "xxd -r data.txt -> random_file"
random_file got created then its file type was checked and found out to be gzip
then name change: mv random_file random_file.gz
then : gunzip random_file.gz
then unzipped file type checked: bzip2 -> mv random_file random_file.bz2
then: bzip2 -d random_file.bz2
then subsequently after steps: when POSIX tar archive (GNU) was found then again mv and to unzip : tar -xf random_file.tar
And finally after many steps when data8.bin (even after gunzip) was found to have ASCII file type then it was read and the password was found
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 14: *Check file: bandit14.txt on Desktop
           Password: MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS (once entering, bash: cat /etc/bandit_pass/bandit14) : 
           The /etc directory is where the system stores configuration files and administrative data. It stands for "Editable Text Configuration" (or just "et cetera"). It’s the "brain" of the operating system's settings.
           bandit_pass: This is a specific folder created by the OverTheWire team. It acts as a digital vault. It contains one file for every level in the game.
           bandit_14: This is a simple text file that contains only one thing: the password for the next level.

Flow: Created a txt file at local machine with this SSH key then opening terminal in the file location and chmodding 600 to it and then used 
"ssh -i bandit14.txt bandit14@bandit.labs.overthewire.org -p 2220" to log in
-i : It told SSH "Don't ask me for a password. Look inside this file on my Desktop for my private key."
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 15: 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo (bash: nc localhost 30000) 
          nc -> networking utilitiy to establish two-way terminal
          localhost -> It tells "Don't look for this service on the internet; look for a program running right here on the same machine"
          port 30000 -> The Port is like an individual apartment number inside a building of IP address.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 16: kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx (bash: openssl s_client -connect localhost:30001) 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Level 17: EReVavePLFHtFlFsjn3hyzMlvSuSAcRD (Check Bandit17.txt on desktop)

Flow:     bash: nmap -p 31000-32000 -sV localhost
          nmap: Interacts with every port to check which one is open and which not
          -sV: tells the type of encryption ports hold (ssl/echo mean its useless it will just return anything that you'll enter) (ssl/unknown is mysterious)
          This fetches the list of ports that u wanna check
          Checked a particular port with openssl
          KEYUPDATE hanging was there hence used -> bash: echo "kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx" | openssl s_client -connect localhost:31790 -quiet
          echo also enters a newline character preventing the hanging in the output and quiet strips out the certificates and all that the port returns when connecting

Level 18: x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO (diff passwords.old passwords.new) 

Level 19: cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8 (passed "cat readme" along with the ssh command only to tunnel through the .bashrc logout)

Level 20: 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO (This was really crazy! bash: ./bandit20-do cat /etc/bandit_pass/bandit20)
          This bandit20-do file had UID for the user bandit20 which works like a uniform that allows to access things that are permitted to bandit20
          
Level 21: EeoULMCra2q0dSkYj561DX7s1CpBuOBt (sent data to a random port via nc; bash: echo "0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO" | nc -lp 1234 &)
          -l -> listen (wait); -p -> (specifying port); & -> sends it to background
          then ./suconnect 1234 --> this connects to the same port gets the password sent through the port via echo, verifies it and returns the next password to the terminal
          
Level 22: tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q (cron is like a butler who autommates the stuff and doesn't do anything until specified)
          ls /etc/cron.d/ -> checked the present files
          cat /etc/cron.d/cronjob_bandit22
          @reboot bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null
          * * * * * bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null (* * * * * -> tells to apply this every minute)
          "/usr/bin/cronjob_bandit22.sh" -> is the script that cron is running
          bash: cat /usr/bin/cronjob_bandit22.sh
          #!/bin/bash
          chmod 644 /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv
          cat /etc/bandit_pass/bandit22 > /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv (This /tmp/.... is the file where the cron is storing the password to)
          finally: cat t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv
    
Level 23: 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga (with basic above steps only:)
          Got $mytarget = echo I am user $myname | md5sum | cut -d ' ' -f 1 ($myname is a placeholder)
          Hash returned
          cat hash
          
Level 24: gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 (Ran a shellscript file that transferred the cronjob_bandit24 tasks to our own txt file)

Level 25: iCi86ttT4KSNe1armKiwbQNmB3YJP3q4 (Created own scrip file in which looping was done to try every single permutation while being connected)

Level 26: *check bandit26key.txt (has the ssh key) -> explored a whole new weird method: shrunk down the terminal and entered
          :set shell= /bin/bash
          :shell
          and on maximising i got into bandit26

Level 27: upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB (Nothing new, a previous tactic only)

Level 28: Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN (git cloning to the local machine)

Level 29: 4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7 (git log history) using git log -p
Level 30: qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL (git branch to see all the branches) -> Checked into every branch and its logs -> found the password in the remotes/origin/dev logs
																




                                          



>>>>>>> 9c4761b2a4e17db5993c423a9d9310efb895bcc5:bandit_solutions.txt
