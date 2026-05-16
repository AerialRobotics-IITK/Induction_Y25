------BANDIT WARGAME------
Level 0: SSH (Secure Shell) is a cryptographic network protocol for operating network services securely over an unsecured network. Both ends of a communication channel use automatically generated public-private key pairs to encrypt a network connection, and then use a password to authenticate the user. While authentication is based on the private key, the key is never transferred through the network during authentication. SSH only verifies that the same person offering the public key also owns the matching private key.
command - ssh [username]@[hostname] -p [port]
-p tells the ssh program to not use the default port 22, use 2200 instead

Level 1:
ls lists all files and directories (directory contents) on the directory (here home directory)
ls gives readme only
to display the contents of readme on the terminal, use cat readme
it gives the password
* do not try to login to bandit1 while logged in to bandit0 before
first exit
then ssh bandit1@bandit.labs.overthewire.org -p 2220 and enter password

Level 2:
"cat -" since file is not specified, it just shows whatever the keyboard input is!
to exit this mode, do ctrl+c (This sends an "interrupt" signal to the terminal)
If the file name starts with dash, the most portable method is to use ./ to denote the current directory.
Many commands support a double dash to signal the end of command options. Everything following the -- is treated as a filename.
password- 263JGJPfgU6LtdEvgfWU1XP5yac29mFx

Level 3: 
Though spaces in filename are supported by all modern operating systems, including Windows, macOS, and Linux, they cause issues in command line environments. (because spaces are used as delimiters to separate command from arguments.)
what worked: cat ./'--spaces in this filename--'
other methods are placing a backslash \ before every space and doing ./ for the dash in the beginning.
password - MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx

Level 4:
cd inhere to change the directory.
In Linux, files with names starting with . are hidden and are not listed with just ls command
us ls -a or ls --all to display hidden files
file- .  ..  ...Hiding-From-You
cat .  ..  ...Hiding-From-You gives the password

Level 5:
cat each file one by one
password was found in -file07

Level 6:
find - search for files in a directory hierarchy with specific criteria and stuff
find [-H] [-L] [-P] [-D debugopts] [-Olevel] [starting-point...] [expression]
find searches the directory at each given starting point (if nothing is specified, . is assumed) by evaluating the expression from left to right, according to precedence, until the outcome is known.
If no expression is given, the expression -print is used (but you should probably consider using -print0 instead, anyway).
A symbolic link (or symlink) is a special type of file in computing that acts as a reference to another file or directory. It stores a path to the target file.
-P     Never  follow  symbolic  links.   This  is  the  default  behaviour.  When find examines or prints information about files, and the file is a symbolic link, the information used shall be taken from the properties of the symbolic link itself.
-L     Follow symbolic links.
If more than one of -H, -L and -P is specified, each overrides the others; the last one appearing on  the command  line  takes effect.  Since it is the default, the -P option should be considered to be in effect unless either -H or -L is specified.

Expression: 
This is a kind of query specification describing how we match files and what we do with the files that  were  matched.

find . -size 1033c ! -executable
gives maybehere07 directory -file2
password: HWasnPhtq9AVKe0dmk45nxy20cvUa6EG

Level 7: find / -size 33c -user bandit7 -group bandit6 2>/dev/null 
need to search from the origin, so /
2: standard error
>: redirect symbol
/dev/null: makes the thing on left disappear (lines where permission is denied is removed)
gives /var/lib/dpkg/info/bandit7.password
cat /var/lib/dpkg/info/bandit7.password
password: morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj

Level 8:
grep  searches  for  PATTERNS  in  each  FILE.   PATTERNS  is one or more patterns separated by newline characters, and grep prints each line  that matches a pattern.  Typically PATTERNS should be quoted when grep is used in a shell command.
grep millionth data.txt
gives password: dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc

Level 9: 
The uniq command in Linux is used to filter out adjacent duplicate lines from text input, whether from a file or standard input.  It’s often paired with sort because uniq only detects duplicates that are next to each other.
uniq [OPTIONS] [INPUT_FILE [OUTPUT_FILE]]
If no file is specified, it reads from stdin and writes to stdout.
sort data.txt| uniq -u
-u, --unique : only print unique lines
password: 4CKMh1JI91bUIZZPXDqGanal4xvAg0JM

Level 10:
GNU strings prints the printable character sequences that are at least 4 characters long (or the number given with the options below) and are followed by an unprintable character.
strings filename
This command will extract and display all the printable strings from the specified file.
strings data.txt 
was able to manually spot the password easily
password: FGUW5ilLVJrxX9kMYMmlN4MgbpfMiqey

Level 11:
Base64 encode or decode FILE, or standard input, to standard output.
With no FILE, or when FILE is -, read standard input.
-d, --decode : decode data
base64 -d data.txt
password: dtR173fZKb0RRsDFSGsg2RWnpNVj3qRr

Level 12:
ROT13 is a simple letter substitution cipher that replaces a letter with the 13th letter after it in the Latin alphabet.
The tr command in Unix/Linux is a powerful utility used for translating, deleting, or compressing characters from standard input and writing the result to standard output. It is commonly used for text processing tasks such as case conversion, character replacement, and removing unwanted characters.
cat data.txt | tr 'A-Za-z' 'N-ZA-Mn-za-m'
The password is 7x16WNeHIi5YkIhWsfFIqoognUTyj9Q4

Level 13:
/tmp directory is a public space where anyone can create temporary files.
mktemp -d: This creates a directory with a random, hard-to-guess name.
/tmp/tmp.zC6boRPQju
cp - copy files and directories
cp ~/data.txt . copies data.txt to the current directory. (~represents home directory)
mv - renames source to destination.
xxd - make a hex dump or do the reverse.
To actually "save" the result so you can work with it, you have to tell Linux where to put the data using a redirection operator.
xxd -r mydatapass.txt > data1
Now you gotta redo the compression. We don't know what kind of file data1 is.
It is gzip compressed. Rename with extension .gz
gunzip data.gz to extract it. gunzip looks for data.gz, creates a new file called data (removing the .gz), and deletes the original data.gz. So, you don't need to redirect it to some other file (which doesn't even happen)
This is a common point of confusion because tar behaves differently than gunzip or bzip2.
When you use gunzip, it usually replaces the original file. When you use tar -xf (e-xtract file), it extracts the contents of the archive but does not necessarily name the resulting file "data". It extracts the file with its original name from when it was first archived. It is saved with some other name, use ls to find.
FO5dwFsc0cbaIiH0h8J2eUks2vdTDwAn

Level 14:
We get the RSA private key easily.
log out first then ssh -i sshkey.private bandit14@bandit.labs.overthewire.org -p 2220
cat /etc/bandit_pass/bandit14
MU4VWeTyJk8ROof1qqmcBPaLh7lDCPvS

Level 15:
use netcat (nc) to send messages!
cmd- echo "password"| nc 127.0.0.1 30000
here we are using 127.0.0.1 because we want to send the message to the localhost
password- 8xCjnmgoKbGLhHFAZlGE5Tmu4M2tKJQo

Level 16:
we are using openssl to create a client because the server is already encrypted with openssl. using nc directly does not work as it does not understand encryption. So we create a client to open this connection and then send the password to receive the password for the next level
openssl s_client -connect localhost:30001
password: kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx

Level 17:
command used - nmap -sT -p 31000-32000 127.0.0.1
this gives us: PORT      STATE SERVICE
31046/tcp open  unknown
31518/tcp open  unknown
31691/tcp open  unknown
31790/tcp open  unknown
31960/tcp open  unknown
sT means we are doing syn connect, where we use three way handshake to establish a connection.
(tried -sS, but nothing)
To find out which ports use ssl/tls, we use the command 
ss -t dst 127.0.0.1
-t checks which ports actually use tcp protocol, since ssl/tsl works on that. This gives 31790 only but to confirm if it actually uses ssl, we try connecting to it.
echo "kSkvUpMQ7lBYyCM4GBPvCvT1BfWRy0Dx" | openssl s_client -connect localhost:31790 -quiet
gives RSA private key
ssh -i sshkey.private.txt (.txt very imp) bandit17@bandit.labs.overthewire.org -p 2220

Level18:
diff passwords.old passwords.new gives:
diff passwords.old passwords.new
42c42
< KxOU4IzbXM8j8HeAWPAXTd1eC77mp1qV
---
> x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO
(< means the line is from file1 and > means it is from file2)
password: x2gLTTjFwMOhQ8oWNbMN362QKxfRqGlO

Level 19: 
Since we can't log into the level exactly, we use the cmd:
ssh bandit18@bandit.labs.overthewire.org -p 2220 "cat ~/readme"
password : cGWpMaKXVwDUNgPAVJbWYuGHVn9zl3j8

.bashrc is the command that is executed everytime you log into an interactive Linux environment. If you use the ssh user@host "command" method, you are starting a non-interactive shell. In many configurations, a non-interactive shell skips the .bashrc script.

Level 20:
In Linux, SetUID is a special type of file permission that allows a user to execute a program with the permissions of the file's owner, rather than the permissions of the user who is running it. (Say you want to edit a particular file in which only the admin has the permission to change it. In this case, setUID can be used)

password: 0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO

here bandit20-do is a program that makes the user behave like the host for the attribute/command written after it(?)
so we use 
./bandit20-do cat /etc/bandit_pass/bandit20

Level 21:at
password: EeoULMCra2q0dSkYj561DX7s1CpBuOBt
basically, you have to be the client and the server. 
echo "0qXahG8ZjOVMN9Ghs7iOWsCfZyXOUbYO" | nc -l -p 12345 & ./suconnect 12345
this causes suconnect to run in the background. basically, we create a listening connection on any port and suconnect initiates a handshake and a connection is established. Then the password is pushed down the connection and suconnect verifies it against the actual password and sends the next level's password (since it is a setuid binary, it can access level 21's password stored in its hard drive)

Level 22:
we first list all the files in /etc/cron.d/
this gives a number of files, the one named cronjob_bandit22 is the required file.
When we try reading it with cat, it gives:

@reboot bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null
* * * * * bandit22 /usr/bin/cronjob_bandit22.sh &> /dev/null

* * * * * means that the script runs every minute of every hour of every day. The actual shell script being executed every minute (by the permission of bandit22) is /usr/bin/cronjob_bandit22.sh

we read it, it says:

#!/bin/bash
chmod 644 /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv
cat /etc/bandit_pass/bandit22 > /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv

this means that the script is copying the password into this public file every minute. Note that we don't have the permission to read /etc/bandit_pass/bandit22. We read the file to get the password:

password: tRae0UfB9v0UzbCdn9cY0gQnds9GF58Q

Level 23:
Similar to the previous level, we got the list of files and found out that our required file is  /etc/cron.d/cronjob_bandit23. We read to find that a script (/usr/bin/cronjob_bandit23.sh) is run every minute all the time. We read that script, it gives:

#!/bin/bash

myname=$(whoami)
mytarget=$(echo I am user $myname | md5sum | cut -d ' ' -f 1)

echo "Copying passwordfile /etc/bandit_pass/$myname to /tmp/$mytarget"

cat /etc/bandit_pass/$myname > /tmp/$mytarget

If we run it just like that, then it take the username as bandit22 and eventually give the password for level22. To get the password for level 23, we manually type the command into our terminal:

echo I am user bandit23 | md5sum | cut -d ' ' -f 1

This gives the hash of the text (md5sum is a hashing technique): 8ca319486bfbbc3663ea0fbe81326349

(do not just run the entire shell script by replacing myname by bandit23, because we can't access /etc/bandit_pass/bandit23 directly like this) So our password to level 23 is in the file /tmp/8ca319486bfbbc3663ea0fbe81326349

password: 0Zf11ioIjMVN551jX3CmStKLYqjk54Ga

Level24:
We read cronjob_bandit24 and then, similar to previous levels, /usr/bin/cronjob_bandit24.sh

#!/bin/bash

shopt -s nullglob

myname=$(whoami)

cd /var/spool/"$myname"/foo || exit
echo "Executing and deleting all scripts in /var/spool/$myname/foo:"
for i in * .*;
do
    if [ "$i" != "." ] && [ "$i" != ".." ];
    then
        echo "Handling $i"
        owner="$(stat --format "%U" "./$i")"
        if [ "${owner}" = "bandit23" ] && [ -f "$i" ]; then
            timeout -s 9 60 "./$i"
        fi
        rm -rf "./$i"
    fi
done

This cronjob executes all files in /var/spool/bandit24/foo and then removes them. So we create a script file in tmp directory: 

#!/bin/bash

cat /etc/bandit_pass/bandit24 > /tmp/sau123mya/pass.txt

This when executed by cronjob (which has the permissions of bandit24) copies the password of level 24 into the file pass.txt (by creating it). But for that our directory and script should be executable:

so,
chmod 777 /tmp/sau123mya
chmod 777 /tmp/sau123mya/pass.txt

also copy script.sh to /var/spool/bandit24/foo by:
cp /tmp/sau123mya/script.sh /var/spool/bandit24/foo

we get the password:
gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8

Level 25:
we need to make a script with for loop.
we create a directory in tmp and we create a script.sh
nano script.sh:

#!/bin/bash

for i in {0..9}{0..9}{0..9}{0..9}
do
        echo "gb8KRRCsshuZXI0tUuR6ypOFjiZbf3G8 $i" >> list.txt;
done

then ./script.sh
and cat list.txt | nc localhost 30002
password: iCi86ttT4KSNe1armKiwbQNmB3YJP3q4

Level 26:
We easily get the RSA Private key.
But as soon as we login, we are kicked out.
cat /etc/passwd shows the default shell.
It shows that default shell of level 26 is /usr/bin/showtext
cat /usr/bin/showtext
shows the way it displays text is through the more command. when the more command completely executes, it exits the shell. We don't want that so we make the terminal small show that more command is not completely executed. 
now click v to open the vi text editor and there we change the default shell using :set shell = /bin/bash and then execute shell using :shell.

Level 27:
Use the setuid to get the password
password: upsNCc7vzaRDx6oZC6GiR6ERwe1MowGB

Level 28:
first we move into the directory where we want to clone it
then 
git clone ssh://bandit27-git@bandit.labs.overthewire.org:2220/home/bandit27-git/repo
the password is in readme:
The password to the next level is: Yz9IpL0sBcCeuG7m9uQFt8ZNpS4HZRcN

Level 29:
Do the same, but the current version doesn't have password. So we use git checkout to got to the previous commit:
password: 4pT1t5DENaYuqnqvadYs1oE4QLCdjmJ7

Level 30:
Do the same, but check our branch, at different commits. Checked different branches on remote. Found on the latest version of dev branch.
password: qp30ex3VLz5MDG1n91YowTv4Q8l7CDZL
