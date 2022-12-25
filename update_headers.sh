#!/bin/sh
## Grab kernel sources and export raw uapi headers
set -e

NAME="linux"
VERSION="$(curl -s https://www.kernel.org/ | grep -A1 'longterm:' -m1 | grep -oP '(?<=strong>).*(?=</strong.*)')"
#shellcheck disable=SC2086
MVER="$(echo $VERSION | cut -d. -f1)"
URL="https://cdn.kernel.org/pub/$NAME/kernel/v$MVER.x/$NAME-$VERSION.tar.xz"

# shellcheck disable=SC2317
on_exit () {
	echo "Unknown Error"
	exit 1
}

# shellcheck disable=SC3047
trap on_exit ERR

# check if we need to update headers
if grep -q "$VERSION" .version 1>/dev/null 2>&1
then
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
ARCHS="$(find ./arch/ -maxdepth 1 -type d | cut -c8-)"
for arch in $ARCHS
do
	mkdir "../.header_tmp/$arch"
	echo  "Installing $arch headers..."
	make  distclean || true
	make  headers_install ARCH="$arch" INSTALL_HDR_PATH="../.header_tmp/$arch" || true
done

# clean up kernel temp folder
cd ..
rm -rf ".kernel_tmp"

# install headers create file listing
cd   .header_tmp
find . -type d -empty -delete
ARCHS="$(find . -maxdepth 1 -type d | cut -c3-)"
for arch in $ARCHS
do
	echo  "Preparing $arch headers..."
	mkdir "../arch/$arch"
	cd    "$arch/include"
	mv    asm/ "../../../arch/$arch"
	cp    -rl ./*/ "../../../include" || true
	cd    "../.."
	rm    -rf "$arch/include"
done

# cleanup header tmp folder
cd ..
rm -rf ".header_tmp"
exit 0
