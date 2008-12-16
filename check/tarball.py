#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/check/tarball.py
#
#

#
# ---------- information ------------------------------------------------------
#
# this tool creates a tarball with the test files
#
# the argument 'filename' is the yaml file that describe the test suite.
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import re
import os

import env

#
# ---------- globals ----------------------------------------------------------
#

yaml_filename = None
g_directory = None

#
# ---------- functions --------------------------------------------------------
#

#
# usage()
#
# this function displays the usage.
#
def			usage():
  stage = None
  type = None

  env.display(env.HEADER_NONE, "usage: tarball.py [filename]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

#
# warning()
#
# this function asks the user if everything is right.
#
def			warning():
  env.display(env.HEADER_OK, "your current configuration:", env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter", env.OPTION_NONE)

  env.input(env.OPTION_NONE)

#
# clean()
#
# this function removes unwanted stuff i.e the env._EXPORT_FILTER_
# stuff as well as the subversion directories.
#
def			clean():
  entries = None
  entry = None

  # a message.
  env.display(env.HEADER_OK,
              "cleaning the distribution from unwanted/private stuff...",
              env.OPTION_NONE)

  # remove the subversion directories
  entries = env.search(g_directory, "^\.svn$",
                       env.OPTION_DIRECTORY | env.OPTION_RECURSIVE)

  for entry in entries:
    env.remove(entry, env.OPTION_NONE)

  # remove the env._EXPORT_FILTER_ stuff
  entities = re.findall("([^\t ]+)", env._EXPORT_FILTER_)

  for entity in entities:
    env.remove(entity, env.OPTION_NONE)


def                     dist():
    global yaml_filename
    global g_directory

    g_directory = env.temporary(env.OPTION_DIRECTORY)

    fd = open(yaml_filename, "r")
    content = ""
    while fd:
        line = fd.readline()
        if line:
            content += line
        else:
            break
    fd.close()

    for e in re.findall('path: ([\w/]+)', content):
        env.copy(e, g_directory + "/checks/" + e, env.OPTION_RECURSIVE)

    clean()

    tarball_name = env._EXPORT_ + "-" +                 \
                   env.stamp(env.OPTION_NONE) + "-check.tar.bz2"
    env.cd(g_directory, env.OPTION_NONE)
    
    env.display(env.HEADER_OK,
                "packing the check files...",
                env.OPTION_NONE)

    env.pack("checks",
             env._EXPORT_DIR_ + "/" + tarball_name,
             env.OPTION_NONE)
             
    env.display(env.HEADER_OK,
                "removing tomporary directory...",
                env.OPTION_NONE)

    env.remove(g_directory, env.OPTION_NONE)

#
# main()
#
# this function performs the main stuff.
#
def			main():
  global yaml_filename

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  yaml_filename = sys.argv[1]

  # a message.
  env.display(env.HEADER_OK, "tarball for %s" % sys.argv[1], env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # warn the user about what is about to happen.
  warning()

  dist()

  # a final message.
  env.display(env.HEADER_OK, "tarball done", env.OPTION_NONE);

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
