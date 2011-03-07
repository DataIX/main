
LEN?=		6
CC=		cc
CFLAGS:=	-O2 -pipe -fno-strict-aliasing
CFLAGS+=	-DLEN=${LEN}

SRCS=		pwlgen.c
PROG=		pwlgen${LEN}

${PROG}:
	@echo ${CC} ${CFLAGS} -o $@ ${SRCS}
	@${CC} ${CFLAGS} -o $@ ${SRCS}
	@strip -s $@
