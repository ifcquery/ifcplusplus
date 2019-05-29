# NOTE: This Makefile.am relies on $(htmldir) being AC_SUBST'ed into place...

include ../src/Inventor/Win/common/Makefile.common

KEYHTMLPAGE = index.html

if BUILD_HTMLPAGES

# **************************************************************************
# build trickery

$(KEYHTMLPAGE): $(SoGuiBuiltFiles)
	@( cd $(top_builddir); "@sim_ac_doxygen_exe@" sogui.doxygen )

filelist.txt: $(KEYHTMLPAGE)
	@if cmp -s $(top_srcdir)/src/Inventor/Win/common/docs/doxygen/Coin_logo.png Coin_logo.png; then :; else \
	  echo  " cp $(top_srcdir)/src/Inventor/Win/common/docs/doxygen/Coin_logo.png ."; \
	  cp $(top_srcdir)/src/Inventor/Win/common/docs/doxygen/Coin_logo.png .; \
	fi
	@ls -1 | egrep '(\.html|\.png|\.gif|\.css)$$' >filelist.txt

all-local: filelist.txt

clean-local:
	@if test -f filelist.txt; then \
	  cat filelist.txt | xargs rm -f filelist.txt; \
	fi

# **************************************************************************
# install trickery
# - uninstall after "make clean" will regenerate docs - ought to be fixed

install-data-local: install-html

uninstall-local: uninstall-html

if MAC_FRAMEWORK
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Documentation"
	@echo " ln -sf Versions/Current/Documentation $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Documentation"; \
	ln -sf Versions/Current/Documentation "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Documentation"
	@destdir="$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Documentation"; \
	list="`cat filelist.txt`"; \
	for file in $$list; do \
	  echo " $(INSTALL_DATA) $$file $(DESTDIR)$$destdir/$$file"; \
	  $(INSTALL_DATA) $$file "$(DESTDIR)$$destdir/$$file"; \
	done

uninstall-html: filelist.txt
	@$(NORMAL_UNINSTALL)
	@destdir="$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Documentation"; \
	list="`cat filelist.txt`"; \
	for file in $$list; do \
	  echo " rm -f $(DESTDIR)$(destdir)/$$file"; \
	  rm -f "$(DESTDIR)$(destdir)/$$file"; \
	done
	@echo " rmdir $(DESTDIR)$(destdir)"; \
	rmdir $(DESTDIR)$(destdir) 2>/dev/null; \
	:
else
install-html: filelist.txt
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(htmldir)
	@list="`cat filelist.txt`"; \
	for i in $$list; do \
	  echo " $(INSTALL_DATA) $$i $(DESTDIR)$(htmldir)/$$i"; \
	  $(INSTALL_DATA) $$i $(DESTDIR)$(htmldir)/$$i; \
	done

uninstall-html: filelist.txt
	@$(NORMAL_UNINSTALL)
	@list="`cat filelist.txt`"; \
	for i in $$list; do \
	  echo " rm -f $(DESTDIR)$(htmldir)/$$i"; \
	  rm -f $(DESTDIR)$(htmldir)/$$i; \
	done
	@echo " rmdir $(DESTDIR)$(htmldir)"; \
	rmdir $(DESTDIR)$(htmldir) 2>/dev/null; \
	:
endif

endif

