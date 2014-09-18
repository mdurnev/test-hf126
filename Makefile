DESTDIR ?=
prefix ?= /usr
CFLAGS ?= -O2
LDFLAGS ?= -Wl,--hash-style=gnu

all: test_hf126

test_hf126: src/test_hf126.o
	$(CC) -o $@ $^ -lsystemd-daemon $(LDFLAGS)

install:
	install -d $(DESTDIR)$(prefix)/bin
	install -m 0755 test_hf126 $(DESTDIR)$(prefix)/bin/
	install -d $(DESTDIR)/etc/systemd/system
	install -m 0644 systemd/test_hf126.service $(DESTDIR)/etc/systemd/system/
	sed -i "s:@prefix@:$(prefix):" $(DESTDIR)/etc/systemd/system/test_hf126.service

clean:
	rm -f src/*.o
	rm -f test_hf126

.PHONY: clean
