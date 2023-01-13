# SPDX-License-Identifier: 0BSD
# Copyright (C) 2022-2023 by raisinware
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

PREFIX ?= /usr/local
INSTALL_HDR_PATH = $(DESTDIR)$(PREFIX)
ARCH ?= $(error ARCH must be set)
INSTALL_CMD = install

# GNU Make is a pile of crap
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
ARCH_INCLUDES = $(wildcard arch/$(ARCH)/asm/*.h)
GENERIC_INCLUDES = $(call rwildcard,include,*.h)

all:
defconfig:
clean:
distclean:

headers_install: install

install: $(ARCH_INCLUDES:arch/$(ARCH)/%=$(INSTALL_HDR_PATH)/include/%) $(GENERIC_INCLUDES:include/%=$(INSTALL_HDR_PATH)/include/%)

$(INSTALL_HDR_PATH)/include/%: arch/$(ARCH)/%
	$(INSTALL_CMD) -D -m 644 $< $@

$(INSTALL_HDR_PATH)/include/%: include/%
	$(INSTALL_CMD) -D -m 644 $< $@

.PHONY: all defconfig headers_install install clean distclean
