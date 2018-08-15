# Restricted-Shell
A quick and simple shell in C that is designed to be fast and efficient... This shell is in early stages of development...

# Documentation
To easily setup this amazing restricted shell... Follow the below commands:

First, you need to change to the only working directory right now, and then we can download the repository! You can accomplish such by running: 

***Warning***: Running the below commands may and will most likely require "sudo" access!

```shell
cd /etc/ && git clone https://github.com/NerdOfCode/Restricted-Shell.git
```

Next, you can began the automated setup by running

```shell
cd Restricted-Shell && bash run.sh
```
And Congratulations on setting up this Restricted-Shell!

If you're interested in using this shell full-time, simply edit the `/etc/passwd` file and change to the following, by running:

```shell
sudo vipw 

USERNAME_HERE:x:1004:1003:,,,:/home/USERNAME_HERE:/etc/Restricted-Shell/run.sh
```

Enjoy!!! Oh, and also if anyone is to come across any errors or suggestions, please feel free to send an email to team@nerdofcode.com!

# W.I.P.

1. First, and foremost... We will need to add real time keyboard input in order to allow users to review previous commands...
