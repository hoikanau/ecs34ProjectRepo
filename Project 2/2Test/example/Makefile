# The location of the expat directory

XPDIR=/expat/xmlparse

# Expat doesn't actually link itself into a library, so we
# have to refer to the individual objects that make up expat

XP_OBJ = $(XPDIR)/xmltok/xmltok.o \
         $(XPDIR)/xmltok/xmlrole.o \
         $(XPDIR)/xmlparse/xmlparse.o \
         $(XPDIR)/xmlparse/hashtable.o

XP_INC = -I$(XPDIR)/xmltok -I$(XPDIR)/xmlparse

################
# Adds support for checking lexical aspects of namespaces
XP_NS = -DXML_NS

################
# Adds support for processing DTDs
XP_DTD = -DXML_DTD

################
# Byte order macro
#This is for little endian machines

XP_BO = -DXML_BYTE_ORDER=12

# and this one is for big endian machines

# XP_BO = -DXML_BYTE_ORDER=21


################
# Define XML_Char as unsigned short
# XP_UNI = -DXML_UNICODE

################
# Define XML_Char as wchar_t
#XP_UNI_WC = -DXML_UNICODE_WCHAR_T

################
# Uncomment XP_MM if your system has a bcopy function but not memmove
#XP_MM = -D"memmove(d,s,l)=bcopy(s,d,l)"


XP_DEFINES = $(XP_NS) $(XP_DTD) $(XP_BO) $(XP_UNI) $(XP_UNI_WC) $(XP_MM)

################
# To get debugging, set this to -g
OPT = -O

CFLAGS = $(OPT) $(XP_INC) $(XP_DEFINES)

help:
	@echo
	@echo "*********************************************************"
	@echo "** Provide as an argument the executable that you"
	@echo "** want built. The choices are:"
	@echo "**	outline"
	@echo "**	line"
	@echo "**	namespace"

outline: outline.o $(XP_OBJ)
	$(CC) -o outline outline.o $(XP_OBJ)

line: line.o $(XP_OBJ)
	$(CC) -o line line.o $(XP_OBJ)

namespace: namespace.o $(XP_OBJ)
	$(CC) -o namespace namespace.o $(XP_OBJ)
