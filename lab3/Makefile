# name of the program (Minix service)
PROG=lab3

# source code files to be compiled
SRCS = lab3.c keyboard.c utils.c timer.c kbc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

export C_INCLUDE_PATH=C:\Users\guire\Desktop\LCOM\MINIX-LCOM\shared\lab2\timer.h

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf
CPPFLAGS += -D LAB

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk> 