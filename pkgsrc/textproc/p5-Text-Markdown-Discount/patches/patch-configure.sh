$NetBSD: patch-configure.sh,v 1.2 2013/07/10 01:29:21 schmonz Exp $

Fix messed up configure.sh.

--- discount-2.1.5a/configure.sh.orig	2012-09-13 22:47:28.000000000 +0000
+++ discount-2.1.5a/configure.sh
@@ -87,11 +87,11 @@ AC_C_INLINE
 AC_SCALAR_TYPES sub hdr
 AC_CHECK_BASENAME
 
-AC_CHECK_HEADERS sys/types.h pwd.h && AC_CHECK_FUNCS getpwuid
+AC_CHECK_HEADERS sys/types.h pwd.h && AC_CHECK_FUNCS 'getpwuid(0)'
 
-if AC_CHECK_FUNCS srandom; then
+if AC_CHECK_FUNCS 'srandom(0)'; then
     AC_DEFINE 'INITRNG(x)' 'srandom((unsigned int)x)'
-elif AC_CHECK_FUNCS srand; then
+elif AC_CHECK_FUNCS 'srand(0)'; then
     AC_DEFINE 'INITRNG(x)' 'srand((unsigned int)x)'
 else
     AC_DEFINE 'INITRNG(x)' '(void)1'
@@ -113,7 +113,7 @@ else
     AC_DEFINE 'COINTOSS()' '1'
 fi
 
-if AC_CHECK_FUNCS strcasecmp; then
+if AC_CHECK_FUNCS 'strcasecmp("X", "x")'; then
     :
 elif AC_CHECK_FUNCS stricmp; then
     AC_DEFINE strcasecmp stricmp
@@ -121,7 +121,7 @@ else
     AC_FAIL "$TARGET requires either strcasecmp() or stricmp()"
 fi
 
-if AC_CHECK_FUNCS strncasecmp; then
+if AC_CHECK_FUNCS 'strncasecmp("X", "x", 1)'; then
     :
 elif AC_CHECK_FUNCS strnicmp; then
     AC_DEFINE strncasecmp strnicmp
@@ -129,7 +129,7 @@ else
     AC_FAIL "$TARGET requires either strncasecmp() or strnicmp()"
 fi
 
-if AC_CHECK_FUNCS fchdir || AC_CHECK_FUNCS getcwd ; then
+if AC_CHECK_FUNCS 'fchdir(0)' || AC_CHECK_FUNCS 'getcwd((char *)0, 0)'; then
     AC_SUB 'THEME' ''
 else
     AC_SUB 'THEME' '#'
