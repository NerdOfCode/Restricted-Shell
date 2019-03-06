# Restricted-Shell

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0889612457db4cad8696f21db3b970bc)](https://app.codacy.com/app/NerdOfCode/Restricted-Shell?utm_source=github.com&utm_medium=referral&utm_content=NerdOfCode/Restricted-Shell&utm_campaign=Badge_Grade_Dashboard)

| OS | Status |
|----|--------|
| Ubuntu 16.04 | <img src="https://img.shields.io/badge/Build-Passing-brightgreen.svg">               |
| Ubuntu 18.04 | <img src="https://img.shields.io/badge/Build-Passing-brightgreen.svg">               |
| CentOS 7.5   | <img src="https://img.shields.io/badge/Build-Passing-brightgreen.svg">               |
| Fedora 28    | <img src="https://img.shields.io/codeship/d6c1ddd0-16a3-0132-5f85-2e35c05e22b1.svg"> |

A quick and simple shell in C that is designed to be fast and efficient... This shell is in early stages of development...

## Documentation
To easily setup this amazing restricted shell... Follow the instructions found below:

## Single User Mode

First, you need to change to the only working directory right now, and then we can download the repository! You can accomplish such by running: 

```shell
cd ~/ && git clone https://github.com/NerdOfCode/Restricted-Shell.git
```

Next, you can began the automated setup by running

```shell
cd Restricted-Shell/ && bash run.sh
```
And then just answer the questions to the automatic configuration!

### Multi-User Mode

There is no such mode as of now.

## Setting R-Shell to Default Shell

***Please be aware that the below option is completely experimental and may not be working***

If you're interested in using this shell full-time, simply edit the `/etc/passwd` file and change to the following, by running:

```shell
sudo vipw 

USERNAME_HERE:x:1004:1003:,,,:/home/USERNAME_HERE:/home/USER_HERE/Restricted-Shell/shell
```

After performing the above, make sure to first run `run.sh` to customize your shell... Then feel free, to logout and back in, to see the changes!

Enjoy!!! Oh, and also if anyone is to come across any errors or suggestions, please feel free to send an email to team@nerdofcode.com!

## Notes

When installing this Restricted-Shell, make sure to be mindful of the permissions you place on this shell's files/directories...

As of right now, I recommend creating and using separate installations for each user... I realize this is a major inconvenience to most, it is the best way to ensure the upmost security of your system.

Good luck :)!

## W.I.P

1.  First, and foremost... We will need to add real time keyboard input in order to allow users to review previous commands...
2.  Secondly, we need to add an enhanced multi-user mode!
