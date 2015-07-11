#
# hl command :
# ~~~~~~~~~~~~
#
# This command uses regcomp() and regexec() to colorize (highlight)
# strings from stdin using options on the command line.
#
# Usage :
#
# hl: version 1.36
# Usage: src/hl [-h|-H|-eidDL1234][-E][-rgybmcwRGYBMCW|--config_name] regexp ...
#   -h : help
#   -H : help + configuration names
#   -V : version
#   -v : verbose
#   -u : do not bufferize output on stdout
#   -e : extended regular expressions
#   -i : ignore case
#   -E : print on stderr
#   -r : red
#   -g : green
#   -y : yellow
#   -b : blue
#   -m : magenta
#   -c : cyan
#   -w : white
#   -R : red     (reverse video)
#   -G : green   (reverse video)
#   -Y : yellow  (reverse video)
#   -B : blue    (reverse video)
#   -M : magenta (reverse video)
#   -C : cyan    (reverse video)
#   -W : white   (reverse video)
#   -d : debug
#   -D : display regular expressions
#   -L : lex debug
#   -1 : color brightness (half-bright)
#   -2 : color brightness (normal : default)
#   -3 : color brightness (bright)
#   -4 : color brightness (underscore)
#   Configurations :
#     --apt
#     --cal
#     --df
#     --diff
#     --err
#     --ethtool
#     --heartbeat
#     --hl
#     --ifconfig
#     --iptables
#     --ls_doc
#     --MAC
#     --percent
#     --ps_cpu
#     --services
#     --validate_IP
#     --virsh_list
#     --xxd
# 
#
# Examples :
# ~~~~~~~~~~
#
#
#  /sbin/ifconfig -a | hl -ei -m '^(eth|(vir)?br|vnet)[0-9.]*:[0-9]+\>'       \
#                      -b '^(eth|(vir)?br|vnet)[0-9.]*\.[0-9]+\>'             \
#                      -c '([0-9a-f]{2}:){5}[0-9a-f]{2}'                      \
#                      -g '\<UP\>|\<RUNNING\>|([0-9]{1,3}\.){3}[0-9]{1,3}\>'  \
#                      -y '^(eth|(vir)?br|vnet)[0-9.:]*\>'
#
#  cat firewall_rules | hl -e -c INPUT                    \
#                             -y 'FORWARD|POSTROUTING'    \
#                             -b '#.*'                    \
#                             -W 'OUTPUT'                 \
#                             -g '.*ACCEPT.*'             \
#                             -r '.*(DROP|REJECT).*'      \
#                             -m 'iptables.*-F.*'         \
#                             -w '^iptables .*'
