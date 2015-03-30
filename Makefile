
TARS= client.cxx stun_check.cxx stun_check.h stun.h udp.cxx udp.h \
	Makefile rfc3489.txt

# if you chnage this version, change in stun.h too 
VERSION=0.97

#CXXFLAGS+=-O2
#LDFLAGS+=-O2 -lssl
STUNLIB=libstun.a

#
# Alternatively, for debugging.
#
CXXFLAGS+=-g -O -Wall
LDFLAGS+=-g -O -Wall
# for solaris
#LDFLAGS+= -lnsl -lsocket


all: stun_check

clean:
	- rm *.o stun_check

tar: $(TARS)
	cd ..; tar cvfz `date +"stund/stund_$(VERSION)_$(PROG)%b%d.tgz"` \
			 $(addprefix stund/, $(TARS))

stun_check: stun_check.o stun.o udp.o 
	$(CXX) $(LDFLAGS) -o $@  $^

%.o:%.cxx
	$(CXX)  -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

libstun.a: stun.o udp.o
	ar rv $@ $<

%:RCS/%
	co $@

# Dependancies
stun_check.o: stun.h udp.h stun_check.h
stun.o: stun.h udp.h
udp.o: stun.h udp.h 
