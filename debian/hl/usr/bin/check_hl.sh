#!/bin/bash
#
#    Check whether hl is available or not (via the PATH)
#    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#    Author      : Martial Bornet
#    Last update : 2022-01-05
#
#    Here is a way of colorizing the output of some commands in a script,
#    without bothering whether the "hl" command is available of not.
#    If "hl" is available, the output will be colorized according to your
#    colorization options, and if not, the output will be printed as is.
#
#
# Usage of this script :
# ~~~~~~~~~~~~~~~~~~~~~~
# 1. Define the "_hl" function in your script, using your own colorization
#    options :
#
#    _hl()
#    {
#         if [ "$colorize" = 'true' -a "$hl_available" = 'true' ]; then
#              cat - | hl ... ... ...     <= Your colorization options here
#         else
#              cat
#         fi
#    }
#
# 2. In your main program, call the "check_hl" script to detect whether "hl"
#    is available or not :
#
#    . check_hl
#
# 3. Initialize a "colorize" variable based on a colorization criteria (option
#    or whatever) :
#
#    if [ ... ]; then
#         colorize='true'
#    else
#         colorize='false'
#    fi
#
# 4. Call the "_hl" function when you want to colorize the output :
#    ... | _hl
#

type hl > /dev/null 2>&1
if [ $? = 0 ]; then
     hl_available='true'
else
     hl_available='false'
fi

