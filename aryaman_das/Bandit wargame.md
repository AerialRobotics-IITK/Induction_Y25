* Level 1 \- Simple SSH command `ssh -p 2220 bandit.labs.overthewire.org -l bandit0`  
* `ls` and `cat`  
* Levels 1 to 4 \- `ls` and `cat` with quotes or with `-a` to see hidden files as well  
* Level 5 and 6 \- `find` command  
* Pipe the output of cat with the grep command \- `cat data.txt | grep “millionth”`   
* `sort data.txt | uniq -u`  
* `strings data.txt` reveals all the strings and the password is in them  
* `cat data.txt | base64 -d`  
* Decode the ROT13 string on [dcode.fr](http://dcode.fr)  
* Level 12 \- First use `xxd` to convert it back to a normal file, which is then repeatedly unzipped  
* Unable to solve  
* Level 14 \- Use `nc` to connect to port 30000  
* Level 15 \- Use `ncat` because `nc` does not have SSL capability  
* Level 16 \- nmap scan  
* Level 18 \- `ssh bandit.labs.overthewire.org -p 2220 -l bandit18 command ls` followed by `ssh bandit.labs.overthewire.org -p 2220 -l bandit18 command cat readme` 
