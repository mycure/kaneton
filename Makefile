##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/Makefile
##
## created       julien quintard   [sun nov 20 14:06:01 2005]
## updated       julien quintard   [wed jun  7 13:13:35 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

-include		env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

.PHONY:		all init clean kaneton clear purge proto		\
		dep build install check info 				\
		view view- export export- cheat cheat-			\
		play play- record record-				

#
# ---------- variables --------------------------------------------------------
#

KANETON_SHELL		?=		/bin/bash

_SHELL_			?=		$(KANETON_SHELL)
_MAKE_			?=		$(MAKE)

#
# ---------- default rule -----------------------------------------------------
#

ifeq ($(_SIGNATURE_),kaneton)
  all:			kaneton
else
  all kaneton								\
  clear purge								\
  proto dep								\
  build install								\
  check info								\
  view view-								\
  export export-							\
  cheat cheat-								\
  play play-								\
  record record-							\
  clean									\
  view-% export-% cheat-% play-% record-%:	init
	$(_MAKE_) -f Makefile $@
endif

#
# ---------- environment ------------------------------------------------------
#

init:
	cd env								; \
	$(_SHELL_) init.sh						; \
	cd ..

#
# ---------- conditional ------------------------------------------------------
#

ifeq ($(_SIGNATURE_),kaneton)

#
# ---------- environment ------------------------------------------------------
#

clean:
	$(call change-directory,$(_ENV_DIR_),)				; \
	$(_SHELL_) clean.sh						; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		libs $(_SRC_DEP_)

CLEARDIRS		:=		libs view export check		\
					cheat				\
					$(_CTCOMPARE_DIR_)		

#
# ---------- development ------------------------------------------------------
#

kaneton:
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call makefile,$(CLEARDIRS),clear)

purge:
	$(call purge,)

#
# ---------- prototypes -------------------------------------------------------
#

proto:
	$(call makefile,$(SUBDIRS),proto)

#
# ---------- dependencies -----------------------------------------------------
#

dep:
	$(call makefile,$(SUBDIRS),dep)

#
# ---------- boot -------------------------------------------------------------
#

build:
	$(call change-directory,$(_MBL_DIR_)/$(_MBL_),)			; \
	$(call shell-script,$(_MBL_).sh,build,)				; \
	$(call change-directory,$(_SRC_DIR_),)

install:		kaneton
	$(call change-directory,$(_MBL_DIR_)/$(_MBL_),)			; \
	$(call shell-script,$(_MBL_).sh,install,)			; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- check ------------------------------------------------------------
#

check:
	$(call makefile,$(_CHECK_DIR_),)

#
# ---------- view -------------------------------------------------------------
#

view- view:
	$(call change-directory,$(_VIEW_DIR_),)				; \
	$(call shell-script,$(_VIEW_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

view-%:
	$(call change-directory,$(_VIEW_DIR_),)				; \
	$(call shell-script,$(_VIEW_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- play -------------------------------------------------------------
#

play- play:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call shell-script,$(_PLAY_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

play-%:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call shell-script,$(_PLAY_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- record -----------------------------------------------------------
#

record- record:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call shell-script,$(_RECORD_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

record-%:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call shell-script,$(_RECORD_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- information ------------------------------------------------------
#

info:
	$(call print,white,,)

	$(call print,blue,"--- ",--no-newline)
	$(call print,white,http://www.kaneton.org,)

	$(call print,white,,)

#
# ---------- dist -------------------------------------------------------------
#

dist:			export-dist

#
# ---------- export -----------------------------------------------------------
#

export- export:
	$(call change-directory,$(_EXPORT_DIR_),)			; \
	$(call shell-script,$(_EXPORT_SCRIPT_),,)			; \
	$(call change-directory,$(_SRC_DIR_),)

export-%:
	$(call change-directory,$(_EXPORT_DIR_),)			; \
	$(call shell-script,$(_EXPORT_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- cheat ------------------------------------------------------------
#

cheat- cheat:
	$(call makefile,$(_CTCOMPARE_DIR_),)

	$(call change-directory,$(_CHEAT_DIR_),)			; \
	$(call shell-script,$(_CHEAT_SCRIPT_),,)			; \
	$(call change-directory,$(_SRC_DIR_),)

cheat-%:
	$(call makefile,$(_CTCOMPARE_DIR_),)

	$(call change-directory,$(_CHEAT_DIR_),)			; \
	$(call shell-script,$(_CHEAT_SCRIPT_),$*,)			; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- /conditional -----------------------------------------------------
#

endif
