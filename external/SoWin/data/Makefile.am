############################################################################
## So@GUI@/data/Makefile.am
## $Id$

SUBDIRS = bin materials

BUILT_SOURCES = \
	bin/iv2h.sh \
	materials/materials.h \
	materials/artdeco/artdeco.h \
	materials/autumn/autumn.h \
	materials/glass/glass.h \
	materials/metal/metal.h \
	materials/neon/neon.h \
	materials/rococo/rococo.h \
	materials/santafe/santafe.h \
	materials/sheen/sheen.h \
	materials/silky/silky.h \
	materials/spring/spring.h \
	materials/summer/summer.h \
	materials/tropical/tropical.h \
	materials/winter/winter.h

EXTRA_DIST = \
	materials/materials.h.in \
	materials/artdeco/artdeco.h.in \
	materials/autumn/autumn.h.in \
	materials/glass/glass.h.in \
	materials/metal/metal.h.in \
	materials/neon/neon.h.in \
	materials/rococo/rococo.h.in \
	materials/santafe/santafe.h.in \
	materials/sheen/sheen.h.in \
	materials/silky/silky.h.in \
	materials/spring/spring.h.in \
	materials/summer/summer.h.in \
	materials/tropical/tropical.h.in \
	materials/winter/winter.h.in

all-local: built-sources

############################################################################

built-sources: $(BUILT_SOURCES)

bin/iv2h.sh: $(srcdir)/bin/iv2h.sh.in
	@./config.status --file=$@; \
	chmod +x bin/iv2h.sh

materials/materials.h: $(srcdir)/materials/materials.h.in
	@./config.status --file=$@

materials/artdeco/artdeco.h: $(srcdir)/materials/artdeco/artdeco.h.in
	@./config.status --file=$@

materials/autumn/autumn.h: $(srcdir)/materials/autumn/autumn.h.in
	@./config.status --file=$@

materials/glass/glass.h: $(srcdir)/materials/glass/glass.h.in
	@./config.status --file=$@

materials/metal/metal.h: $(srcdir)/materials/metal/metal.h.in
	@./config.status --file=$@

materials/neon/neon.h: $(srcdir)/materials/neon/neon.h.in
	@./config.status --file=$@

materials/rococo/rococo.h: $(srcdir)/materials/rococo/rococo.h.in
	@./config.status --file=$@

materials/santafe/santafe.h: $(srcdir)/materials/santafe/santafe.h.in
	@./config.status --file=$@

materials/sheen/sheen.h: $(srcdir)/materials/sheen/sheen.h.in
	@./config.status --file=$@

materials/silky/silky.h: $(srcdir)/materials/silky/silky.h.in
	@./config.status --file=$@

materials/spring/spring.h: $(srcdir)/materials/spring/spring.h.in
	@./config.status --file=$@

materials/summer/summer.h: $(srcdir)/materials/summer/summer.h.in
	@./config.status --file=$@

materials/tropical/tropical.h: $(srcdir)/materials/tropical/tropical.h.in
	@./config.status --file=$@

materials/winter/winter.h: $(srcdir)/materials/winter/winter.h.in
	@./config.status --file=$@

############################################################################
