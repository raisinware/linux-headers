=============
linux-headers
=============
Raw Linux Kernel userspace headers, automatically fetched by GitHub Actions.
The scripts to fetch new headers or install existing headers are under
`0BSD <https://spdx.org/licenses/0BSD.html>`_, while the actual headers are
under their original licenses (mostly ``GPL-2.0-only WITH Linux-syscall-note``)

Branches
========
Branches other then main can be rebased/deleted at any time. Please check the
GitHub wiki for current info on available branches.

Install
=======
There are 2 methods for easily installing the kernel headers.

meson:
^^^^^^

::

    meson setup build [-Dprefix=...] [-Dincludedir=...] [-Darch=...]
    [DESTDIR=...] meson install -C build

GNU Make:
^^^^^^^^^

::

    [PREFIX=...] make -n ARCH="..." [DESTDIR="/home/raisin/Documents/test"] install
