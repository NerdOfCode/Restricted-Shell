# Restricted-Shell
<img src="https://img.shields.io/badge/Build-Passing-brightgreen.svg">
A quick and simple shell in C that is designed to be fast and efficient... This shell is in early stages of development...

# Documentation
To easily setup this amazing restricted shell... Follow the below commands:

First, you need to change to the only working directory right now, and then we can download the repository! You can accomplish such by running: 

```shell
cd ~/ && git clone https://github.com/NerdOfCode/Restricted-Shell.git
```

Next, you can began the automated setup by running

```shell
cd Restricted-Shell/ && bash run.sh
```
And then just answer the questions to the automatic configuration!

If you're interested in using this shell full-time, simply edit the `/etc/passwd` file and change to the following, by running:

```shell
sudo vipw 

USERNAME_HERE:x:1004:1003:,,,:/home/USERNAME_HERE:/etc/Restricted-Shell/run.sh
```

Enjoy!!! Oh, and also if anyone is to come across any errors or suggestions, please feel free to send an email to team@nerdofcode.com!

# Notes

When installing this Restricted-Shell, make sure to be mindful of the permissions you place on this shell's files/directories...

As of right now, I recommend creating and using separate installations for each user... I realize this is a major inconvenience to most, it is the best way to ensure the upmost security of your system.

Good luck :)!

# W.I.P.

1. First, and foremost... We will need to add real time keyboard input in order to allow users to review previous commands...
