divert(-1)                                                   -*- Autoconf -*-
# This file is part of Autoconf.
# Base m4 layer.
# Requires GNU m4.
# Copyright (C) 1999, 2000 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#
# As a special exception, the Free Software Foundation gives unlimited
# permission to copy, distribute and modify the configure scripts that
# are the output of Autoconf.  You need not follow the terms of the GNU
# General Public License when using or distributing such scripts, even
# though portions of the text of Autoconf appear in them.  The GNU
# General Public License (GPL) does govern all other use of the material
# that constitutes the Autoconf program.
#
# Certain portions of the Autoconf source text are designed to be copied
# (in certain cases, depending on the input) into the output of
# Autoconf.  We call these the "data" portions.  The rest of the Autoconf
# source text consists of comments plus executable code that decides which
# of the data portions to output in any given case.  We call these
# comments and executable code the "non-data" portions.  Autoconf never
# copies any of the non-data portions into its output.
#
# This special exception to the GPL applies to versions of Autoconf
# released by the Free Software Foundation.  When you make and
# distribute a modified version of Autoconf, you may extend this special
# exception to the GPL to apply to your modified version as well, *unless*
# your modified version has the potential to copy into its output some
# of the text that was the non-data portion of the version that you started
# with.  (In other words, unless your change moves or copies text from
# the non-data portions to the data portions.)  If your modification has
# such potential, you must delete any notice of this special exception
# to the GPL from your modified version.
#
# Written by Akim Demaille.
#

# Set the quotes, whatever the current quoting system.
changequote()
changequote([, ])

# Some old m4's don't support m4exit.  But they provide
# equivalent functionality by core dumping because of the
# long macros we define.
ifdef([__gnu__], ,
[errprint(Autoconf requires GNU m4. Install it before installing Autoconf or
set the M4 environment variable to its path name.)
m4exit(2)])


## --------------------------------- ##
## Defining macros and name spaces.  ##
## --------------------------------- ##

# m4_rename(SRC, DST)
# -------------------
#
# Rename the macro SRC as DST.
define([m4_rename],
[define([$2], defn([$1]))undefine([$1])])

# Some m4 internals have names colliding with tokens we might use.
# Rename them a` la `m4 --prefix-builtins'.
m4_rename([eval],    [m4_eval])
m4_rename([format],  [m4_format])
m4_rename([shift],   [m4_shift])
m4_rename([symbols], [m4_symbols])


## --------------------------------------------- ##
## Move some m4 builtins to a safer name space.  ##
## --------------------------------------------- ##

# m4_errprint(MSG)
# ----------------
# Same as `errprint', but reports the file and line.
define([m4_errprint], [errprint(__file__:__line__: [$1
])])


# m4_warn(MSG)
# ------------
# Warn the user.
define([m4_warn], [m4_errprint([warning: $1])])


# m4_fatal(MSG, [EXIT-STATUS])
# ----------------------------
# Fatal the user.                                                      :)
define([m4_fatal],
[m4_errprint([error: $1])dnl
m4exit(ifelse([$2],, 1, [$2]))])


# m4_assert( EXPRESSION [, EXIT-STATUS = 1 ])
# ------------------------------------------
# This macro ensures that EXPRESSION evaluates to true, and exits if
# EXPRESSION evaluates to false.
define([m4_assert],
[ifelse(m4_eval([$1]), 0,
        [m4_fatal([assert failed: $1], [$2])],
        [])])


# We also want to neutralize include (and sinclude for symmetry),
# but we want to extend them slightly: warn when a file is included
# several times.  This is in general a dangerous operation because
# quite nobody quotes the first argument of define.
#
# For instance in the following case:
#   define(foo, [bar])
# then a second reading will turn into
#   define(bar, [bar])
# which is certainly not what was meant.

# m4_include_unique(FILE)
# -----------------------
# Declare that the FILE was loading; and warn if it has already
# been included.
define([m4_include_unique],
[ifdef([m4_include($1)],
       [m4_warn([file `$1' included several times])])dnl
define([m4_include($1)])])


# m4_include(FILE)
# ----------------
# As the builtin include, but warns against multiple inclusions.
define([m4_include],
[m4_include_unique([$1])dnl
builtin([include], [$1])])


# m4_sinclude(FILE)
# -----------------
# As the builtin sinclude, but warns against multiple inclusions.
define([m4_sinclude],
[m4_include_unique([$1])dnl
builtin([sinclude], [$1])])

# Neutralize include and sinclude.
undefine([include])
undefine([sinclude])


## --------------------------------------- ##
## Some additional m4 structural control.  ##
## --------------------------------------- ##

# Both `ifval' and `ifset' tests against the empty string.  The
# difference is that `ifset' is specialized on macros.
#
# In case of arguments of macros, eg $[1], it makes little difference.
# In the case of a macro `FOO', you don't want to check `ifval(FOO,
# TRUE)', because if `FOO' expands with commas, there is a shifting of
# the arguments.  So you want to run `ifval([FOO])', but then you just
# compare the *string* `FOO' against `', which, of course fails.
#
# So you want a variation of `ifset' that expects a macro name as $[1].
# If this macro is both defined and defined to a non empty value, then
# it runs TRUE etc.


# ifval(COND, [IF-TRUE], [IF-FALSE])
# ----------------------------------
# If COND is not the empty string, expand IF-TRUE, otherwise IF-FALSE.
# Comparable to ifdef.
define([ifval], [ifelse([$1],[],[$3],[$2])])


# ifset(MACRO, [IF-TRUE], [IF-FALSE])
# -----------------------------------
# If MACRO has no definition, or of its definition is the empty string,
# expand IF-FALSE, otherwise IF-TRUE.
define([ifset],
[ifdef([$1],
       [ifelse(defn([$1]), [], [$3], [$2])],
       [$3])])


# ifndef(NAME, [IF-NOT-DEFINED], [IF-DEFINED])
# --------------------------------------------
define([ifndef],
[ifdef([$1], [$3], [$2])])


# m4_default(EXP1, EXP2)
# ----------------------
# Returns EXP1 if non empty, otherwise EXP2.
define([m4_default], [ifval([$1], [$1], [$2])])


# m4_shiftn( N, ... )
# -------------------
# Returns ... shifted N times.  Useful for recursive "varargs" constructs.
define([m4_shiftn],
[m4_assert(($1 >= 0) && ($# > $1))dnl
_m4_shiftn($@)])

define([_m4_shiftn],
[ifelse([$1], 0,
        [m4_shift($@)],
        [_m4_shiftn(m4_eval([$1]-1), m4_shift(m4_shift($@)))])])


# m4_case(SWITCH, VAL1, IF-VAL1, VAL2, IF-VAL2, ..., DEFAULT)
# -----------------------------------------------------------
# m4 equivalent of
# switch (SWITCH)
# {
#   case VAL1:
#     IF-VAL1;
#     break;
#   case VAL2:
#     IF-VAL2;
#     break;
#   ...
#   default:
#     DEFAULT;
#     break;
# }.
# All the values are optional, and the macro is robust to active
# symbols properly quoted.
define([m4_case],
[ifelse([$#], 0, [],
	[$#], 1, [],
	[$#], 2, [$2],
        [$1], [$2], [$3],
        [m4_case([$1], m4_shiftn(3, $@))])])


# m4_match(SWITCH, RE1, VAL1, RE2, VAL2, ..., DEFAULT)
# ----------------------------------------------------
# m4 equivalent of
#
# if (SWITCH =~ RE1)
#   VAL1;
# elif (SWITCH =~ RE2)
#   VAL2;
# elif ...
#   ...
# else
#   DEFAULT
#
# All the values are optional, and the macro is robust to active symbols
# properly quoted.
define([m4_match],
[ifelse([$#], 0, [],
	[$#], 1, [],
	[$#], 2, [$2],
        regexp([$1], [$2]), -1, [m4_match([$1], m4_shiftn(3, $@))],
        [$3])])

# m4_do(STRING, ...)
# ------------------
# This macro invokes all its arguments (in sequence, of course).  It is
# useful for making your macros more structured and readable by dropping
# unecessary dnl's and have the macros indented properly.

define([m4_do],
  [ifelse($#, 0, [],
          $#, 1, [$1],
          [$1[]m4_do(m4_shift($@))])])

## --------------------- ##
## Implementing m4 loops ##
## --------------------- ##


# m4_for(VARIABLE, FIRST, LAST, [STEP = +/-1], EXPRESSION)
# --------------------------------------
# Expand EXPRESSION defining VARIABLE to FROM, FROM + 1, ..., TO.
# Both limits are included.

define([m4_for],
[m4_case(m4_sign(m4_eval($3 - $2)),
         1, [m4_assert(m4_sign(m4_default($4, 1)) == 1)],
        -1, [m4_assert(m4_sign(m4_default($4, -1)) == -1)])dnl
pushdef([$1], [$2])dnl
ifelse(m4_eval([$3 > $2]), 1,
       [_m4_for([$1], [$3], m4_default([$4], 1), [$5])],
       [_m4_for([$1], [$3], m4_default([$4], -1), [$5])])dnl
popdef([$1])])

define([_m4_for],
[$4[]dnl
ifelse($1, [$2], [],
       [define([$1], m4_eval($1+[$3]))_m4_for([$1], [$2], [$3], [$4])])])


# Implementing `foreach' loops in m4 is much more tricky than it may
# seem.  Actually, the example of a `foreach' loop in the m4
# documentation is wrong: it does not quote the arguments properly,
# which leads to undesired expansions.
#
# The example in the documentation is:
#
# | # foreach(VAR, (LIST), STMT)
# | define([foreach],
# |        [pushdef([$1])_foreach([$1], [$2], [$3])popdef([$1])])
# | define([_arg1], [$1])
# | define([_foreach],
# | 	   [ifelse([$2], [()], ,
# | 		   [define([$1], _arg1$2)$3[]_foreach([$1],
# |                                                   (shift$2),
# |                                                   [$3])])])
#
# But then if you run
#
# | define(a, 1)
# | define(b, 2)
# | define(c, 3)
# | foreach([f], [([a], [(b], [c)])], [echo f
# | ])
#
# it gives
#
#  => echo 1
#  => echo (2,3)
#
# which is not what is expected.
#
# Of course the problem is that many quotes are missing.  So you add
# plenty of quotes at random places, until you reach the expected
# result.  Alternatively, if you are a quoting wizard, you directly
# reach the following implementation (but if you really did, then
# apply to the maintenance of libm4!).
#
# | # foreach(VAR, (LIST), STMT)
# | define([foreach], [pushdef([$1])_foreach($@)popdef([$1])])
# | define([_arg1], [[$1]])
# | define([_foreach],
# |  [ifelse($2, [()], ,
# | 	     [define([$1], [_arg1$2])$3[]_foreach([$1],
# |                                               [(shift$2)],
# |                                               [$3])])])
#
# which this time answers
#
#  => echo a
#  => echo (b
#  => echo c)
#
# Bingo!
#
# Well, not quite.
#
# With a better look, you realize that the parens are more a pain than
# a help: since anyway you need to quote properly the list, you end up
# with always using an outermost pair of parens and an outermost pair
# of quotes.  Rejecting the parens both eases the implementation, and
# simplifies the use:
#
# | # foreach(VAR, (LIST), STMT)
# | define([foreach], [pushdef([$1])_foreach($@)popdef([$1])])
# | define([_arg1], [$1])
# | define([_foreach],
# |  [ifelse($2, [], ,
# | 	     [define([$1], [_arg1($2)])$3[]_foreach([$1],
# |                                                 [shift($2)],
# |                                                 [$3])])])
#
#
# Now, just replace the `$2' with `m4_quote($2)' in the outer `ifelse'
# to improve robustness, and you come up with a quite satisfactory
# implementation.


# m4_foreach(VARIABLE, LIST, EXPRESSION)
# --------------------------------------
#
# Expand EXPRESSION assigning each value of the LIST to VARIABLE.
# LIST should have the form `item_1, item_2, ..., item_n', i.e. the
# whole list must *quoted*.  Quote members too if you don't want them
# to be expanded.
#
# This macro is robust to active symbols:
#      | define(active, [ACT, IVE])
#      | m4_foreach(Var, [active, active], [-Var-])
#     => -ACT--IVE--ACT--IVE-
#
#      | m4_foreach(Var, [[active], [active]], [-Var-])
#     => -ACT, IVE--ACT, IVE-
#
#      | m4_foreach(Var, [[[active]], [[active]]], [-Var-])
#     => -active--active-
define([m4_foreach],
[pushdef([$1])_m4_foreach($@)popdef([$1])])

# Low level macros used to define m4_foreach.
define([m4_car], [$1])
define([_m4_foreach],
[ifelse(m4_quote($2), [], [],
        [define([$1], [m4_car($2)])$3[]_m4_foreach([$1],
                                                   [m4_shift($2)],
                                                   [$3])])])


## ----------------- ##
## Text processing.  ##
## ----------------- ##


# m4_quote(STRING)
# ----------------
# Return STRING quoted.
#
# It is important to realize the difference between `quote(exp)' and
# `[exp]': in the first case you obtain the quoted *result* of the
# expansion of EXP, while in the latter you just obtain the string
# `exp'.
define([m4_quote], [[$*]])


# m4_noquote(STRING)
# ------------------
# Return the result of ignoring all quotes in STRING and invoking the
# macros it contains.  Amongst other things useful for enabling macro
# invocations inside strings with [] blocks (for instance regexps and
# help-strings).
define([m4_noquote],
[changequote(-=<{,}>=-)$1-=<{}>=-changequote([,])])


# m4_split(STRING, [REGEXP])
# --------------------------
#
# Split STRING into an m4 list of quoted elements.  The elements are
# quoted with [ and ].  Beginning spaces and end spaces *are kept*.
# Use m4_strip to remove them.
#
# REGEXP specifies where to split.  Default is [\t ]+.
#
# Pay attention to the changequotes.  Inner changequotes exist for
# obvious reasons (we want to insert square brackets).  Outer
# changequotes are needed because otherwise the m4 parser, when it
# sees the closing bracket we add to the result, believes it is the
# end of the body of the macro we define.
#
# Also, notice that $1 is quoted twice, since we want the result to
# be quoted.  Then you should understand that the argument of
# patsubst is ``STRING'' (i.e., with additional `` and '').
#
# This macro is safe on active symbols, i.e.:
#   define(active, ACTIVE)
#   m4_split([active active ])end
#   => [active], [active], []end

changequote(<<, >>)
define(<<m4_split>>,
<<changequote(``, '')dnl
[dnl Can't use m4_default here instead of ifelse, because m4_default uses
dnl [ and ] as quotes.
patsubst(````$1'''',
	  ifelse(``$2'',, ``[ 	]+'', ``$2''),
	  ``], ['')]dnl
changequote([, ])>>)
changequote([, ])



# m4_join(STRING)
# ---------------
# If STRING contains end of lines, replace them with spaces.  If there
# are backslashed end of lines, remove them.  This macro is safe with
# active symbols.
#    define(active, ACTIVE)
#    m4_join([active
#    act\
#    ive])end
#    => active activeend
define([m4_join],
[translit(patsubst([[[$1]]], [\\
]), [
], [ ])])


# m4_strip(STRING)
# ----------------
# Expands into STRING with tabs and spaces singled out into a single
# space, and removing leading and trailing spaces.
#
# This macro is robust to active symbols.
#    define(active, ACTIVE)
#    m4_strip([  active  		active ])end
#    => active activeend
#
# This macro is fun!  Because we want to preserve active symbols, STRING
# must be quoted for each evaluation, which explains there are 4 levels
# of brackets around $1 (don't forget that the result must be quoted
# too, hence one more quoting than applications).
#
# Then notice the patsubst of the middle: it is in charge of removing
# the leading space.  Why not just `patsubst(..., [^ ])'?  Because this
# macro will receive the output of the preceding patsubst, i.e. more or
# less [[STRING]].  So if there is a leading space in STRING, then it is
# the *third* character, since there are two leading `['; Equally for
# the outer patsubst.
define([m4_strip],
[patsubst(patsubst(patsubst([[[[$1]]]],
                            [[ 	]+], [ ]),
                   [^\(..\) ], [\1]),
          [ \(.\)$], [\1])])



# m4_append(MACRO-NAME, STRING)
# -----------------------------
# Redefine MACRO-NAME to hold its former content plus STRING at the
# end.  It is valid to use this macro with MACRO-NAME undefined.
#
# This macro is robust to active symbols.  It can be used to grow
# strings.
#
#    | define(active, ACTIVE)
#    | m4_append([sentence], [This is an])
#    | m4_append([sentence], [ active ])
#    | m4_append([sentence], [symbol.])
#    | sentence
#    | undefine([active])dnl
#    | sentence
#    => This is an ACTIVE symbol.
#    => This is an active symbol.
#
# It can be used to define hooks.
#
#    | define(active, ACTIVE)
#    | m4_append([hooks], [define([act1], [act2])])
#    | m4_append([hooks], [define([act2], [active])])
#    | undefine([active])
#    | act1
#    | hooks
#    | act1
#    => act1
#    =>
#    => active
define([m4_append],
[define([$1],
ifdef([$1], [defn([$1])])[$2])])


# m4_list_append(MACRO-NAME, STRING)
# ----------------------------------
# Same as `m4_append', but each element is separated by `, '.
define([m4_list_append],
[define([$1],
ifdef([$1], [defn([$1]), ])[$2])])




## ----------------------------------- ##
## Helping macros to display strings.  ##
## ----------------------------------- ##

# m4_foreach_quoted(VARIABLE, LIST, EXPRESSION)
# ---------------------------------------------
# FIXME: This macro should not exists.  Currently it's used only in
# m4_wrap, which needs to be rewritten.  But it's godam hard.
define([m4_foreach_quoted],
[pushdef([$1], [])_m4_foreach_quoted($@)popdef([$1])])

# Low level macros used to define m4_foreach.
define([m4_car_quoted], [[$1]])
define([_m4_foreach_quoted],
[ifelse($2, [()], ,
        [define([$1], [m4_car_quoted$2])$3[]_m4_foreach_quoted([$1],
                                                               [(m4_shift$2)],
                                                               [$3])])])


# m4_wrap(STRING, [PREFIX], [FIRST-PREFIX], [WIDTH])
# --------------------------------------------------
# Expands into STRING wrapped to hold in WIDTH columns (default = 79).
# If prefix is set, each line is prefixed with it.  If FIRST-PREFIX is
# specified, then the first line is prefixed with it.  As a special
# case, if the length of the first prefix is greater than that of
# PREFIX, then FIRST-PREFIX will be left alone on the first line.
#
# Typical outputs are:
#
# m4_wrap([Short string */], [   ], [/* ], 20)
#  => /* Short string */
#
# m4_wrap([Much longer string */], [   ], [/* ], 20)
#  => /* Much longer
#  =>    string */
#
# m4_wrap([Short doc.], [          ], [  --short ], 30)
#  =>   --short Short doc.
#
# m4_wrap([Short doc.], [          ], [  --too-wide ], 30)
#  =>   --too-wide
#  =>           Short doc.
#
# m4_wrap([Super long documentation.], [          ], [  --too-wide ], 30)
#  =>   --too-wide
#  => 	  Super long
#  => 	  documentation.
#
# FIXME: there is no checking of a longer PREFIX than WIDTH, but do
# we really want to bother with people trying each single corner
# of a software?
#
# This macro does not leave a trailing space behind the last word,
# what complicates it a bit.  The algorithm is stupid simple: all the
# words are preceded by m4_Separator which is defined to empty for the
# first word, and then ` ' (single space) for all the others.
define([m4_wrap],
[pushdef([m4_Prefix], m4_default([$2], []))dnl
pushdef([m4_Prefix1], m4_default([$3], [m4_Prefix]))dnl
pushdef([m4_Width], m4_default([$4], 79))dnl
pushdef([m4_Cursor], len(m4_Prefix1))dnl
pushdef([m4_Separator], [])dnl
m4_Prefix1[]dnl
ifelse(m4_eval(m4_Cursor > len(m4_Prefix)),
       1, [define([m4_Cursor], len(m4_Prefix))
m4_Prefix])[]dnl
m4_foreach_quoted([m4_Word], (m4_split(m4_strip(m4_join([$1])))),
[define([m4_Cursor], m4_eval(m4_Cursor + len(m4_Word) + 1))dnl
dnl New line if too long, else insert a space unless it is the first
dnl of the words.
ifelse(m4_eval(m4_Cursor > m4_Width),
       1, [define([m4_Cursor], m4_eval(len(m4_Prefix) + len(m4_Word) + 1))]
m4_Prefix,
       [m4_Separator])[]dnl
m4_Word[]dnl
define([m4_Separator], [ ])])dnl
popdef([m4_Separator])dnl
popdef([m4_Cursor])dnl
popdef([m4_Width])dnl
popdef([m4_Prefix1])dnl
popdef([m4_Prefix])dnl
])



## ------------------- ##
## Number processing.  ##
## ------------------- ##

# m4_sign(A)
# ----------
#
# The sign of the integer A.
define([m4_sign],
[m4_match([$1],
          [^-], -1,
          [^0+], 0,
                 1)])

# m4_cmp(A, B)
# ------------
#
# Compare two integers.
# A < B -> -1
# A = B ->  0
# A > B ->  1
define([m4_cmp],
[m4_sign(m4_eval([$1 - $2]))])


# m4_list_cmp(A, B)
# -----------------
#
# Compare the two lists of integers A and B.  For instance:
#   m4_list_cmp((1, 0),     (1))    ->  0
#   m4_list_cmp((1, 0),     (1, 0)) ->  0
#   m4_list_cmp((1, 2),     (1, 0)) ->  1
#   m4_list_cmp((1, 2, 3),  (1, 2)) ->  1
#   m4_list_cmp((1, 2, -3), (1, 2)) -> -1
#   m4_list_cmp((1, 0),     (1, 2)) -> -1
#   m4_list_cmp((1),        (1, 2)) -> -1
define([m4_list_cmp],
[ifelse([$1$2], [()()], 0,
        [$1], [()], [m4_list_cmp((0), [$2])],
        [$2], [()], [m4_list_cmp([$1], (0))],
        [m4_case(m4_cmp(m4_car$1, m4_car$2),
                 -1, -1,
                  1, 1,
                  0, [m4_list_cmp((m4_shift$1), (m4_shift$2))])])])
