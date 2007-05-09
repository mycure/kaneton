#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/env/critical.py
#
# created       julien quintard   [fri dec 15 13:43:03 2006]
# updated       julien quintard   [wed may  9 09:32:55 2007]
#

#
# ---------- imports ----------------------------------------------------------
#

import os
import sys
import re

#
# ---------- globals ----------------------------------------------------------
#

g_directories = None
g_contents = None
g_assignments = []

g_variables = {}

#
# ---------- functions --------------------------------------------------------
#

#
# error()
#
# this function displays an error and quit.
#
def			error(msg):
  sys.stderr.write("[!] " + msg)
  sys.exit(42)



#
# warning()
#
# this function simply displays an error.
#
def			warning(msg):
  sys.stderr.write("[!] " + msg)



#
# load()
#
# this function takes an arbitray number of directories where pattern
# files could be located and load them in a single python string.
#
def			load(directories, pattern):
  content = ""

  directory = None
  includes = None
  include = None
  handle = None
  files = None
  line = None
  file = None
  cwd = None

  for directory in directories:
    if not os.path.isdir(directory):
      continue

    files = os.listdir(directory);

    for file in files:
      if not os.path.isfile(directory + "/" + file):
        continue

      if not re.match(pattern, os.path.basename(directory + "/" + file)):
        continue

      try:
        handle = open(directory + "/" + file, "r")
      except IOError:
        error("unable to open the file " + directory + "/" + file + "\n")

      for line in handle.readlines():
        content += line

      includes = re.findall("("						\
                              "^"					\
                              "include"					\
                              "[ \t]*"					\
                              "(.*)"					\
                              "\n"					\
                            ")", content, re.MULTILINE);

      for include in includes:
        content = content.replace(include[0],
                                  load([os.path.dirname(directory +
                                                        "/" +
                                                        include[1])],
                                       "^" +
                                         os.path.basename(directory +
                                                          "/" +
                                                          include[1]) +
                                         "$"))

      handle.close()

  return content



#
# comments()
#
# this function removes the comments from the file
#
def			comments():
  global g_contents
  comments = None
  c = None

  comments = re.findall("^#.*$", g_contents, re.MULTILINE);

  for c in comments:
    g_contents = g_contents.replace(c, "", 1)



#
# locate()
#
# this function tries to locate the variable of the given array and
# return the corresponding tuple.
#
def		locate(array, variable):
  var = None

  for var in array:
    if variable == var[0]:
      return var

  return None



#
# extract()
#
# this function extracts the variables assignments from the environment
# configuration files previously loaded.
#
def			extract():
  global g_assignments
  assignments = None
  assignment = None
  already = None
  new = None

  assignments = re.findall("^"					\
                             "[ \t]*"					\
                             "([a-zA-Z0-9_]+)"				\
                             "[ \t]*"					\
                             "(\+?=)"					\
                             "[ \t]*"					\
                             "((?:(?:\\\\\n)|[^\n])+)"			\
                             "\n", g_contents, re.MULTILINE);

  for assignment in assignments:
    # look for a previous registered assignment.
    already = locate(g_assignments, assignment[0])

    if already:
      # if it is an assignment, just override the previous declaration.
      if assignment[1] == "=":
        new = (assignment[0], assignment[2])
      # if it is a concatenation, override the previous one with a
      # concatenation of the two values.
      elif assignment[1] == "+=":
        new = (assignment[0], already[1] + " " + assignment[2])
      else:
        error("unknown assignment token '" + assignment[1] + "' for the "
              "variable '" + assignment[0] + "'.\n")

      # remove and insert the new tuple.
      g_assignments.remove(already)
      g_assignments.append(new)
    else:
      if assignment[1] == "=":
        # simple insert the tuple.
        new = (assignment[0], assignment[2])

        g_assignments.append(new)
      elif assignment[1] == "+=":
        error("appending to the undefined variable '" + assignment[0] +
              "' is not allowed\n")
      else:
        error("unknown assignment token '" + assignment[1] + "' for the "
              "variable '" + assignment[0] + "'.\n")



#
# expand()
#
# this function tries to expand the given variable.
#
def		expand(name, stack):
  variables = None
  position = None
  tuple = None
  value = None
  var = None

  # try to get the variable's value from the already expanded variables.
  try:
    value = g_variables[name]
    return
  except:
    # check if the variable was declared somewhere.
    tuple = locate(g_assignments, name)
    if not tuple:
      warning("the kaneton environment variable " + name +
              " is not defined\n")
      value = ""
    else:
      value = tuple[1]

  # check recursion assignments.
  try:
    position = stack.index(name)
  except:
    pass

  if position != None:
    error("the kaneton environment variable " + name +
          " recursively references itself.\n")

  # locate, expand and replace the kaneton variables.
  variables = re.findall("\$\{([^}]+)\}", value)
  if variables:
    for var in variables:
      expand(var, stack + [name])
    for var in variables:
      value = value.replace("${" + var + "}", g_variables[var])

  # locate, expand and replace the shell variables.
  variables = re.findall("\$\(([^}]+)\)", value)
  if variables:
    for var in variables:
      if os.getenv(var) == None:
        warning("shell user variable " + var + " is not defined\n")
        value = value.replace("$(" + var + ")", "")
      else:
        value = value.replace("$(" + var + ")", os.getenv(var))

  # finally register the new variable with its expanded value.
  g_variables[name] = value



#
# resolve()
#
# this function resolves the variable assignments.
def			resolve():
  for assignment in g_assignments:
    expand(assignment[0], [])



#
# generate()
#
# this function generates the kaneton development environment files.
#
def			generate(mfile, mcontent, pfile, pcontent):
  mhandle = None
  phandle = None
  line = None
  var = None

  # open the make environment file.
  try:
    mhandle = open(mfile, "w")
  except IOError:
    error("unable to open the file " + mfile + "\n")

  # open the python environment file.
  try:
    phandle = open(pfile, "w")
  except IOError:
    error("unable to open the file " + pfile + "\n")

  # inject the kaneton environment configuration for the make and python
  # environment.
  for var in g_variables:
    mhandle.write(var + " := " + g_variables[var] + "\n")
    phandle.write(var + " = " + "\"" + g_variables[var] + "\"" + "\n")

  # append the make environment files to the make environment file.
  mhandle.write(mcontent)

  # append the python environment files to the python environment file.
  phandle.write(pcontent)

  # close the files.
  phandle.close()
  mhandle.close()



#
# main()
#
# this function builds a kaneton development environment.
#
def			main():
  global g_directories
  global g_contents
  architecture = None
  source_dir = None
  plateform = None
  contents = None
  machine = None
  python = None
  host = None
  user = None

  # get the shell environment variables.
  user = os.getenv("KANETON_USER")
  host = os.getenv("KANETON_HOST")
  python = os.getenv("KANETON_PYTHON")
  plateform = os.getenv("KANETON_PLATEFORM")
  architecture = os.getenv("KANETON_ARCHITECTURE")

  # check the presence of the shell environment variable.
  if user == None:
    error("the shell environment variable KANETON_USER is not set")

  if host == None:
    error("the shell environment variable KANETON_HOST is not set")

  if python == None:
    error("the shell environment variable KANETON_PYTHON is not set")

  if plateform == None:
    error("the shell environment variable KANETON_PLATEFORM is not set")

  if architecture == None:
    error("the shell environment variable KANETON_ARCHITECTURE is not set")

  # set the configuration directories based on the user variables.
  g_directories = ("profile/",
                   "profile/environment/",
                   "profile/environment/behaviour/",
                   "profile/environment/behaviour/" + host + "/" +
                                                      architecture + "/",
                   "profile/kaneton/",
                   "profile/kaneton/core/",
                   "profile/kaneton/plateform/",
                   "profile/kaneton/plateform/" + plateform + "/",
                   "profile/kaneton/architecture/",
                   "profile/kaneton/architecture/" + architecture + "/",
                   "profile/user/",
                   "profile/user/" + user + "/")

  # first, set a virtual kaneton variable containing the location of
  # the kaneton source directory.
  # this directory is expected to be relatively located at: ../
  # since this script should have been launched in the environment/
  # directory
  cwd = os.getcwd()
  os.chdir("..")
  source_dir = os.getcwd()
  os.chdir(cwd)

  # load the content of the configuration files *.conf.
  g_contents = "_SOURCE_DIR_		=		" + source_dir + "\n"
  g_contents += load(g_directories, "^.*\.conf$")

  # removes the comments
  comments()

  # extract the assignments from the files.
  extract()

  # resolve the variables in assignments.
  resolve()

  # generate the development environment files.
  generate("env.mk", load(g_directories, "^.*\.mk$"),
           "env.py", load(g_directories, "^.*\.py$"))

#
# ---------- entry point ------------------------------------------------------
#

main()
