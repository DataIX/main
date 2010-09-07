# ex:ts=8
# Ports collection makefile for:	GDB 6
# Date created:				30 March 2001
# Whom:					David O'Brien <obrien@NUXI.com>
#
# $FreeBSD: ports/devel/gdb6/Makefile,v 1.57 2010/08/11 19:44:36 linimon Exp $
#

PORTNAME=	gdb
PORTVERSION=	7.2
PORTREVISION=	5
CATEGORIES=	devel
MASTER_SITES=   ${MASTER_SITE_GNU:S,$,:gdb,}
MASTER_SITE_SUBDIR=gdb/:gdb
DISTFILES=	${PORTNAME}-${PORTVERSION}${EXTRACT_SUFX}:gdb

MAINTAINER=	ports@freebsd.org, jhell@dataix.net
COMMENT=	GDB and Reverse Debugging

USE_BZIP2=	yes
USE_GMAKE=	yes
USE_GETTEXT=	yes
USE_ICONV=	yes
GNU_CONFIGURE=	yes
CONFIGURE_ENV+=	CONFIGURED_M4=m4 CONFIGURED_BISON=byacc
CONFIGURE_ARGS=	--program-suffix=${PORTVERSION:S/.//g} \
		--enable-target=all \
		--enable-tui --with-libiconv-prefix=${LOCALBASE}
CFLAGS:=	${CFLAGS:C/ +$//}	# blanks at EOL creep in sometimes
CFLAGS+=	-DRL_NO_COMPAT
EXCLUDE=	dejagnu expect sim texinfo intl
EXTRACT_AFTER_ARGS=| ${TAR} -xf - ${EXCLUDE:S/^/--exclude /}
VER=	${PORTVERSION:S/.//}
PLIST_SUB=	VER=${VER}
MAN1=	gdb${VER}.1

.include <bsd.port.pre.mk>

.if ${ARCH} == "ia64" || ${ARCH} == "powerpc"
BROKEN=		Does not configure on ia64 or powerpc
.endif

.if ${ARCH} == "amd64"
CONFIGURE_TARGET=	x86_64-portbld-freebsd${OSREL}
.endif

post-patch:
	@${REINPLACE_CMD} -e 's/$$/ [GDB v${PORTVERSION} for FreeBSD]/' \
		${WRKSRC}/gdb/version.in
	@${REINPLACE_CMD} -e 's/^READLINE/#READLINE/' \
		-e 's/^readline/#readline/' \
		-e 's,^LIBICONV =.*,LIBICONV=-L${LOCALBASE}/lib -liconv,' \
		${WRKSRC}/gdb/Makefile.in
	${ECHO} 'READLINE = -lreadline' >> ${WRKSRC}/gdb/Makefile.in

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/gdb/gdb ${PREFIX}/bin/gdb${VER}
	${LN} ${PREFIX}/bin/gdb${VER} ${PREFIX}/bin/gdbtui${VER}
	${INSTALL_MAN} ${WRKSRC}/gdb/gdb.1 ${MAN1PREFIX}/man/man1/gdb${VER}.1

post-install:
	@${ECHO} "======================================================================="
	@${ECHO}
	@${CAT} ${PKGMESSAGE}
	@${ECHO}
	@${ECHO} "======================================================================="

.include <bsd.port.post.mk>
