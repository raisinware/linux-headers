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
project(
	'linux-headers',
	'c',
	version: '2.6.x'
)

fs = import('fs')
version = fs.read('.version')
message('Kernel Headers Version: ' + version)

include_dir = get_option('includedir')

install_subdir('include/',
               install_dir: include_dir,
			   strip_directory: true,
			   install_tag: 'devel')

selected_arch = get_option('arch')
target_arch = build_machine.cpu_family()

if (selected_arch == 'auto')
	if (target_arch == 'aarch64')
		selected_arch = 'arm64'
	elif (target_arch == 'mips64')
		selected_arch = 'mips'
	elif (target_arch == 'ppc') or (target_arch == 'ppc64')
		selected_arch = 'powerpc'
	elif (target_arch == 'riscv32') or (target_arch == 'riscv64')
		selected_arch = 'riscv'
	elif (target_arch == 's390x')
		selected_arch = 's390'
	elif (target_arch == 'sh4')
		selected_arch = 'sh'
	elif (target_arch == 'sparc64')
		selected_arch = 'sparc'
	elif (target_arch == 'x86_64')
		selected_arch = 'x86'
	else
		selected_arch = target_arch
	endif
endif

message('Installing kernel headers for: ' + selected_arch)
if (selected_arch != 'generic')
	arch_dir = join_paths('arch', selected_arch)
	install_subdir(arch_dir,
	               install_dir: include_dir,
				   strip_directory: true,
				   install_tag: 'devel')
endif
