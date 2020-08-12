hl : a colorization command
===========================

Purpose
-------
**In short** :

**hl** is a binary program compiled from **C** and **lex** source which can greatly **help you read log files**, the **output of commands** or **scripts**, **configuration files**, **text files**. It can **highlight thresholds**, **colorize blocks of text** delimited by markers, **alternate colors** when the value of an element **changes** (or on the contrary when it **does not change**), check values **consistency** on a line, **colorize fields** of text, and so on. **Many configurations** have been created, and you can **easily create yours** to suit your needs, using **simple text strings** or **regular expressions**.

Its purpose is to colorize **what is important in the text you read**. It has been designed to **help you get straight to the point**. It's **fast** and **efficient**.

There are no need to add plug-ins when you want to colorize a new syntax : you just have to define a new configuration with **basic** or **extended** **regular expressions**. You don't have to install the binary nor the configuration files in a system directory, you can use environment variables to specify where to fetch the files.

At the present time, it has only been included in the ArchLinux distribution. There are no RPM nor .deb package, if you want to use it, you'll have to compile it for your system (which is very easy) or just use the binary on this web site. By the way, if you want to create an RPM or .deb package for it, you're welcome !

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

Here is the [first review](https://connect.ed-diamond.com/Linux-Pratique/LP-093/Colorisez-vos-textes-avec-la-commande-hl) about `hl` in a french magazine (Linux Pratique), and here is the [second one](https://connect.ed-diamond.com/Linux-Pratique/LPHS-046/Tirez-parti-de-la-colorisation-pour-faciliter-la-lecture-de-vos-donnees).

Usage
-----

```
hl: version 1.91
Usage: hl [-h|-H|-V|-[[%.]eiuvdDEL1234][-[rgybmcwRGYBMCWnAI] regexp ...][--config_name ...] ]
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
Environment variable HL_DEFAULT is undefined. Default value = "3Y".
Environment variable HL_A1 is undefined. Default value = "2B".
Environment variable HL_A2 is undefined. Default value = "1n".
Environment variable HL_CONF        = "/DATA3/projets/hl/config_files:/home/machine/mb:/etc/default/hl"
Environment variable HL_CONF_GLOB is undefined. Default value = "hl_*.cfg:hl.cfg:.hl_*.cfg:.hl.cfg".
  Configurations :
/DATA3/projets/hl/config_files/hl_C.cfg            : C
/DATA3/projets/hl/config_files/hl_HW.cfg           : dmidecode
/DATA3/projets/hl/config_files/hl_HW.cfg           : show_disks
/DATA3/projets/hl/config_files/hl_HW.cfg           : show_all_disks
/DATA3/projets/hl/config_files/hl_acl.cfg          : acl
/DATA3/projets/hl/config_files/hl_cmds.cfg         : apt-get
/DATA3/projets/hl/config_files/hl_cmds.cfg         : jigdo
/DATA3/projets/hl/config_files/hl_cmds.cfg         : ll
/DATA3/projets/hl/config_files/hl_cmds.cfg         : ls_doc
/DATA3/projets/hl/config_files/hl_cmds.cfg         : man
/DATA3/projets/hl/config_files/hl_cmds.cfg         : mpg123
/DATA3/projets/hl/config_files/hl_cmds.cfg         : namei
/DATA3/projets/hl/config_files/hl_cmds.cfg         : nm
/DATA3/projets/hl/config_files/hl_cmds.cfg         : non_text
/DATA3/projets/hl/config_files/hl_cmds.cfg         : objdump
/DATA3/projets/hl/config_files/hl_cmds.cfg         : samba
/DATA3/projets/hl/config_files/hl_cmds.cfg         : sc
/DATA3/projets/hl/config_files/hl_cmds.cfg         : xxd
/DATA3/projets/hl/config_files/hl_cr.cfg           : cr
/DATA3/projets/hl/config_files/hl_cr.cfg           : ref_list
/DATA3/projets/hl/config_files/hl_cr.cfg           : cm
/DATA3/projets/hl/config_files/hl_date.cfg         : cal
/DATA3/projets/hl/config_files/hl_date.cfg         : date
/DATA3/projets/hl/config_files/hl_date.cfg         : ymdHMS
/DATA3/projets/hl/config_files/hl_date.cfg         : mdHMS
/DATA3/projets/hl/config_files/hl_date.cfg         : dHMS
/DATA3/projets/hl/config_files/hl_date.cfg         : dHMS_debug
/DATA3/projets/hl/config_files/hl_diff.cfg         : diff_tree
/DATA3/projets/hl/config_files/hl_diff.cfg         : diff_tree2
/DATA3/projets/hl/config_files/hl_diff.cfg         : diff_tree2
/DATA3/projets/hl/config_files/hl_errors.cfg       : error
/DATA3/projets/hl/config_files/hl_errors.cfg       : err
/DATA3/projets/hl/config_files/hl_errors.cfg       : errs
/DATA3/projets/hl/config_files/hl_errors.cfg       : errors
/DATA3/projets/hl/config_files/hl_errors.cfg       : error_kw
/DATA3/projets/hl/config_files/hl_errors.cfg       : warning_kw
/DATA3/projets/hl/config_files/hl_errors.cfg       : remark_kw
/DATA3/projets/hl/config_files/hl_errors.cfg       : warning
/DATA3/projets/hl/config_files/hl_errors.cfg       : remark
/DATA3/projets/hl/config_files/hl_files.cfg        : hosts
/DATA3/projets/hl/config_files/hl_files.cfg        : passwd
/DATA3/projets/hl/config_files/hl_files.cfg        : passwd_chk
/DATA3/projets/hl/config_files/hl_firewall.cfg     : iptables
/DATA3/projets/hl/config_files/hl_hl.cfg           : colors
/DATA3/projets/hl/config_files/hl_hl.cfg           : colors42
/DATA3/projets/hl/config_files/hl_hl.cfg           : color_names
/DATA3/projets/hl/config_files/hl_hl.cfg           : hl
/DATA3/projets/hl/config_files/hl_hl.cfg           : hl_conf
/DATA3/projets/hl/config_files/hl_hl.cfg           : hl_debug
/DATA3/projets/hl/config_files/hl_hl.cfg           : hl_usage
/DATA3/projets/hl/config_files/hl_hl.cfg           : hi_red
/DATA3/projets/hl/config_files/hl_hl.cfg           : hi_green
/DATA3/projets/hl/config_files/hl_hl.cfg           : hi_yellow
/DATA3/projets/hl/config_files/hl_hl.cfg           : hi_blue
/DATA3/projets/hl/config_files/hl_hl.cfg           : hi_magenta
/DATA3/projets/hl/config_files/hl_hl.cfg           : hi_cyan
/DATA3/projets/hl/config_files/hl_hl.cfg           : hi_white
/DATA3/projets/hl/config_files/hl_hl.cfg           : dim_red
/DATA3/projets/hl/config_files/hl_hl.cfg           : dim_green
/DATA3/projets/hl/config_files/hl_hl.cfg           : dim_yellow
/DATA3/projets/hl/config_files/hl_hl.cfg           : dim_blue
/DATA3/projets/hl/config_files/hl_hl.cfg           : dim_magenta
/DATA3/projets/hl/config_files/hl_hl.cfg           : dim_cyan
/DATA3/projets/hl/config_files/hl_hl.cfg           : dim_white
/DATA3/projets/hl/config_files/hl_hl.cfg           : red
/DATA3/projets/hl/config_files/hl_hl.cfg           : green
/DATA3/projets/hl/config_files/hl_hl.cfg           : yellow
/DATA3/projets/hl/config_files/hl_hl.cfg           : blue
/DATA3/projets/hl/config_files/hl_hl.cfg           : magenta
/DATA3/projets/hl/config_files/hl_hl.cfg           : cyan
/DATA3/projets/hl/config_files/hl_hl.cfg           : white
/DATA3/projets/hl/config_files/hl_log.cfg          : log_d
/DATA3/projets/hl/config_files/hl_log.cfg          : log_d_debug
/DATA3/projets/hl/config_files/hl_log.cfg          : log_dHMS
/DATA3/projets/hl/config_files/hl_network.cfg      : eth
/DATA3/projets/hl/config_files/hl_network.cfg      : eth_VIP
/DATA3/projets/hl/config_files/hl_network.cfg      : ethtool
/DATA3/projets/hl/config_files/hl_network.cfg      : eth_cpt
/DATA3/projets/hl/config_files/hl_network.cfg      : fail2ban
/DATA3/projets/hl/config_files/hl_network.cfg      : ifconfig
/DATA3/projets/hl/config_files/hl_network.cfg      : IP
/DATA3/projets/hl/config_files/hl_network.cfg      : ip
/DATA3/projets/hl/config_files/hl_network.cfg      : MAC
/DATA3/projets/hl/config_files/hl_network.cfg      : netstat
/DATA3/projets/hl/config_files/hl_network.cfg      : ping
/DATA3/projets/hl/config_files/hl_network.cfg      : tcpdump
/DATA3/projets/hl/config_files/hl_network.cfg      : validate_IP
/DATA3/projets/hl/config_files/hl_perso.cfg        : rc
/DATA3/projets/hl/config_files/hl_perso.cfg        : out_err
/DATA3/projets/hl/config_files/hl_perso.cfg        : show_disks
/DATA3/projets/hl/config_files/hl_rpn.cfg          : rpn
/DATA3/projets/hl/config_files/hl_rpn.cfg          : rpn_op_id
/DATA3/projets/hl/config_files/hl_sh.cfg           : sh
/DATA3/projets/hl/config_files/hl_stat.cfg         : iostat
/DATA3/projets/hl/config_files/hl_stat.cfg         : zabbix_netstat
/DATA3/projets/hl/config_files/hl_system.cfg       : chkconfig
/DATA3/projets/hl/config_files/hl_system.cfg       : df
/DATA3/projets/hl/config_files/hl_system.cfg       : dpkg-query
/DATA3/projets/hl/config_files/hl_system.cfg       : dmidecode
/DATA3/projets/hl/config_files/hl_system.cfg       : fdisk
/DATA3/projets/hl/config_files/hl_system.cfg       : free
/DATA3/projets/hl/config_files/hl_system.cfg       : free_used_swap
/DATA3/projets/hl/config_files/hl_system.cfg       : iostat
/DATA3/projets/hl/config_files/hl_system.cfg       : ps
/DATA3/projets/hl/config_files/hl_system.cfg       : ps_cpu
/DATA3/projets/hl/config_files/hl_system.cfg       : ps_cpu_time
/DATA3/projets/hl/config_files/hl_system.cfg       : smartctl
/DATA3/projets/hl/config_files/hl_system.cfg       : strace
/DATA3/projets/hl/config_files/hl_system.cfg       : stty
/DATA3/projets/hl/config_files/hl_system.cfg       : swap
/DATA3/projets/hl/config_files/hl_system.cfg       : on-off
/DATA3/projets/hl/config_files/hl_system.cfg       : chkconfig
/DATA3/projets/hl/config_files/hl_system.cfg       : on-off
/DATA3/projets/hl/config_files/hl_system.cfg       : service
/DATA3/projets/hl/config_files/hl_system.cfg       : services
/DATA3/projets/hl/config_files/hl_system.cfg       : smartctl
/DATA3/projets/hl/config_files/hl_system.cfg       : strace
/DATA3/projets/hl/config_files/hl_system.cfg       : stty
/DATA3/projets/hl/config_files/hl_system.cfg       : w
/DATA3/projets/hl/config_files/hl_vm.cfg           : virsh_list
/DATA3/projets/hl/config_files/hl_za.cfg           : za
/DATA3/projets/hl/config_files/hl_za.cfg           : za0
/DATA3/projets/hl/config_files/hl_za.cfg           : za_conf
/DATA3/projets/hl/config_files/hl_za.cfg           : zalc
/DATA3/projets/hl/config_files/hl_za.cfg           : za_ls
/DATA3/projets/hl/config_files/hl_za.cfg           : za_usage
/DATA3/projets/hl/config_files/hl_za.cfg           : za_videos
/DATA3/projets/hl/config_files/hl_za.cfg           : za_tty
/DATA3/projets/hl/config_files/hl_za.cfg           : za_tty_no_ref
/DATA3/projets/hl/config_files/.hl.cfg             : acl
/DATA3/projets/hl/config_files/.hl.cfg             : apt
/DATA3/projets/hl/config_files/.hl.cfg             : C
/DATA3/projets/hl/config_files/.hl.cfg             : cal
/DATA3/projets/hl/config_files/.hl.cfg             : colors
/DATA3/projets/hl/config_files/.hl.cfg             : colors42
/DATA3/projets/hl/config_files/.hl.cfg             : color_names
/DATA3/projets/hl/config_files/.hl.cfg             : df
/DATA3/projets/hl/config_files/.hl.cfg             : diff
/DATA3/projets/hl/config_files/.hl.cfg             : dpkg-query
/DATA3/projets/hl/config_files/.hl.cfg             : err
/DATA3/projets/hl/config_files/.hl.cfg             : eth
/DATA3/projets/hl/config_files/.hl.cfg             : ethtool
/DATA3/projets/hl/config_files/.hl.cfg             : eth_VIP
/DATA3/projets/hl/config_files/.hl.cfg             : fail2ban
/DATA3/projets/hl/config_files/.hl.cfg             : fdisk
/DATA3/projets/hl/config_files/.hl.cfg             : free
/DATA3/projets/hl/config_files/.hl.cfg             : ha_log
/DATA3/projets/hl/config_files/.hl.cfg             : heartbeat
/DATA3/projets/hl/config_files/.hl.cfg             : hl
/DATA3/projets/hl/config_files/.hl.cfg             : hosts
/DATA3/projets/hl/config_files/.hl.cfg             : ifconfig
/DATA3/projets/hl/config_files/.hl.cfg             : IP
/DATA3/projets/hl/config_files/.hl.cfg             : ip
/DATA3/projets/hl/config_files/.hl.cfg             : iptables
/DATA3/projets/hl/config_files/.hl.cfg             : jigdo
/DATA3/projets/hl/config_files/.hl.cfg             : ls_doc
/DATA3/projets/hl/config_files/.hl.cfg             : MAC
/DATA3/projets/hl/config_files/.hl.cfg             : namei
/DATA3/projets/hl/config_files/.hl.cfg             : netstat
/DATA3/projets/hl/config_files/.hl.cfg             : passwd
/DATA3/projets/hl/config_files/.hl.cfg             : passwd_chk
/DATA3/projets/hl/config_files/.hl.cfg             : percent
/DATA3/projets/hl/config_files/.hl.cfg             : ps_cpu
/DATA3/projets/hl/config_files/.hl.cfg             : ps_cpu_time
/DATA3/projets/hl/config_files/.hl.cfg             : ps
/DATA3/projets/hl/config_files/.hl.cfg             : rev_color_names
/DATA3/projets/hl/config_files/.hl.cfg             : samba
/DATA3/projets/hl/config_files/.hl.cfg             : sep3
/DATA3/projets/hl/config_files/.hl.cfg             : services
/DATA3/projets/hl/config_files/.hl.cfg             : show_disks
/DATA3/projets/hl/config_files/.hl.cfg             : tcpdump
/DATA3/projets/hl/config_files/.hl.cfg             : validate_IP
/DATA3/projets/hl/config_files/.hl.cfg             : virsh_list
/DATA3/projets/hl/config_files/.hl.cfg             : w
/DATA3/projets/hl/config_files/.hl.cfg             : xxd
/DATA3/projets/hl/config_files/.hl.cfg             : hi_red
/DATA3/projets/hl/config_files/.hl.cfg             : hi_green
/DATA3/projets/hl/config_files/.hl.cfg             : hi_yellow
/DATA3/projets/hl/config_files/.hl.cfg             : hi_blue
/DATA3/projets/hl/config_files/.hl.cfg             : hi_magenta
/DATA3/projets/hl/config_files/.hl.cfg             : hi_cyan
/DATA3/projets/hl/config_files/.hl.cfg             : hi_white
/DATA3/projets/hl/config_files/.hl.cfg             : dim_red
/DATA3/projets/hl/config_files/.hl.cfg             : dim_green
/DATA3/projets/hl/config_files/.hl.cfg             : dim_yellow
/DATA3/projets/hl/config_files/.hl.cfg             : dim_blue
/DATA3/projets/hl/config_files/.hl.cfg             : dim_magenta
/DATA3/projets/hl/config_files/.hl.cfg             : dim_cyan
/DATA3/projets/hl/config_files/.hl.cfg             : dim_white
/DATA3/projets/hl/config_files/.hl.cfg             : red
/DATA3/projets/hl/config_files/.hl.cfg             : green
/DATA3/projets/hl/config_files/.hl.cfg             : yellow
/DATA3/projets/hl/config_files/.hl.cfg             : blue
/DATA3/projets/hl/config_files/.hl.cfg             : magenta
/DATA3/projets/hl/config_files/.hl.cfg             : cyan
/DATA3/projets/hl/config_files/.hl.cfg             : white
/home/machine/mb/.hl.cfg                           : errs
/home/machine/mb/.hl.cfg                           : errors
/home/machine/mb/.hl.cfg                           : error_kw
/home/machine/mb/.hl.cfg                           : warning_kw
/home/machine/mb/.hl.cfg                           : remark_kw
/home/machine/mb/.hl.cfg                           : error
/home/machine/mb/.hl.cfg                           : warning
/home/machine/mb/.hl.cfg                           : remark
/home/machine/mb/.hl.cfg                           : cr
/home/machine/mb/.hl.cfg                           : dmidecode
/home/machine/mb/.hl.cfg                           : local
/home/machine/mb/.hl.cfg                           : netstat
/home/machine/mb/.hl.cfg                           : show_disks
/home/machine/mb/.hl.cfg                           : show_all_disks
/home/machine/mb/.hl.cfg                           : za0
/home/machine/mb/.hl.cfg                           : za_conf
/home/machine/mb/.hl.cfg                           : zalc
/home/machine/mb/.hl.cfg                           : za_usage
/home/machine/mb/.hl.cfg                           : za_videos
/home/machine/mb/.hl.cfg                           : za
/home/machine/mb/.hl.cfg                           : alpha
/home/machine/mb/.hl.cfg                           : beta
/home/machine/mb/.hl.cfg                           : alphabeta
/home/machine/mb/.hl.cfg                           : alpha2
/home/machine/mb/.hl.cfg                           : beta
/home/machine/mb/.hl.cfg                           : alphabeta
/home/machine/mb/.hl.cfg                           : alphabeta2
/home/machine/mb/.hl.cfg                           : col_dupl
/home/machine/mb/.hl.cfg                           : free_used_swap
/home/machine/mb/.hl.cfg                           : swap
/home/machine/mb/.hl.cfg                           : p
/home/machine/mb/.hl.cfg                           : test
/home/machine/mb/.hl.cfg                           : esc0m
/home/machine/mb/.hl.cfg                           : ifconfig
/home/machine/mb/.hl.cfg                           : iostat
/home/machine/mb/.hl.cfg                           : zabbix_netstat
/home/machine/mb/.hl.cfg                           : sh
/home/machine/mb/.hl.cfg                           : do_done1
/home/machine/mb/.hl.cfg                           : do_done2
/home/machine/mb/.hl.cfg                           : strace
/home/machine/mb/.hl.cfg                           : fun
/home/machine/mb/.hl.cfg                           : rc
/home/machine/mb/.hl.cfg                           : ref_list
/home/machine/mb/.hl.cfg                           : cm
/home/machine/mb/.hl.cfg                           : mpg123
/home/machine/mb/.hl.cfg                           : out_err
/home/machine/mb/.hl.cfg                           : chkterm
/home/machine/mb/.hl.cfg                           : stty
/home/machine/mb/.hl.cfg                           : hdr_sql
/home/machine/mb/.hl.cfg                           : DOMAIN
/home/machine/mb/.hl.cfg                           : FTP
/home/machine/mb/.hl.cfg                           : MAIL
/home/machine/mb/.hl.cfg                           : MX
/home/machine/mb/.hl.cfg                           : NS
/home/machine/mb/.hl.cfg                           : POP
/home/machine/mb/.hl.cfg                           : SMTP
/home/machine/mb/.hl.cfg                           : STAR
/home/machine/mb/.hl.cfg                           : VALEM
/home/machine/mb/.hl.cfg                           : WWW
/home/machine/mb/.hl.cfg                           : pass
/home/machine/mb/.hl.cfg                           : fields
/home/machine/mb/.hl.cfg                           : ll
/home/machine/mb/.hl.cfg                           : quote1
/home/machine/mb/.hl.cfg                           : quote2
/home/machine/mb/.hl.cfg                           : quote1N
/home/machine/mb/.hl.cfg                           : quote2N
/home/machine/mb/.hl.cfg                           : passwd
/home/machine/mb/.hl.cfg                           : hl_debug
/home/machine/mb/.hl.cfg                           : umpkeys
/home/machine/mb/.hl.cfg                           : date
/home/machine/mb/.hl.cfg                           : auth
/home/machine/mb/.hl.cfg                           : whois
/home/machine/mb/.hl.cfg                           : objdump
/home/machine/mb/.hl.cfg                           : C01
/home/machine/mb/.hl.cfg                           : C10
/home/machine/mb/.hl.cfg                           : C001
/home/machine/mb/.hl.cfg                           : C110
/home/machine/mb/.hl.cfg                           : hosts
/home/machine/mb/.hl.cfg                           : C_chk
/home/machine/mb/.hl.cfg                           : etc_motd
/home/machine/mb/.hl.cfg                           : etc_motd2
/home/machine/mb/.hl.cfg                           : hl
/home/machine/mb/.hl.cfg                           : smartctl
/home/machine/mb/.hl.cfg                           : diff_tty
/home/machine/mb/.hl.cfg                           : za_ls
/home/machine/mb/.hl.cfg                           : AT
/home/machine/mb/.hl.cfg                           : on-off
/home/machine/mb/.hl.cfg                           : chkconfig
/home/machine/mb/.hl.cfg                           : service
/home/machine/mb/.hl.cfg                           : atride
/home/machine/mb/.hl.cfg                           : methods
/home/machine/mb/.hl.cfg                           : extract_methods
/home/machine/mb/.hl.cfg                           : za_tty
/home/machine/mb/.hl.cfg                           : za_tty_no_ref
/home/machine/mb/.hl.cfg                           : partie
/home/machine/mb/.hl.cfg                           : sqware
/home/machine/mb/.hl.cfg                           : diff_tree
/home/machine/mb/.hl.cfg                           : diff_tree2
/home/machine/mb/.hl.cfg                           : diff_tree2
/home/machine/mb/.hl.cfg                           : json
/home/machine/mb/.hl.cfg                           : eth_cpt
/home/machine/mb/.hl.cfg                           : adm_help
/home/machine/mb/.hl.cfg                           : iozone_diff
/home/machine/mb/.hl.cfg                           : rpn
/home/machine/mb/.hl.cfg                           : rpn_op_id
/home/machine/mb/.hl.cfg                           : MNIST
/home/machine/mb/.hl.cfg                           : ping
/home/machine/mb/.hl.cfg                           : ymdHMS
/home/machine/mb/.hl.cfg                           : mdHMS
/home/machine/mb/.hl.cfg                           : dHMS
/home/machine/mb/.hl.cfg                           : dHMS_debug
/home/machine/mb/.hl.cfg                           : log_d
/home/machine/mb/.hl.cfg                           : log_d_debug
/home/machine/mb/.hl.cfg                           : log_dHMS
/home/machine/mb/.hl.cfg                           : xxd
/home/machine/mb/.hl.cfg                           : non_text
/home/machine/mb/.hl.cfg                           : sc
/home/machine/mb/.hl.cfg                           : alt_all
/home/machine/mb/.hl.cfg                           : alt_min
/home/machine/mb/.hl.cfg                           : alt_sec
/home/machine/mb/.hl.cfg                           : test_A
/home/machine/mb/.hl.cfg                           : test_A_sec
/home/machine/mb/.hl.cfg                           : test_A_sec_partial
/home/machine/mb/.hl.cfg                           : test_I
/home/machine/mb/.hl.cfg                           : test_I_level1
/home/machine/mb/.hl.cfg                           : test_selector
/home/machine/mb/.hl.cfg                           : A0
/home/machine/mb/.hl.cfg                           : A1
/home/machine/mb/.hl.cfg                           : A2
/home/machine/mb/.hl.cfg                           : A3
/home/machine/mb/.hl.cfg                           : A4
/home/machine/mb/.hl.cfg                           : I1
/home/machine/mb/.hl.cfg                           : I2
/home/machine/mb/.hl.cfg                           : I3
/home/machine/mb/.hl.cfg                           : I4
/home/machine/mb/.hl.cfg                           : FA0
/home/machine/mb/.hl.cfg                           : FA1
/home/machine/mb/.hl.cfg                           : FA2
/home/machine/mb/.hl.cfg                           : FA3
/home/machine/mb/.hl.cfg                           : FA4
/home/machine/mb/.hl.cfg                           : FI1
/home/machine/mb/.hl.cfg                           : FI2
/home/machine/mb/.hl.cfg                           : FI3
/home/machine/mb/.hl.cfg                           : FI4
/home/machine/mb/.hl.cfg                           : lsA
/home/machine/mb/.hl.cfg                           : nm
/home/machine/mb/.hl.cfg                           : used_modules
/home/machine/mb/.hl.cfg                           : srv
/home/machine/mb/.hl.cfg                           : sep3
/home/machine/mb/.hl.cfg                           : sep4
/home/machine/mb/.hl.cfg                           : HOME_HL_CFG
/etc/default/hl                                    : apt-get
/etc/default/hl                                    : cal
/etc/default/hl                                    : chkconfig
/etc/default/hl                                    : df
/etc/default/hl                                    : diff
/etc/default/hl                                    : dpkg-query
/etc/default/hl                                    : dmidecode
/etc/default/hl                                    : ethtool
/etc/default/hl                                    : fdisk
/etc/default/hl                                    : free
/etc/default/hl                                    : ifconfig
/etc/default/hl                                    : iostat
/etc/default/hl                                    : iptables
/etc/default/hl                                    : man
/etc/default/hl                                    : mpg123
/etc/default/hl                                    : namei
/etc/default/hl                                    : netstat
/etc/default/hl                                    : objdump
/etc/default/hl                                    : ps
/etc/default/hl                                    : smartctl
/etc/default/hl                                    : strace
/etc/default/hl                                    : stty
/etc/default/hl                                    : tcpdump
/etc/default/hl                                    : w
/etc/default/hl                                    : xxd
/etc/default/hl                                    : acl
/etc/default/hl                                    : C
/etc/default/hl                                    : colors
/etc/default/hl                                    : colors42
/etc/default/hl                                    : color_names
/etc/default/hl                                    : err
/etc/default/hl                                    : eth
/etc/default/hl                                    : eth_VIP
/etc/default/hl                                    : fail2ban
/etc/default/hl                                    : ha_log
/etc/default/hl                                    : heartbeat
/etc/default/hl                                    : hl
/etc/default/hl                                    : hl_conf
/etc/default/hl                                    : hl_usage
/etc/default/hl                                    : hosts
/etc/default/hl                                    : IP
/etc/default/hl                                    : ip
/etc/default/hl                                    : jigdo
/etc/default/hl                                    : ls_doc
/etc/default/hl                                    : MAC
/etc/default/hl                                    : on-off
/etc/default/hl                                    : passwd
/etc/default/hl                                    : passwd_chk
/etc/default/hl                                    : percent
/etc/default/hl                                    : ps_cpu
/etc/default/hl                                    : ps_cpu_time
/etc/default/hl                                    : rev_color_names
/etc/default/hl                                    : samba
/etc/default/hl                                    : sep3
/etc/default/hl                                    : services
/etc/default/hl                                    : sh
/etc/default/hl                                    : show_disks
/etc/default/hl                                    : validate_IP
/etc/default/hl                                    : virsh_list
/etc/default/hl                                    : hi_red
/etc/default/hl                                    : hi_green
/etc/default/hl                                    : hi_yellow
/etc/default/hl                                    : hi_blue
/etc/default/hl                                    : hi_magenta
/etc/default/hl                                    : hi_cyan
/etc/default/hl                                    : hi_white
/etc/default/hl                                    : dim_red
/etc/default/hl                                    : dim_green
/etc/default/hl                                    : dim_yellow
/etc/default/hl                                    : dim_blue
/etc/default/hl                                    : dim_magenta
/etc/default/hl                                    : dim_cyan
/etc/default/hl                                    : dim_white
/etc/default/hl                                    : red
/etc/default/hl                                    : green
/etc/default/hl                                    : yellow
/etc/default/hl                                    : blue
/etc/default/hl                                    : magenta
/etc/default/hl                                    : cyan
/etc/default/hl                                    : white
```

To use a colorized version of the "man" command, you should define a pager :
MANPAGER=/usr/local/bin/hl_man_pager
(and copy hl_man pager to /usr/local/bin).

Examples
--------

```
/sbin/ifconfig -a | hl -ei -m '^(eth|(vir)?br|vnet)[0-9.]*:[0-9]+\>'       \
			 -b '^(eth|(vir)?br|vnet)[0-9.]*\.[0-9]+\>'             \
			 -c '([0-9a-f]{2}:){5}[0-9a-f]{2}'                      \
			 -g '\<UP\>|\<RUNNING\>|([0-9]{1,3}\.){3}[0-9]{1,3}\>'  \
			 -y '^(eth|(vir)?br|vnet)[0-9.:]*\>'

/sbin/ifconfig -a | hl --ifconfig

/sbin/ifconfig -a | hl --IP --MAC --eth

cat firewall_rules | hl -e -c INPUT                    \
				   -y 'FORWARD|POSTROUTING'    \
				   -b '#.*'                    \
				   -W 'OUTPUT'                 \
				   -g '.*ACCEPT.*'             \
				   -r '.*(DROP|REJECT).*'      \
				   -m 'iptables.*-F.*'         \
				   -w '^iptables .*'

cat firewall_rules | hl --iptables

df -h | hl --df
```
![df](https://github.com/mbornet-hl/hl/blob/master/images/df.png)

```

fdisk -l | hl --fdisk
```
![fdisk](https://github.com/mbornet-hl/hl/blob/master/images/fdisk.png)

Alternate colors on the first 15 lines of `vmstat` :
```
vmstat 1 | head -n 15 | hl -A,2B3w
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
