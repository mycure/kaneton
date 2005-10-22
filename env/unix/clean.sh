#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## clean.sh
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Fri Oct 21 23:27:43 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory src/
#
# this script must always be run by the script src/env/clean.sh



# CONFIGURATION FILE PATH
#
# the configuration file
_CONF_="conf/"$USER"/"$USER".conf"



# CONFIGURATION FILE VARIABLES
#
# default globals
_DISPLAY_="unknown"
_ENVIRONMENT_="unknown"
_ARCHITECTURE_="unknwon"



# INSTALL SPECIFIC VARIABLES
#
# generic names for this script
_KANETON_MK_=".kaneton.mk"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # display
  _DISPLAY_=`cat $_CONF_ | grep -E "^_DISPLAY_ = .*$" | cut -b 13-`

  # environment
  _ENVIRONMENT_=`cat $_CONF_ | grep -E "^_ENVIRONMENT_ = .*$" | cut -b 17-`

  # architecture
  _ARCHITECTURE_=`cat $_CONF_ | grep -E "^_ARCHITECTURE_ = .*$" | cut -b 18-`
}



# clean
#
# clean the environment for unix operating system
clean()
{
  # cleaning dev tree
  if [ -e $_KANETON_MK_ ] ; then
    display "   cleaning development tree" "+"
    make clear 1>&2 > /dev/null
  fi

  # makefile dependencies
  display "   cleaning makefile dependencies" "+"
  rm -f $_KANETON_MK_

  # remove the links to machine-dependent directories
  display "   removing links to machine-dependent directories" "+"

  rm -f core/bootstrap/arch/machdep
  rm -f core/bootloader/arch/machdep
  rm -f core/include/arch/machdep
  rm -f core/kaneton/arch/machdep
  rm -f tools/scripts/ld/arch/machdep
}



# PRINT A MESSAGE
#
# prints a message using the user variable DISPLAY
print()
{
  color=$1
  message=$2
  options=$3

  if [ $_DISPLAY_ = "color" ] ; then

    case "$color" in
      "red")
        echo -e $options '\E[;31m'"\033[1m$message\033[0m"
	;;

      "green")
        echo -e $options '\E[;32m'"\033[1m$message\033[0m"
	;;

      "yellow")
        echo -e $options '\E[;33m'"\033[1m$message\033[0m"
	;;

      "blue")
        echo -e $options '\E[;34m'"\033[1m$message\033[0m"
	;;

      "white")
        echo -e $options '\E[;37m'"\033[1m$message\033[0m"
	;;

      *)
	;;
    esac

  else

    echo $options "$message"

  fi
}



# DISPLAY A MESSAGE
#
# displays a message with a header
display()
{
  msg=$1
  header=$2

  case "$header" in
    "+")
      print "blue" "[" "-n"
      print "green" "+" "-n"
      print "blue" "]" "-n"
      ;;

    "!")
      print "blue" "[" "-n"
      print "red" "!" "-n"
      print "blue" "]" "-n"
      ;;

    "?")
      print "blue" "[" "-n"
      print "yellow" "?" "-n"
      print "blue" "]" "-n"
      ;;

    *)
      ;;
  esac

  print "white" "$msg" ""
}



# ENTRY POINT
#
# entry point of this script

# call the read_kaneton_conf function
read_kaneton_conf

# install environment
clean
