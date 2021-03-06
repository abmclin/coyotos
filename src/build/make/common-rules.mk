#
# Copyright (C) 2005, The EROS Group, LLC.
#
# This file is part of the Coyotos Operating System.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#


unexport DIRS
unexport ETAGDIRS
unexport GENERATED
unexport CLEANLIST

ifndef MAKEVARS_LOADED
include $(COYOTOS_SRC)/build/make/makevars.mk
endif

ifndef CLEANDIRS
CLEANDIRS=$(DIRS)
endif

ifeq "$(BUILDDIR)" ""
$(error BUILDDIR is not set!)
endif
ifneq "$(BUILDDIR)" "."
CLEAN_BUILDDIR=$(BUILDDIR)
endif

.PHONY: tags
.PHONY: world package

######################################################################
#
# TOP LEVEL BUILD RULES FOR REMAKING CURRENT PACKAGE OR ENTIRE WORLD
#
######################################################################
world:
	$(MAKE) -C $(COYOTOS_SRC) $(MAKERULES) packages

ifneq "$(PACKAGE)" ""
package:
	$(MAKE) -C $(COYOTOS_ROOT)/src/$(PACKAGE) $(MAKERULES) interfaces
	$(MAKE) -C $(COYOTOS_ROOT)/src/$(PACKAGE) $(MAKERULES) libs
	$(MAKE) -C $(COYOTOS_ROOT)/src/$(PACKAGE) $(MAKERULES) install

endif

pristine: distclean
	$(MAKE) -C $(COYOTOS_ROOT)/src world

distclean:
	$(MAKE) -C $(COYOTOS_ROOT)/src -k targdir-clean clean

######################################################################
#
# GLOBAL AND RECURSIVE TARGETS FOR USE WITHIN A PACKAGE.
#
######################################################################

export RECURSE_TARGET

.PHONY: recurse
recurse:
	@for i in `echo $(DIRS)`; do \
		if [ -d "$$i" ]; then\
			$(MAKE) -C $$i $(MAKERULES) $(RECURSE_TARGET) recurse; \
			if [ $$? -ne 0 ]; then\
				echo "*** RECURSIVE BUILD STOPS ***";\
				exit 1;\
			fi; \
		fi; \
	done

.PHONY: subdirs
subdirs: RECURSE_TARGET=subdirs
subdirs: recurse

.PHONY: install
install: RECURSE_TARGET=install
install: recurse

.PHONY: interfaces
interfaces: RECURSE_TARGET=interfaces
interfaces: recurse

.PHONY: doxygen
doxygen: $(GENERATED)
doxygen: RECURSE_TARGET=doxygen
doxygen: recurse

### install: recursive-install
### recursive-install:
### ifneq "$(DIRS)" ""
### 	@for i in $(DIRS); do \
### 		if [ -d "$$i" ]; then\
### 			$(MAKE) -C $$i $(MAKERULES) install; \
### 			if [ $$? -ne 0 ]; then\
### 				echo "*** RECURSIVE BUILD STOPS ***";\
### 				exit 1;\
### 			fi; \
### 		fi; \
### 	done
### endif

.PHONY: clean

clean: nodepend

# Clean does not use the generic recurse, because it needs to recurse over
# different directories
clean: generic-clean
clean:
	@for i in `echo $(CLEANDIRS)`; do \
		if [ -d "$$i" ]; then\
			$(MAKE) -C $$i $(MAKERULES) clean; \
			if [ $$? -ne 0 ]; then\
				echo "*** RECURSIVE CLEAN STOPS ***";\
				exit 1;\
			fi; \
		fi; \
	done

.PHONY: generic-clean
generic-clean:
	-rm -f *.o core *~ new.Makefile  ".#"*
	-rm -f .*.m sysgen.map $(TARGETS) TAGS
	-rm -f *.dvi *.blg *.aux *.log *.toc $(CLEANLIST)
ifneq "$(CLEAN_BUILDDIR)" ""
	-rm -rf $(CLEAN_BUILDDIR)
endif

package-clean:
	$(MAKE) -C $(COYOTOS_ROOT)/src/$(PACKAGE) $(MAKERULES) clean

## CLEANING: The following piece of idiocy works around the fact that
## the autodependency files may refer to stuff that has been cleaned,
## and that this can therefore perversely cause the clean target to
## fail.

.PHONY: nodepend 
nodepend:
	-find . -name '.*.m' -exec rm {} \;
	-find . -name 'DEPEND' -exec rm {} \;

# This is a debugging target..
.PHONY: walk
walk: RECURSE_TARGET=walk
walk: recurse

here:
	@echo $(PWD)

COMMONRULES_LOADED=1
