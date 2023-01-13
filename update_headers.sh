#!/bin/sh
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
#
## script to grab latest lts kernel sources and export raw uapi headers
set -e

NAME="linux"
#VERSION="$(curl -s https://www.kernel.org/ | grep -A1 'mainline:' -m1 | grep -oP '(?<=strong>).*(?=</strong.*)')"
VERSION="2.1.23"
#shellcheck disable=SC2086
MVER="$(echo $VERSION | cut -d. -f1)"
#URL="https://cdn.kernel.org/pub/$NAME/kernel/v$MVER.x/$NAME-$VERSION.tar.xz"
URL="https://cdn.kernel.org/pub/$NAME/kernel/v$MVER.1/$NAME-$VERSION.tar.xz"

# shellcheck disable=SC2317
on_exit () {
	echo "Unknown Error"
	exit 1
}

# shellcheck disable=SC3047
trap on_exit ERR 2>/dev/null || true

# check if we need to update headers
if grep -q "$VERSION" .version 1>/dev/null 2>&1
then
	echo "Kernel Headers already up to date."
	exit 0
fi
echo "$VERSION" > .version

# setup directories
rm -rf .kernel_tmp .header_tmp arch include
mkdir  .kernel_tmp .header_tmp arch include

# download and extract
cd       .kernel_tmp
echo     "Downloading from $URL..."
curl -OL "$URL"
tar -xf  "$NAME-$VERSION.tar.xz" --strip-components 1 --exclude-vcs
rm       "$NAME-$VERSION.tar.xz"

# install all available headers
#ARCHS="$(find ./arch/ -maxdepth 1 -type d | cut -c8-)"
ARCHS="x86 mips alpha m68k sparc sparc64 powerpc"
for arch in $ARCHS
do
	#mkdir "../.header_tmp/$arch"
	mkdir -p "../.header_tmp/$arch/include"
	echo  "Installing $arch headers..."
	make  distclean || true
	#make  headers_install ARCH="$arch" INSTALL_HDR_PATH="../.header_tmp/$arch" || true
	if [ "$arch" = "x86" ]
	then mv "include/asm-i386" "include/asm-x86"
	fi
	if [ "$arch" = "powerpc" ]
	then mv "include/asm-ppc" "include/asm-powerpc"
	fi
	cp    -rv "include/asm-$arch" "include/asm-generic" "include/linux" "include/net" "include/scsi" "../.header_tmp/$arch/include"
	mv    "../.header_tmp/$arch/include/asm-$arch" "../.header_tmp/$arch/include/asm"
done

# clean up kernel temp folder
cd ..
rm -rf ".kernel_tmp"

# prepare headers
cd   .header_tmp
find . -type d -empty -delete
ARCHS="$(find . -maxdepth 1 -type d | cut -c3-)"
for arch in $ARCHS
do
	echo  "Preparing $arch headers..."
	mkdir "../arch/$arch"
	cd    "$arch/include"
	mv    asm/ "../../../arch/$arch"
	cp    -rl ./*/ "../../../include" 2>/dev/null || true
	cd    "../.."
	rm    -rf "$arch/include"
done

# cleanup header tmp folder
cd ..
rm -rf ".header_tmp"
exit 0
