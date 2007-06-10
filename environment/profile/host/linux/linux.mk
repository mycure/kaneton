#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/environment/profile/host/linux/linux.mk
#
# created       julien quintard   [tue may  8 13:03:34 2007]
# updated       julien quintard   [sun jun 10 14:21:13 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this file implements the remaining functions of the kaneton make interface.
#
# indeed the major generic part of the interface is already provided by the
# host profile.
#

#
# ---------- python path ------------------------------------------------------
#

export PYTHONPATH=$(shell $(_ECHO_) $${PYTHONPATH}):$(_PYTHON_INCLUDE_DIR_)

#
# ---------- functions --------------------------------------------------------
#

#
# colorize functions
#
# 1:		text
# 2:		options
#

define env_colorize-black
  "[30;01m$(1)[39;49;00m"
endef

define env_colorize-red
  "[31;01m$(1)[39;49;00m"
endef

define env_colorize-green
  "[32;01m$(1)[39;49;00m"
endef

define env_colorize-yellow
  "[33;01m$(1)[39;49;00m"
endef

define env_colorize-blue
  "[34;01m$(1)[39;49;00m"
endef

define env_colorize-magenta
  "[35;01m$(1)[39;49;00m"
endef

define env_colorize-cyan
  "[36;01m$(1)[39;49;00m"
endef

define env_colorize-white
  "[37;01m$(1)[39;49;00m"
endef

define env_colorize-
  "$(1)"
endef

#
# print functions wrapper
#
# 1:		text
# 2:		color
# 3:		options
#

ifeq ($(_DISPLAY_),$(_DISPLAY_COLORED_))	# if the user wants to display
						# the text with color

define env_print
  print_options=""							; \
  if [ -n "$(3)" ] ; then						\
    if [ $$(( $(3) & $(ENV_OPTION_NO_NEWLINE) )) -ne 0 ] ; then		\
      print_options="$${print_options} -n"				; \
    fi									; \
  fi									; \
  $(_ECHO_) $${print_options} $(call env_colorize-$(2),$(1),)
endef

else						# if not ...

define env_print
  print_options=""							; \
  if [ -n "$(3)" ] ; then						\
    if [ $$(( $(3) & $(ENV_OPTION_NO_NEWLINE) )) -ne 0 ] ; then		\
      print_options="$${print_options} -n"				; \
    fi									; \
  fi									; \
  $(_ECHO_) $${print_options} $(1)
endef

endif

#
# change the current working directory
#
# $(1):		directory
# $(2):		options
#

define env_cd
  cd_options=""								; \
  $(_CD_) $${cd_options} $(1)
endef

#
# returns a file contents
#
# $(1):		the file
# $(2):		options
#

define env_pull
  $(_CAT_) $(2) $(1)
endef

#
# launch a new program/script/make etc.
#
# $(1):		file
# $(2):		arguments
# $(3):		options
#

define env_launch
  launch_options=""							; \
  cwd=$$($(_PWD_))							; \
  directory=$$($(_DIRNAME_) $(1))					; \
  file=$$($(_BASENAME_) $(1))						; \
  if [ $${directory} != "." ] ; then					\
    $(_CD_) $${directory}						; \
  fi									; \
  case "$${file}" in							\
    *.sh)								\
      $(_SHELL_) $${launch_options} $${file} $(_SHELL_FLAGS_) $(2)	; \
      ;;								\
    *.py)								\
      $(_PYTHON_) $${launch_options} $${file} $(_PYTHON_FLAGS_) $(2)	; \
      ;;								\
    *.pl)								\
      $(_PERL_) $${launch_options} $${file} $(_PERL_FLAGS_) $(2)	; \
      ;;								\
    Makefile)								\
      $(_MAKE_) $${launch_options} -f $${file} $(_MAKE_FLAGS_) ${2}	; \
      ;;								\
  esac									; \
  if [ $${directory} != "." ] ; then					\
    $(_CD_) $${cwd}							; \
  fi
endef

#
# from c file to preprocessed c file
#
# $(1):		preprocessed file
# $(2):		c file
# $(3):		options
#

define env_preprocess
  preprocess_options=""							; \
  $(call env_display,green,PREPROCESS,$(2),		,)		; \
  $(_CPP_) -P $(_CPP_FLAGS_) $${preprocess_options} $(2) -o $(1)
endef

#
# from c file to object file
#
# $(1):		object file
# $(2):		c file
# $(3):		options
#

define env_compile-c
  compile_c_options=""							; \
  $(call env_display,green,COMPILE-C,$(2),		,)		; \
  $(_CC_) $(_CC_FLAGS_) $${compile_c_options} -c $(2) -o $(1)
endef

#
# from lex file to c file
#
# $(1):		c file
# $(2):		lex file
# $(3):		options
#

define env_lex-l
  lex_l_options=""							; \
  $(call env_display,green,LEX-L,$(2),			,)		; \
  $(_LEX_) $${lex_l_options} $(2) > $(1)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		options
#

define env_assemble-S
  assemble_S_options=""							; \
  $(call env_display,green,ASSEMBLE-S,$(2),		,)		; \
  $(_CC_) $(_CC_FLAGS_) $${assemble_S_options}				\
          -c $(2) -o $(1)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		options
#

define env_assemble-asm
  assemble_asm_options=""						; \
  if [ -n "$(3)" ] ; then						\
    if [ $$(( $(3) & $(ENV_OUTPUT_OBJECT) )) -ne 0 ] ; then		\
      assemble_asm_options="$${assemble_asm_options} -f elf"		; \
    fi									; \
    if [ $$(( $(3) & $(ENV_OUTPUT_BINARY) )) -ne 0 ] ; then		\
      assemble_asm_options="$${assemble_asm_options} -f bin"		; \
    fi									; \
  fi									; \
  $(call env_display,green,ASSEMBLE-ASM,$(2),		,)		; \
  $(_NASM_) $${assemble_asm_options} $(2) -o $(1)
endef

#
# create a static library from object files
#
# $(1):		static library file name
# $(2):		object files
# $(3):		options
#

define env_static-library
  static_library_options=""						; \
  $(call env_display,magenta,STATIC-LIBRARY,$(1),	,)		; \
  $(_AR_) $${static_library_options} $(1) $(2)				; \
  $(_RANLIB_) $${static_library_options} $(1)
endef

#
# create a dynamic library from object files, static libraries
# and/or dynamic libraries
#
# $(1):		dynamic library file name
# $(2):		objects files and/or libraries
# $(3):		options
#

define env_dynamic-library
  dynamic_library_options=""						; \
  $(call env_display,magenta,DYNAMIC-LIBRARY,$(1),	,)		; \
  $(_LD_) --shared $(_LD_FLAGS_) $${dynamic_library_options}		\
          -o $(1) $(2)
endef

#
# create an executable file from object file and/or library files
#
# $(1):		executable file name
# $(2):		objects files and/or libraries
# $(3):		layout file
# $(4):		options
#

define env_executable
  executable_options=""							; \
  if [ -n $(3) ] ; then							\
    executable_options="$${executable_options} -T $(3)"			; \
  fi									; \
  if [ -n "$(4)" ] ; then						\
    if [ $$(( $(4) & $(ENV_OPTION_NO_STANDARD) )) -ne 0 ] ; then	\
      assemble_asm_options="$${assemble_asm_options} -nostdinc -nostdlib" ; \
    fi									; \
  fi									; \
  $(call env_display,magenta,EXECUTABLE,$(1),		,)		; \
  $(_CC_) $(_CC_FLAGS_) $(_LD_FLAGS_) $${executable_options}		\
          -o $(1) $(2)
endef

#
# create an archive file from multiple object files
#
# note that the archive file is also an object file
#
# $(1):		archive file name
# $(2):		objects files and/or libraries
# $(3):		options
#

define env_archive
  archive_options=""							; \
  $(call env_display,magenta,ARCHIVE,$(1),		,)		; \
  $(_LD_) -r $(_LD_FLAGS_) $${archive_options} -o $(1) $(2)
endef

#
# remove the files
#
# $(1):		files
# $(2):		options
#

define env_remove
  remove_options=""							; \
  for f in $(1) ; do							\
    if [ -e $${f} ] ; then						\
      $(call env_display,red,REMOVE,$${f},		,)		; \
    fi									; \
    $(_RM_) $${remove_options} $${f}					; \
  done
endef

#
# purge i.e clean the directory from unwanted files
#

define env_purge
  $(call env_display,red,PURGE,,,)					; \
  $(_PURGE_)
endef

#
# generate prototypes from a source file
#
# $(1):		file list
# $(2):		options
#

define env_prototypes
  prototypes_options=""							; \
  for o in $(2); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  for f in $(1) ; do							\
    if [ -e $${f} ] ; then						\
      $(call env_display,yellow,PROTOTYPES,$${f},		,)	; \
      $(call env_launch,$(_MKP_TOOL_),					\
        $${prototypes_options} $${f},)					; \
    fi									; \
  done
endef

#
# genereate header dependencies
#
# $(1):		the files for which the dependencies are generated
# $(2):		the output file
# $(3):		options
#

define env_headers
  headers_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(_TOUCH_) $(2)							; \
  for f in $(1) ; do							\
    if [ -e $${f} ] ; then						\
      $(call env_display,yellow,HEADERS,$$i,		,)		; \
      $(_CC_) $(_CC_FLAGS_) -M -MG $${headers_options}			\
        $${f} >> $(2)							; \
    fi									; \
  done
endef

#
# generate a version file
#
# $(1):		the version file to generate
#

define env_version
  $(call env_display,yellow,VERSION,$(1),		,)		; \
  $(_ECHO_) -n "" > $(1)						; \
  $(_ECHO_) "#include <klibc.h>" >> $(1)				; \
  $(_ECHO_) "#include <kaneton.h>" >> $(1)				; \
  $(_ECHO_) "" >> $(1)							; \
  $(_ECHO_) -n "const char version[] = \"$(_TITLE_)-$(_VERSION_)" >> $(1) ; \
  $(_ECHO_) " "$(shell $(_DATE_))" $(USER)@$(HOSTNAME)\";" >> $(1)
endef

#
# create a link between two files
#
# $(1):		link created
# $(2):		destination
# $(3):		options
#

define env_link
  link_options=""							; \
  $(call env_display,cyan,LINK,$(1),			,)		; \
  $(_LN_) $${link_options} $(2) $(1)
endef

#
# compile a tex document
#
# $(1):		the file name without extension
# $(2):		options
#

define env_compile-tex
  compile_tex_options=""						; \
  $(call env_display,green,COMPILE-TEX,$(1),		,)		; \
  $(_PDFLATEX_) $${compile_tex_options} $(1).tex			; \
  $(_BIBTEX_) $(1).tex							; \
  $(_PDFLATEX_) $${compile_tex_options} $(1).tex			; \
  $(_PDFLATEX_) $${compile_tex_options} $(1).tex
endef

#
# build a document
#
# $(1):		the file name without extension
# $(2):		options
#

define env_document
  if [ -n "$(2)" ] ; then						\
    if [ $$(( $(2) & $(ENV_OPTION_PRIVATE) )) -ne 0 ] ; then		\
      $(_ECHO_) '\def\mode{private}' > $(_DEPENDENCY_TEX_)		; \
    fi									; \
  fi									; \
  $(call env_compile-tex,$(1),$(2))
endef

#
# launch a document viewer
#
# $(1):		the file name without extension
# $(2):		options
#

define env_view
  $(call env_display,yellow,VIEW,$(1),			,)		; \
  $(_XPDF_) $(1).pdf
endef
