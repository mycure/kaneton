#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## grub.sh
## 
## path          /home/mycure/data/research/projects/kaneton/source/utils
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Fri May 27 10:54:32 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in its directory src/utils/



# CONFIGURATION FILE PATH
#
# configuration file
CONF="../conf/"$USER"/"$USER".conf"
MODULES=`sed "s/#.*//g" ../conf/"$USER"/modules.conf | grep -v "^$"`



# CONFIGURATION FILE VARIABLES
#
# default globals
TITLE="unknown"
ADDRESS="unknown"
TFTP_ADDRESS="unknown"
TFTP_DIRECTOR="unknown"
BOOTMODE="unknown"
UDEVICE="unknown"
MDEVICE="unknown"
IMAGE="unknown"

KANETON_IMAGE="data/kaneton.img"
MENU="/tmp/menu.lst"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # title
  TITLE=`cat $CONF | grep -E "^TITLE = .*$" | cut -b 9-`

  # address
  ADDRESS=`cat $CONF | grep -E "^ADDRESS = .*$" | cut -b 11-`

  # tftp address
  TFTP_ADDRESS=`cat $CONF | grep -E "^TFTP_ADDRESS = .*$" | cut -b 16-`

  # tftp directory
  TFTP_DIRECTORY=`cat $CONF | grep -E "^TFTP_DIRECTORY = .*$" | cut -b 18-`

  # bootmode
  BOOTMODE=`cat $CONF | grep -E "^BOOTMODE = .*$" | cut -b 12-`

  # unix device
  UDEVICE=`cat $CONF | grep -E "^UDEVICE = .*$" | cut -b 11-`

  # mtools device
  MDEVICE=`cat $CONF | grep -E "^MDEVICE = .*$" | cut -b 11-`

  # image
  IMAGE=`cat $CONF | grep -E "^IMAGE = .*$" | cut -b 9-`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  print " usage: grub.sh init|boot" "!"
}



# WARNING
#
# this function alerts the user, displaying information and asking to continue
warning()
{
  # display information and ask the user to continue or cancel
  print " your current configuration" "+"
  print "   title:                    $TITLE" "+"
  print "   address:                  $ADDRESS" "+"
  print "   tftp address:             $TFTP_ADDRESS" "+"
  print "   tftp directory:           $TFTP_DIRECTORY" "+"
  print "   bootmode:                 $BOOTMODE" "+"
  print "   udevice:                  $UDEVICE" "+"
  print "   mdevice:                  $MDEVICE" "+"
  print "   image:                    $IMAGE" "+"
  print ""
  print " to cancel press CTRL^C, otherwise press enter" "?"

  NEEDLESS=""
  read NEEDLESS
}



# MENU
#
# this function creates the grub menu file
menu()
{
  # remove old version
  rm -f $MENU 2>&1 > /dev/null

  # create new version
  echo "title $TITLE" >> $MENU
  echo "" >> $MENU

  # insert each module in menu

  case "$BOOTMODE" in
    "floppy"|"floppy-image")
      echo "root (fd0)" >> $MENU
      echo "" >> $MENU
      ;;

    "tftp")
      echo "ifconfig --address=$ADDRESS --server=$TFTP_ADDRESS" >> $MENU
      echo "" >> $MENU
      echo "root (nd)" >> $MENU
      echo "" >> $MENU
      ;;

    *)
      print " unknown mode \"$BOOTMODE\"" "!"
      print ""
      print " please check your BOOTMODE variable in $CONF" "!"
      print ""
      usage
      print " supported boot modes for grub are:" "!"
      print "      floppy" "!"
      print "      tftp" "!"
      exit
      ;;
  esac

  i=0
  for module in $MODULES ; do
    if [ $i = 0 ] ; then
      kernel=$module
    else
      modules="$modules $module"
    fi
    i=1
  done

  echo $kernel | sed "s/^.*\/\(.*\)$/kernel \/modules\/\1/g" >> $MENU

  echo "" >> $MENU

  for module in $modules
  do
      echo $module | sed "s/^.*\/\(.*\)$/module \/modules\/\1/g" >> $MENU
  done
}



# INIT
#
# initialize the boot device
init()
{
  case "$BOOTMODE" in
    "floppy"|"tftp")
      cat $KANETON_IMAGE > $UDEVICE
      ;;

    "floppy-image")
      cp $KANETON_IMAGE $IMAGE
      ;;

    *)
      print " unknown mode \"$BOOTMODE\"" "!"
      print ""
      print " please check your BOOTMODE variable in $CONF" "!"
      print ""
      usage
      print " current supported boot modes for grub are:" "!"
      print "      floppy" "!"
      print "      tftp" "!"
      print "      floppy-image" "!"
      exit
      ;;
  esac
}



# BOOT
#
# installs the distribution on the boot device
boot()
{
  case "$BOOTMODE" in
    "floppy")
      if [ ! -f $MENU ] ; then
        print " $MENU" "!"
      else
        mcopy -n -o $MENU $MDEVICE/boot/grub/
        print " $MENU" "+"
      fi

      for module in $MODULES ; do
        if [ ! -f ../$module ] ; then
          print " $module" "!"
        else
          mcopy -n -o ../$module $MDEVICE/modules/
          print " $module" "+"
        fi
      done
      print ""
      ;;

    "tftp")
      if [ ! -f $MENU ] ; then
        print " $MENU" "!"
      else
        mcopy -n -o $MENU $MDEVICE/boot/grub/
        print " $MENU" "+"
      fi

      for module in $MODULES ; do
        if [ ! -f ../$module ] ; then
          print " $module" "!"
        else
          cp ../$module $TFTP_DIRECTORY
          print " $module" "+"
        fi
      done
      print ""
      ;;

    "floppy-image")
      if [ ! -f $MENU ] ; then
        print " $MENU" "!"
      else
        mcopy -n -o $MENU -i $IMAGE ::/boot/grub/
        print " $MENU" "+"
      fi

      for module in $MODULES ; do
        if [ ! -f ../$module ] ; then
          print " $module" "!"
        else
          mcopy -n -o ../$module -i $IMAGE ::/modules/
          print " $module" "+"
        fi
      done
      print ""
      ;;

    *)
      print " unknown mode \"$BOOTMODE\"" "!"
      print ""
      print " please check your BOOTMODE variable in $CONF" "!"
      print ""
      usage
      print " current supported boot modes for grub are:" "!"
      print "      floppy" "!"
      print "      tftp" "!"
      print "      floppy-image" "!"
      exit
      ;;
  esac
}



# PRINT A MESSAGE
#
# print a message with a header
print()
{
  msg=$1
  header=$2

  case "$header" in
    "+")
      echo -n -e '\E[;34m'"\033[1m[\033[0m"
      echo -n -e '\E[;32m'"\033[1m+\033[0m"
      echo -n -e '\E[;34m'"\033[1m]\033[0m"
      ;;

    "!")
      echo -n -e '\E[;34m'"\033[1m[\033[0m"
      echo -n -e '\E[;31m'"\033[1m!\033[0m"
      echo -n -e '\E[;34m'"\033[1m]\033[0m"
      ;;

    "?")
      echo -n -e '\E[;34m'"\033[1m[\033[0m"
      echo -n -e '\E[;33m'"\033[1m\033[5m?\033[0m\033[0m"
      echo -n -e '\E[;34m'"\033[1m]\033[0m"
      ;;

    *)
      ;;
  esac

  echo -e "\033[1m$msg\033[0m"
  tput sgr0
}



# ENTRY POINT
#
# entry point of this script

# check the number of arguments
if [ $# -lt 1 ] ; then
    usage
    exit -1
fi

action="$1"
shift

case "$action" in
  "init")
    # initializing boot system
    print " initializing boot system" "+"
    print ""

    # call the read_kaneton_conf function
    read_kaneton_conf

    # call the warning function
    warning

    # call the init function
    init

    # boot system initialized
    print " boot system initialized successfully" "+"
    ;;

  "boot")
    # initializing boot system
    print " initializing boot system" "+"
    print ""

    # call the read_kaneton_conf function
    read_kaneton_conf

    # call the warning function
    warning

    # call the menu function
    menu

    # call the boot function
    boot

    # boot system initialized
    print " boot system initialized successfully" "+"
    ;;

  *)
    print " unknown action \"$action\"" "!"
    print ""
    usage
    print " supported action are:" "!"
    print "      init" "!"
    print "      boot" "!"
    exit
    ;;
esac
