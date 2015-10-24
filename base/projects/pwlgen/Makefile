
LEN?=		8
CC=		clang
CFLAGS:=	-O3 -pipe -fno-strict-aliasing
CFLAGS+=	-DLEN=${LEN}

SRCS=		pwlgen.c
PROG=		pwlgen${LEN}

${PROG}:
	@echo ${CC} ${CFLAGS} -o $@ ${SRCS}
	@${CC} ${CFLAGS} -o $@ ${SRCS}
	@strip -s $@
