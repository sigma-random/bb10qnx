# $NetBSD: Makefile,v 1.52 2012/12/06 11:40:58 jperkin Exp $
#

DISTNAME=	indent-2.2.9
PKGNAME=	g${DISTNAME}
PKGREVISION=	3
CATEGORIES=	devel
MASTER_SITES=	${MASTER_SITE_GNU:=indent/}

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://www.gnu.org/software/indent/indent.html
COMMENT=	GNU version of pretty-printer for C source code

LICENSE=	gnu-gpl-v2

PKG_INSTALLATION_TYPES=	overwrite pkgviews

GNU_CONFIGURE=		yes
USE_PKGLOCALEDIR=	yes
INFO_FILES=		yes
MAKE_JOBS_SAFE=		no

AUTO_MKDIRS=		yes

.include "../../mk/bsd.prefs.mk"

post-install:
	${LN} -sf ${PREFIX}/bin/gindent ${DESTDIR}${PREFIX}/${PKGGNUDIR}bin/indent
	${LN} -sf ${PREFIX}/${PKGMANDIR}/man1/gindent.1 ${DESTDIR}${PREFIX}/${PKGGNUDIR}${PKGMANDIR}/man1/indent.1
	${LN} -sf ${PREFIX}/bin/gtexinfo2man ${DESTDIR}${PREFIX}/${PKGGNUDIR}bin/texinfo2man

CONFIGURE_ARGS+=	--program-prefix=g

.include "../../devel/gettext-lib/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
