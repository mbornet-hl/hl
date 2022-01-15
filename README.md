hl : a colorization command
===========================

Purpose
-------
**In short** :

**hl** is a binary program compiled from **C** and **lex** source which can greatly **help you read log files**, the **output of commands** or **scripts**, **configuration files**, **text files**. It can **highlight thresholds**, **colorize blocks of text** delimited by markers, **alternate colors** when the value of an element **changes** (or on the contrary when it **does not change**), check values **consistency** on a line, **colorize fields** of text, and so on. **Many configurations** have been created, and you can **easily create yours** to suit your needs, using **simple text strings** or **regular expressions**.

Its purpose is to colorize **what is important in the text you read**. It has been designed to **help you get straight to the point**. It's **fast** and **efficient**.

There are no need to add plug-ins when you want to colorize a new syntax : you just have to define a new configuration with **basic** or **extended** **regular expressions**. You don't have to install the binary nor the configuration files in a system directory, you can use environment variables to specify where to fetch the files.

Contributing
------------
At the present time, **hl** has only been included in the ArchLinux distribution (see https://aur.archlinux.org/packages/hl-git/ ). There are no RPM nor .deb package, and if you want to use it, you'll have to compile it for your system (which is very easy) or just use the binary on this web site. **hl** would be more widely spread if it can be installed using **dpkg**, **apt-get**, **rpm**, or **yum**. So if you want to create such packages, you are welcome !

Another way to contribute is to **share the configurations that you have created that could help other people**. To do so, please add a small comment above your configuration to specify your name, the creation date of your configuration and its purpose, and add an example of the data (as a separate file : 50 lines max) that your configuration colorizes. You also can add a screenshot of the result of the colorization of your data. Those files will be located in a sub-directory of the *examples* directory.

**In more detail** :

This command is a compiled C program which can colorize text with up to 42 colors according to specified regular expressions. It can colorize patterns on a line when it finds a match, or colorize a group of lines from a start marker to an end marker, or alternate colors when the value of a match changes, or when it doesn't change.
Configuration files can be placed anywhere on the file system (not only in /etc/default) as long as you initialize the environment variables.
It uses regcomp() and regexec() to colorize (highlight) strings from stdin using options on the command line.

It can be used to colorize text files (such as log files, configuration files, source files, ...) or commands output.
Here are two examples of how you can colorize the output of  `ls` using the file date, by day (with `lD`) :
![lD](https://github.com/mbornet-hl/hl/blob/master/images/hl_lD.png)

or by week (with `lW`) :

![lW](https://github.com/mbornet-hl/hl/blob/master/images/hl_lW.png)

These are examples, the dates of the listed files have been modified to give suitable results. The scripts `lD` and `lW` are available in the `scripts` directory, and you can modify them to change the colors to your taste.

Standard system commands can be colorized without having to change their syntax nor having to manually pipe their output to the `hl`command, using the `hl_generic` script.
If you want to colorize the output of a command, here is what you have to do :
  1. Define a colorization configuration having the name of the command,
  2. Create a hard link on hl_generic with the name of the command,
  3. Make sure the directory containing the new link appears before
     the directory containing the original command in your PATH variable,
  4. Use your command the way you are used to.

`hl` can use up to 42 colors :

![hl_colors](https://github.com/mbornet-hl/hl/blob/master/images/hl_colors.png)

Articles (in french)
---------------------
Here is the [first review](https://connect.ed-diamond.com/Linux-Pratique/LP-093/Colorisez-vos-textes-avec-la-commande-hl) (Colorisez vos textes avec la commande hl) about `hl` in a french magazine (Linux Pratique), and here is the [second one](https://connect.ed-diamond.com/Linux-Pratique/LPHS-046/Tirez-parti-de-la-colorisation-pour-faciliter-la-lecture-de-vos-donnees) (Tirez parti de la colorisation pour faciliter la lecture de vos données).

Usage
-----

```
hl: version 1.114
Usage: hl [-o][-h|-H|-V|-[[%.]eiuvdDEL1234][-[rgybmcwRGYBMCWnAIsNpPxJ] regexp ...][--config_name ...] ]
  -o  : usage will be displayed on stdout (default = stderr)
  -h  : help
  -H  : help + configuration names
  -V  : version
  -v  : verbose
  -u  : do not bufferize output on stdout
  -e  : extended regular expressions
  -i  : ignore case
  -E  : print on stderr
  -r  : red
  -g  : green
  -y  : yellow
  -b  : blue
  -m  : magenta
  -c  : cyan
  -w  : white
  -R  : red     (reverse video)
  -G  : green   (reverse video)
  -Y  : yellow  (reverse video)
  -B  : blue    (reverse video)
  -M  : magenta (reverse video)
  -C  : cyan    (reverse video)
  -W  : white   (reverse video)
  -n  : never colorize
  -%c : specifies the beginning of a range colorized in color 'c'
  -.  : specifies the end of the previous range
  -d  : debug
  -D  : display regular expressions
  -L  : lex debug
  -1  : color brightness (half-bright)
  -2  : color brightness (normal : default)
  -3  : color brightness (bright)
  -4  : color brightness (underscore)
  -A  : alternate colors when string matched by selection regex changes
  -I  : alternate colors when string matched by selection regex does not change
        Syntax for alternate options : -{A|I}[[s],c1c2...cn]
         where s is a number from 0 to 9 indicating the selection regexp number,
         and c1, c2, ... cn are color specifiers to use
        Alternate colors implies extended regular expressions (-e)
  -s  : alternate colors when the string matched by the selection regex is the image
        by a simple function (+, -, * or /) of the value of the previous matching string
        Syntax for sequential control option : -s[[-+*/]p[adox]:][n],c1c2...cn]
         where p is a positive integer (parameter),
         n is a number from 0 to 9 indicating the selection regexp number,
         and c1, c2, ... cn are color specifiers to use
           d : decimal (default)
           o : octal
           x : hexadecimal
           a : ascii (first character of the matching string)
        Alternate colors implies extended regular expressions (-e)
  -J  : Colorize date according to the day of the week
        Syntax for day of week option : -JYs1ms2ds3[,c1c2...c7]
         where :
           s1 : number of the sub-regex for the year
           s2 : number of the sub-regex for the month
           s3 : number of the sub-regex for the day of the month
        and c1c2...c7 are the optional color specifiers for Sunday to Saturday
  -N  : consistent numbering of sub-expressions in -A/-I and -s
  -p  : display configuration(s) matching glob-like expression (pattern)
  -P  : display configuration(s) matching regexp
  -x  : display options count for each config (with -vH options)
```

You can get a more verbose version of the usage with the '-v' option :
```
hl: version 1.114
Usage: hl [-o][-h|-H|-V|-[[%.]eiuvdDEL1234][-[rgybmcwRGYBMCWnAIsNpPxJ] regexp ...][--config_name ...] ]
  -o  : usage will be displayed on stdout (default = stderr)
  -h  : help
  -H  : help + configuration names
  -V  : version
  -v  : verbose
  -u  : do not bufferize output on stdout
  -e  : extended regular expressions
  -i  : ignore case
  -E  : print on stderr
  -r  : red
  -g  : green
  -y  : yellow
  -b  : blue
  -m  : magenta
  -c  : cyan
  -w  : white
  -R  : red     (reverse video)
  -G  : green   (reverse video)
  -Y  : yellow  (reverse video)
  -B  : blue    (reverse video)
  -M  : magenta (reverse video)
  -C  : cyan    (reverse video)
  -W  : white   (reverse video)
  -n  : never colorize
  -%c : specifies the beginning of a range colorized in color 'c'
  -.  : specifies the end of the previous range
  -d  : debug
  -D  : display regular expressions
  -L  : lex debug
  -1  : color brightness (half-bright)
  -2  : color brightness (normal : default)
  -3  : color brightness (bright)
  -4  : color brightness (underscore)
  -A  : alternate colors when string matched by selection regex changes
  -I  : alternate colors when string matched by selection regex does not change
        Syntax for alternate options : -{A|I}[[s],c1c2...cn]
         where s is a number from 0 to 9 indicating the selection regexp number,
         and c1, c2, ... cn are color specifiers to use
        Alternate colors implies extended regular expressions (-e)
        Example : -A2,2By  '(^([^ ]+ [0-9]{2} ..:..):..)'
        Example : -A1 '(.*([12][0-9]{3}[-/][0-9]{2}[-/][0-9]{2})[   ]+.*)'
        Example : -I1 '^([^:]*:[^:]*:([^:]*)[:]*.*)'
  -s  : alternate colors when the string matched by the selection regex is the image
        by a simple function (+, -, * or /) of the value of the previous matching string
        Syntax for sequential control option : -s[[-+*/]p[adox]:][n],c1c2...cn]
         where p is a positive integer (parameter),
         n is a number from 0 to 9 indicating the selection regexp number,
         and c1, c2, ... cn are color specifiers to use
           d : decimal (default)
           o : octal
           x : hexadecimal
           a : ascii (first character of the matching string)
        Alternate colors implies extended regular expressions (-e)
        Example : -s1:2,3b3r  '^(#[   ]*define[      ]+[^      ]+[  ]+\(([   ]*-?[0-9]+)\))'
        Example : -s-5:2,1G1B  '^(Countdown[    ]*:[      ]*([0-9]{2}))'\(([      ]*-?[0-9]+)\))'
        Example : -s*2x:2,3g3m  '^(#[      ]*define[      ]+[^      ]+[  ]+0x([0-9a-fA-F]+))'\(([     ]*-?[0-9]+)\))'
  -J  : Colorize date according to the day of the week
        Syntax for day of week option : -JYs1ms2ds3[,c1c2...c7]
         where :
           s1 : number of the sub-regex for the year
           s2 : number of the sub-regex for the month
           s3 : number of the sub-regex for the day of the month
        and c1c2...c7 are the optional color specifiers for Sunday to Saturday
        Example : -JY2m3d4,3R1b1g2b2g3b3r '^(.* ([0-9]{4})-([0-9]{2})-([0-9]{2}))'
        Example : -Jm2:d3:Y4,3R1g1g1g1g1g3R '^(.* ([0-9]{2})/([0-9]{2})/([0-9]{4}))'
        Example : -JY2:m3:d4 '^(.* ([0-9]{4})-([0-9]{2})-([0-9]{2}))'
  -N  : consistent numbering of sub-expressions in -A/-I and -s
  -p  : display configuration(s) matching glob-like expression (pattern)
  -P  : display configuration(s) matching regexp
  -x  : display options count for each config (with -vH options)
Buffer size = 64 Ko
Environment variable HL_CONF        = "/home/machine/mb/.hl.cfg:/DATA3/projets/hl/config_files:/home/machine/mb/hl_conf"
Environment variable HL_CONF_GLOB   is undefined. Default value = "hl_*.cfg:hl.cfg:.hl_*.cfg:.hl.cfg".
Environment variable HL_DEFAULT     is undefined. Default value = "3Y".
Environment variable HL_A1          is undefined. Default value = "2B".
Environment variable HL_A2          is undefined. Default value = "1n".
Environment variable HL_SUNDAY      is undefined. Default value = "3R".
Environment variable HL_MONDAY      is undefined. Default value = "2b".
Environment variable HL_TUESDAY     is undefined. Default value = "2c".
Environment variable HL_WEDNESDAY   is undefined. Default value = "2g".
Environment variable HL_THURSDAY    is undefined. Default value = "3g".
Environment variable HL_FRIDAY      is undefined. Default value = "3y".
Environment variable HL_SATURDAY    is undefined. Default value = "3r".
```
To use a colorized version of the "man" command, you should define a pager :
MANPAGER=/usr/local/bin/hl_man_pager
(and copy hl_man pager to /usr/local/bin).

Examples
--------

```
# /sbin/ifconfig -a | hl -ei -m '^(eth|(vir)?br|vnet)[0-9.]*:[0-9]+\>'       \
                -b '^(eth|(vir)?br|vnet)[0-9.]*\.[0-9]+\>'                   \
                -c '([0-9a-f]{2}:){5}[0-9a-f]{2}'                            \
                -g '\<UP\>|\<RUNNING\>|([0-9]{1,3}\.){3}[0-9]{1,3}\>'        \
                -y '^(eth|(vir)?br|vnet)[0-9.:]*\>'

# /sbin/ifconfig -a | hl --ifconfig

# /sbin/ifconfig -a | hl --IP --MAC --eth

$ cat firewall_rules | hl -e -c INPUT              \
                       -y 'FORWARD|POSTROUTING'    \
                       -b '#.*'                    \
                       -W 'OUTPUT'                 \
                       -g '.*ACCEPT.*'             \
                       -r '.*(DROP|REJECT).*'      \
                       -m 'iptables.*-F.*'         \
                       -w '^iptables .*'

$ cat firewall_rules | hl --iptables

$ df -h | hl --df

$ hl -p df

$ hl -ovp ifconfig
```
![df](https://github.com/mbornet-hl/hl/blob/master/images/df.png)

```

# fdisk -l | hl --fdisk
```
![fdisk](https://github.com/mbornet-hl/hl/blob/master/images/fdisk.png)

Alternate colors on the first 15 lines of `vmstat` :
```
# vmstat 1 | head -n 15 | hl -A,2B3w
```
![vmstat](https://github.com/mbornet-hl/hl/blob/master/images/fig12.png)

Alternate colors when the first character of the filenames changes :
```
/bin/ls -l | hl -A1,3c2B '^[-d].*[       ](([^ ])([^ ]*))$'
```
![ls_blue](https://github.com/mbornet-hl/hl/blob/master/images/hl_ls_alt_blue.png)

![ls_yellow](https://github.com/mbornet-hl/hl/blob/master/images/hl_ls_alt_yellow.png)

or in another way using many colors :
```
/bin/ls -l | hl -A1,2Y3c2B2r2G '(^[-d].*[       ](.).*)$'
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_example_alternate.png)
