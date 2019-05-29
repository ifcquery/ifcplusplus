# NOTES:
#   This Makefile.am relies on $(htmlhelpdir) being AC_SUBST'ed into place...

include ../src/Inventor/Win/common/Makefile.common

if BUILD_HTMLHELP

# **************************************************************************
# build trickery

SoWin-@SOWIN_MAJOR_VERSION@.chm: $(SoGuiBuiltFiles)
	@( cd $(top_builddir); "@sim_ac_doxygen_exe@" sogui.doxygen )

all-local: SoWin-@SOWIN_MAJOR_VERSION@.chm

# **************************************************************************
# install trickery
# - uninstall after "make clean" will regenerate docs - ought to be fixed

install-data-local: install-htmlhelp

uninstall-local: uninstall-htmlhelp

if MAC_FRAMEWORK
  # Not implemented
else
install-htmlhelp: SoWin-@SOWIN_MAJOR_VERSION@.chm
	@$(NORMAL_INSTALL)
	@if test -d $(DESTDIR)/$(htmlhelpdir); then :; else \
	  echo " $(mkinstalldirs) $(DESTDIR)$(htmlhelpdir)"; \
	  $(mkinstalldirs) $(DESTDIR)$(htmlhelpdir); \
	fi
	for file in SoWin-@SOWIN_MAJOR_VERSION@.chm; do \
	  echo " $(INSTALL_DATA) $$file $(DESTDIR)$(htmlhelpdir)/$$file"; \
	  $(INSTALL_DATA) $$file "$(DESTDIR)$(htmlhelpdir)/$$file"; \
	done

uninstall-htmlhelp: SoWin-@SOWIN_MAJOR_VERSION@.chm
	@$(NORMAL_UNINSTALL)
	for file in SoWin-@SOWIN_MAJOR_VERSION@.chm; do \
	  echo " rm -f $(DESTDIR)$(htmlhelpdir)/$$file"; \
	  rm -f "$(DESTDIR)$(htmlhelpdir)/$$file"; \
	done
	@echo " rmdir $(DESTDIR)$(htmlhelpdir)"; \
	rmdir $(DESTDIR)$(htmlhelpdir) 2>/dev/null; \
	:
endif

endif

# IRIX make barfs if clean-local is moved inside the conditional clause.
# Automake bug?  20011025 larsa
clean-local:
	@if test -f SoWin-@SOWIN_MAJOR_VERSION@.chm; then \
	  echo " rm SoWin-@SOWIN_MAJOR_VERSION@.chm"; \
	  rm SOWIN-@SOWIN_MAJOR_VERSION@.chm; \
	fi

