#
# Copyright (C) 2010-2014 Freescale Semiconductor, Inc.
# All rights reserved.
#
# This software may be distributed under the terms of the
# GNU General Public License ("GPL") as published by the Free Software
# Foundation, either version 2 of that License or (at your option) any
# later version.
#
# THIS SOFTWARE IS PROVIDED BY Freescale Semiconductor ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Freescale Semiconductor BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([ls-dbg], [1.0], [freescale.com])


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

OLD_CFLAGS=$CFLAGS
AC_PROG_CC
# reset flags to original values
CFLAGS=$OLD_CFLAGS

AC_PROG_LIBTOOL

#PKG_PROG_PKG_CONFIG([0.23])

# Define these substitions here to keep all version information in one place.
# For information on how to properly maintain the library version information,
# refer to the libtool manual, section "Updating library version information":
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html

AC_SUBST([DBGDEV_SO_VERSION], [0:0:0])
AC_SUBST([DBGDEV_API_VERSION], [0.5.0])

##########################################################################
# ls-dbg module support
##########################################################################

# Define kernel src path
AC_MSG_CHECKING([option to specify kernel path])
AC_ARG_WITH([linux],
    [AS_HELP_STRING([--with-linux=DIR],
        [path to Linux kernel src (default=/home/root/linux)])],
    [KSRC="$withval"],
    [KSRC="/home/root/linux"])
AC_MSG_RESULT([$KSRC])
AC_SUBST([KSRC])

##########################################################################
# 64 bit target support
##########################################################################

#AC_MSG_CHECKING([option to build for 64 bit target])
#AC_ARG_ENABLE([64-bit],
#    [AS_HELP_STRING([--enable-64-bit],
#        [enable 64 bit code generation (default=no)])],
#    [comp_64bit="$enableval"],
#    [comp_64bit=no])
#AC_MSG_RESULT([$comp_64bit])
#
#if test x"$comp_64bit" = x"yes"; then
#    AM_CFLAGS="$AM_CFLAGS -m64"
#fi
#
#AC_SUBST([AM_CFLAGS])

##########################################################################
# debug compilation support
##########################################################################

AC_MSG_CHECKING([option to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (default=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    AM_CFLAGS="$AM_CFLAGS -std=gnu99 -Wall -pedantic -Werror -Wno-uninitialized -g -O0"
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
    AM_CFLAGS="$AM_CFLAGS -std=gnu99 -Wall -pedantic -Werror -Wno-uninitialized -O3"
fi

AC_SUBST([AM_CFLAGS])

##########################################################################

# Don't do this. Adds platform specific files to the distribution.
#AC_CONFIG_MACRO_DIR([m4])

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES([Makefile \
				ls-common/Makefile \
				ls-dbg/Makefile \
				ls-dbg/module/Makefile \
				test/Makefile
])

AC_OUTPUT

