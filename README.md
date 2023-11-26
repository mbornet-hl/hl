hl : a colorization command
===========================

Purpose
-------
**In short** :

**hl** is a binary program compiled from **C** and **lex** source which can greatly **help you read log files**, the **output of commands** or **scripts**, **configuration files**, **text files**. It can **highlight thresholds**, **colorize blocks of text** delimited by markers, **alternate colors** when the value of an element **changes** (or on the contrary when it **does not change**), check values **consistency** on a line, colorize dates based on the **day of the week**, **colorize fields** of text, and so on. **Many configurations** have been created, and you can **easily create yours** to suit your needs, using **simple text strings** or **regular expressions**.

Its purpose is to colorize **what is important in the text you read**. It has been designed to **help you get straight to the point**. It's **fast** and **efficient**.

There are no need to add plug-ins when you want to colorize a new syntax : you just have to define a new configuration with **basic** or **extended** **regular expressions**. You don't have to install the binary nor the configuration files in a system directory, you can use environment variables to specify where to fetch the files.

Contributing
------------
At the present time, **hl** has only been included in the ArchLinux distribution (see https://aur.archlinux.org/packages/hl-git/ ). There are no RPM nor .deb package, and if you want to use it, you'll have to compile it for your system (which is very easy) or just use the binary on this web site. **hl** would be more widely spread if it can be installed using **dpkg**, **apt-get**, **rpm**, or **yum**. So if you want to create such packages, you are welcome !

Another way to contribute is to **share the configurations that you have created that could help other people**. To do so, please add a small comment above your configuration to specify your name, the creation date of your configuration and its purpose, and add an example of the data (as a separate file : 50 lines max) that your configuration colorizes. You also can add a screenshot of the result of the colorization of your data. Those files will be located in a sub-directory of the *examples* directory.

[Donation](https://paypal.me/MartialBornet)
--------
If this software helps you, please consider donating here : [https://paypal.me/MartialBornet](https://paypal.me/MartialBornet) !
Donations will encourage me to keep working on this software, fix bugs (if any), and create new configurations.
This work (software and configuration) is only done during my free time. It helps me a lot, and I hope it helps you too.

Thank you.

Purpose - In more detail 
------------------------
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
Here are some links to articles about **hl** in french magazines :
  1. [Colorisez vos textes avec la commande hl](https://connect.ed-diamond.com/Linux-Pratique/LP-093/Colorisez-vos-textes-avec-la-commande-hl)
  2. [Tirez parti de la colorisation pour faciliter la lecture de vos données](https://connect.ed-diamond.com/Linux-Pratique/LPHS-046/Tirez-parti-de-la-colorisation-pour-faciliter-la-lecture-de-vos-donnees)
  3. [Découvrez la puissance des nouvelles fonctions du surligneur automatique hl](https://connect.ed-diamond.com/linux-pratique/lphs-053/decouvrez-la-puissance-des-nouvelles-fonctions-du-surligneur-automatique-hl)

Usage
-----

```
hl: version 1.170
Usage: hl [-oO][-h|-H|-V|-[[%.]eiuvdDEL1234][-[rgybmcwRGYBMCWnAIsNpPxJTt] regexp ...][--config_name ...] ]
  -o  : usage will be displayed on stdout (default = stderr)
  -O  : debug messages will be displayed on stdout (default = stderr)
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
        Syntax for the use of a 2nd colorset triggered by another option :
         -{A|I}#a:s,c11c12...c1n:c21c22...c2p
         where s is a number from 0 to 9 indicating the selection regexp number,
         a is the number of the option that triggers the use of the 2nd colorset,
         c11c12...c1n are the colors of the 1st colorset, and
         c21c22...c2p are the colors of the 2nd colorset
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
        Syntax for day of week option : -J[Ys1ms2ds3[,c1c2...c7]]
         where :
           s1 : number of the sub-regex for the year
           s2 : number of the sub-regex for the month
           s3 : number of the sub-regex for the day of the month
        and c1c2...c7 are the optional color specifiers for Sunday to Saturday
  -T  : Colorize string according to specified thresholds values
        Syntax for thresholds option : -Ts[x],s1[:c1],s2[:c2]...s10[:c10]
         where :
           s is a number from 0 to 9 indicating the selection regexp number,
           x indicates that thresholds are in hexadecimal
           si : value of the i-th threshold for 1 <= i <= 10
           c1 : color of the i-th range     for 1 <= i <= 10
  -t  : Colorize string according to time periods
        Syntax for time periods option : -tp[0][R][:num][:spec][,c1c2...c10]
         where :
           p is a time period specifier in [YmwdHMSun]
           0 tells that the date must be framed at the beginning of the period
           R is an optional flag telling to use an optional time reference
             instead of the current time. The optional time reference must be
             specified before the regex argument
           num is an optional number of time periods (default is 1)
           spec is a string specifying the position of date elements,
             composed of letters in [YmbdHMSus], each one followed by the
             number of the sub-regex it is associated to,
         and c1c2...c7 are the optional color specifiers for the time periods
  -N  : consistent numbering of sub-expressions in -A/-I and -s
  -p  : display configuration(s) matching glob-like expression (pattern)
  -P  : display configuration(s) matching regexp
  -x  : display options count for each config (with -vH options)
```

You can get a more verbose version of the usage with the '-v' option :
```
hl: version 1.170
Usage: hl [-oO][-h|-H|-V|-[[%.]eiuvdDEL1234][-[rgybmcwRGYBMCWnAIsNpPxJTt] regexp ...][--config_name ...] ]
  -o  : usage will be displayed on stdout (default = stderr)
  -O  : debug messages will be displayed on stdout (default = stderr)
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
        Example : -A2,2By  '(^([^ ]+ [0-9]{2} ..:..):..)'
        Example : -A1 '(.*([12][0-9]{3}[-/][0-9]{2}[-/][0-9]{2})[   ]+.*)'
        Example : -I1 '^([^:]*:[^:]*:([^:]*)[:]*.*)'
        Alternate colors implies extended regular expressions (-e)
        Syntax for the use of a 2nd colorset triggered by another option :
         -{A|I}#a:s,c11c12...c1n:c21c22...c2p
         where s is a number from 0 to 9 indicating the selection regexp number,
         a is the number of the option that triggers the use of the 2nd colorset,
         c11c12...c1n are the colors of the 1st colorset, and
         c21c22...c2p are the colors of the 2nd colorset
        Example : -A0,2B3c '^[^ ]+ +([^ ]+) ' -A#1:0,2G3g:3r2R '^([^ ]+) '
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
        Syntax for day of week option : -J[Ys1ms2ds3[,c1c2...c7]]
         where :
           s1 : number of the sub-regex for the year
           s2 : number of the sub-regex for the month
           s3 : number of the sub-regex for the day of the month
        and c1c2...c7 are the optional color specifiers for Sunday to Saturday
        Example : -JY2m3d4,3R1b1g2b2g3b3r '^(.* ([0-9]{4})-([0-9]{2})-([0-9]{2}))'
        Example : -Jm2:d3:Y4,3R1g1g1g1g1g3R '^(.* ([0-9]{2})/([0-9]{2})/([0-9]{4}))'
        Example : -JY2:m3:d4 '^(.* ([0-9]{4})-([0-9]{2})-([0-9]{2}))'
  -T  : Colorize string according to specified thresholds values
        Syntax for thresholds option : -Ts[x],s1[:c1],s2[:c2]...s10[:c10]
         where :
           s is a number from 0 to 9 indicating the selection regexp number,
           x indicates that thresholds are in hexadecimal
           si : value of the i-th threshold for 1 <= i <= 10
           c1 : color of the i-th range     for 1 <= i <= 10
        Example : -T1,0,10,50,70,95,100  '(([0-9]+)% .*)'
        Example : -T1,0:2b,10:2g,50:2y,70:3y,95:3r,100:3R  '(([0-9]+)% .*)'
  -t  : Colorize string according to time periods
        Syntax for time periods option : -tp[0][R][:num][:spec][,c1c2...c10]
         where :
           p is a time period specifier in [YmwdHMSun]
             with the following meaning :
               Y : year
               m : month
               w : week
               d : day
               H : hour
               M : minute
               S : second
               u : micro-second
               n : nano-second
           0 tells that the date must be framed at the beginning of the period
           R is an optional flag telling to use an optional time reference
             instead of the current time. The optional time reference must be
             specified before the regex argument
           num is an optional number of time periods (default is 1)
           spec is a string specifying the position of date elements,
             composed of letters in [YmbdHMSus], each one followed by the
             number of the sub-regex it is associated to,
             with the following meaning :
               Y : year
               m : month
               b : abbreviated month name
               w : week
               d : day
               H : hour
               M : minute
               S : second
               u : micro-second
               n : nano-second
         and c1c2...c7 are the optional color specifiers for the time periods
  -N  : consistent numbering of sub-expressions in -A/-I and -s
  -p  : display configuration(s) matching glob-like expression (pattern)
  -P  : display configuration(s) matching regexp
  -x  : display options count for each config (with -vH options)
Buffer size = 64 Ko
Environment variable HL_CONF         =  "/home/machine/mb/.hl.cfg:/home/machine/mb/hl_conf:/DATA3/projets/hl/config_files"
Environment variable HL_CONF_GLOB    =  "eh_hl_*.cfg:hl_*.cfg:hl.cfg:.hl_*.cfg:.hl.cfg"
Environment variable HL_DOW_SPEC     is undefined. Default value = "Y2m3d4".
Environment variable HL_DOW_REGEX    is undefined. Default value = "(([0-9]{4})-([0-9]{2})-([0-9]{2}))".
Environment variable HL_TIME_REGEX_Y is undefined. Default value = "(([0-9]{4}))".
Environment variable HL_TIME_REGEX_m is undefined. Default value = "(([0-9]{4})-([0-9]{2}))".
Environment variable HL_TIME_REGEX_d is undefined. Default value = "(([0-9]{4})-([0-9]{2})-([0-9]{2}))".
Environment variable HL_TIME_REGEX_H is undefined. Default value = "(([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}))".
Environment variable HL_TIME_REGEX_M is undefined. Default value = "(([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}))".
Environment variable HL_TIME_REGEX_S is undefined. Default value = "(([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2}))".
Environment variable HL_TIME_REGEX_u is undefined. Default value = "(([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})\.([0-9]{6}))".
Environment variable HL_TIME_REGEX_n is undefined. Default value = "(([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})\.([0-9]{9}))".
Environment variable HL_TIME_SPEC_Y  is undefined. Default value = "Y2".
Environment variable HL_TIME_SPEC_m  is undefined. Default value = "Y2m3".
Environment variable HL_TIME_SPEC_d  is undefined. Default value = "Y2m3d4".
Environment variable HL_TIME_SPEC_H  is undefined. Default value = "Y2m3d4H5".
Environment variable HL_TIME_SPEC_M  is undefined. Default value = "Y2m3d4H5M6".
Environment variable HL_TIME_SPEC_S  is undefined. Default value = "Y2m3d4H5M6S7".
Environment variable HL_TIME_SPEC_u  is undefined. Default value = "Y2m3d4H5M6S7u8".
Environment variable HL_TIME_SPEC_n  is undefined. Default value = "Y2m3d4H5M6S7n8".
Environment variable HL_THRES_REGEX  is undefined. Default value = "([0-9]+)".

Environment variable HL_DEFAULT      is undefined. Default value = "3Y".

Environment variable HL_A1           is undefined. Default value = "2B".
Environment variable HL_A2           is undefined. Default value = "3c".

Environment variable HL_SUNDAY       is undefined. Default value = "3R".
Environment variable HL_MONDAY       is undefined. Default value = "2b".
Environment variable HL_TUESDAY      is undefined. Default value = "2c".
Environment variable HL_WEDNESDAY    is undefined. Default value = "2g".
Environment variable HL_THURSDAY     is undefined. Default value = "3g".
Environment variable HL_FRIDAY       is undefined. Default value = "3y".
Environment variable HL_SATURDAY     is undefined. Default value = "3r".

Environment variable HL_TIME_0       is undefined. Default value = "3w".
Environment variable HL_TIME_1       is undefined. Default value = "3m".
Environment variable HL_TIME_2       is undefined. Default value = "3r".
Environment variable HL_TIME_3       is undefined. Default value = "2y".
Environment variable HL_TIME_4       is undefined. Default value = "3y".
Environment variable HL_TIME_5       is undefined. Default value = "3g".
Environment variable HL_TIME_6       is undefined. Default value = "2g".
Environment variable HL_TIME_7       is undefined. Default value = "3c".
Environment variable HL_TIME_8       is undefined. Default value = "3b".
Environment variable HL_TIME_9       is undefined. Default value = "2b".

Environment variable HL_T_2_1        is undefined. Default value = "3g".
Environment variable HL_T_2_2        is undefined. Default value = "3r".

Environment variable HL_T_3_1        is undefined. Default value = "3g".
Environment variable HL_T_3_2        is undefined. Default value = "3y".
Environment variable HL_T_3_3        is undefined. Default value = "3r".

Environment variable HL_T_4_1        is undefined. Default value = "3b".
Environment variable HL_T_4_2        is undefined. Default value = "3g".
Environment variable HL_T_4_3        is undefined. Default value = "3y".
Environment variable HL_T_4_4        is undefined. Default value = "3r".

Environment variable HL_T_5_1        is undefined. Default value = "3b".
Environment variable HL_T_5_2        is undefined. Default value = "3g".
Environment variable HL_T_5_3        is undefined. Default value = "3y".
Environment variable HL_T_5_4        is undefined. Default value = "2r".
Environment variable HL_T_5_5        is undefined. Default value = "3r".

Environment variable HL_T_6_1        is undefined. Default value = "3b".
Environment variable HL_T_6_2        is undefined. Default value = "3c".
Environment variable HL_T_6_3        is undefined. Default value = "3g".
Environment variable HL_T_6_4        is undefined. Default value = "3y".
Environment variable HL_T_6_5        is undefined. Default value = "3r".
Environment variable HL_T_6_6        is undefined. Default value = "3w".

Environment variable HL_T_7_1        is undefined. Default value = "3b".
Environment variable HL_T_7_2        is undefined. Default value = "3c".
Environment variable HL_T_7_3        is undefined. Default value = "3g".
Environment variable HL_T_7_4        is undefined. Default value = "3y".
Environment variable HL_T_7_5        is undefined. Default value = "3r".
Environment variable HL_T_7_6        is undefined. Default value = "3m".
Environment variable HL_T_7_7        is undefined. Default value = "3w".

Environment variable HL_T_8_1        is undefined. Default value = "2b".
Environment variable HL_T_8_2        is undefined. Default value = "3b".
Environment variable HL_T_8_3        is undefined. Default value = "3c".
Environment variable HL_T_8_4        is undefined. Default value = "3g".
Environment variable HL_T_8_5        is undefined. Default value = "3y".
Environment variable HL_T_8_6        is undefined. Default value = "3r".
Environment variable HL_T_8_7        is undefined. Default value = "3m".
Environment variable HL_T_8_8        is undefined. Default value = "3w".

Environment variable HL_T_9_1        is undefined. Default value = "2b".
Environment variable HL_T_9_2        is undefined. Default value = "3b".
Environment variable HL_T_9_3        is undefined. Default value = "3c".
Environment variable HL_T_9_4        is undefined. Default value = "2g".
Environment variable HL_T_9_5        is undefined. Default value = "3g".
Environment variable HL_T_9_6        is undefined. Default value = "3y".
Environment variable HL_T_9_7        is undefined. Default value = "3r".
Environment variable HL_T_9_8        is undefined. Default value = "3m".
Environment variable HL_T_9_9        is undefined. Default value = "3w".

Environment variable HL_T_10_1       is undefined. Default value = "2b".
Environment variable HL_T_10_2       is undefined. Default value = "3b".
Environment variable HL_T_10_3       is undefined. Default value = "3c".
Environment variable HL_T_10_4       is undefined. Default value = "2g".
Environment variable HL_T_10_5       is undefined. Default value = "3g".
Environment variable HL_T_10_6       is undefined. Default value = "3y".
Environment variable HL_T_10_7       is undefined. Default value = "2y".
Environment variable HL_T_10_8       is undefined. Default value = "3r".
Environment variable HL_T_10_9       is undefined. Default value = "3m".
Environment variable HL_T_10_10      is undefined. Default value = "3w".

Environment variable HL_T_V_1        is undefined. Default value =               0
Environment variable HL_T_V_2        is undefined. Default value =              10
Environment variable HL_T_V_3        is undefined. Default value =              20
Environment variable HL_T_V_4        is undefined. Default value =              30
Environment variable HL_T_V_5        is undefined. Default value =              40
Environment variable HL_T_V_6        is undefined. Default value =              50
Environment variable HL_T_V_7        is undefined. Default value =              60
Environment variable HL_T_V_8        is undefined. Default value =              70
Environment variable HL_T_V_9        is undefined. Default value =              80
Environment variable HL_T_V_10       is undefined. Default value =             100
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

Colorize dates based on the day of the week :
```
$ ./dates_gen | hl --dow
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/dates_gen_colorized.png)

Colorize dates based on the working day or the week-end :
```
$ ./dates_gen | hl --dow_WE
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/dates_gen_week-end.png)

Colorize files modification dates based on the day of the week :
```
$ /bin/ls -ltr --full-time /bin | hl --dow_A
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/ls_bin_dow_colors.png)

Colorize hl configurations :
```
$ hl -vop '*' | hl_conf
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_conf_example.png)

Colorize dates according to the day of the week :
```
$ cat dpkg.log | hl -J
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-J_log_DOW.png)

Colorize numbers according to thresholds :
```
$ ./num_gen | hl -T1,0,30,60,90,100,120,140 '([0-9]+)'
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-T_num.png)

Colorize numbers with 2 thresholds, using default colors and regex :
```
$ ./num_gen_100 | hl -T1,0,50
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-T_num_2.png)

Colorize numbers with 3 thresholds, using default colors and regex :
```
$ ./num_gen_100 | hl -T1,0,50,80
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-T_num_3.png)

Colorize numbers with 5 thresholds, using default colors and regex :
```
$ ./num_gen_100 | hl -T1,0,25,50,75,100
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-T_num_5.png)

Colorize numbers with 10 thresholds, using default colors and regex :
```
$ ./num_gen_100 | hl -T1,0,11,22,33,44,56,67,78,89,100
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-T_num_10.png)

Colorize dates according to age, in minutes :
```
$ ./dates_gen -M | hl -tM
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-tM.png)

Colorize dates according to age, in weeks, with a time reference :
```
$ ./dates_gen -w '2000-06-23 12:34:56.123456789' | hl -twR '2000-06-23 12:34:56.123456789'
```
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_-twR.png)

New environment variables for option -t :
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_env_-t.png)

Enhanced display for hl_tool -M (configurations checksums) :
![Alternate](https://github.com/mbornet-hl/hl/blob/master/images/hl_tool_-M_example.png)

Extracting information from standard input (for advanced users)
---------------------------------------------------------------
**hl** can also be used to extract information from standard input. The idea is to send to **stderr** information that should normally have been colorized. It's somewhat like a **grep -o** that is able to extract **parts of lines**.
To better understand, run the following command :
```
$ man 5 acl | hl -ei -E3n '\<acl_[^(]+\(3\)' 2>&1 > /dev/null  | sed 's/,/\n/g' | sort -u
acl_add_perm(3)
acl_calc_mask(3)
acl_check(3)
acl_clear_perms(3)
acl_cmp(3)
acl_copy_entry(3)
acl_copy_ext(3)
acl_copy_int(3)
acl_create_entry(3)
acl_delete_def_file(3)
acl_delete_entry(3)
acl_delete_perm(3)
acl_dup(3)
acl_entries(3)
acl_equiv_mode(3)
acl_error(3)
acl_extended_fd(3)
acl_extended_file(3)
acl_extended_file_nofollow(3)
acl_free(3)
acl_from_mode(3)
acl_from_text(3)
acl_get_entry(3)
acl_get_fd(3)
acl_get_file(3)
acl_get_perm(3)
acl_get_permset(3)
acl_get_qualifier(3)
acl_get_tag_type(3)
acl_init(3)
acl_set_fd(3)
acl_set_file(3)
acl_set_permset(3)
acl_set_qualifier(3)
acl_set_tag_type(3)
acl_size(3)
acl_to_any_text(3)
acl_to_text(3)
acl_valid(3)
```
then run the following command and try to locate the above functions in the man page :
```
$ man 5 acl | less -RX
```
and finally run the command :
```
$ man 5 acl | hl -ei -3c '\<acl_[^(]+\(3\)' | less -RX
```
You can see that all strings colorized in high cyan have been extracted, sent to stderr, then post-processed to display them one per line.

The **-E** option allows you to extract selected strings by redirecting them to **stderr**.
