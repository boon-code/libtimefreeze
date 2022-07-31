#!/bin/bash

# vim:set softtabstop=8 shiftwidth=8 tabstop=8 noexpandtab:

[ -n "${NUM_JOBS}" ] || NUM_JOBS=5
[ -n "${TOPDIR}" ] || TOPDIR="$(pwd)"
[ -n "${BUILD_DIR}" ] || BUILD_DIR="${TOPDIR}/.build"

debug_enabled=0
coverage_enabled=0
incremental_build=0
verbose_build=0
use_ninja=0
force_make=0
strict=1
optimization=

_usage() {
	local dbgen="disabled"
	[ -z "${DEBUG}" ] || dbgen="enabled"

	echo "$0 [-cigvNM] [-O LVL]"
	echo ""
	echo "Options:"
	echo " -c ....... capture coverage data"
	echo " -i ....... build incrementally"
	echo " -g ....... enable debug mode"
	echo " -O LVL ... level of optimization"
	echo " -v ....... verbose build"
	echo " -N ....... use ninja build"
	echo " -M ....... use make (default)"
	echo ""
	echo "Variables:"
	echo " - BUILD_DIR ... directory to build in:"
	echo "                '${BUILD_DIR}'"
	echo " - TOPDIR ...... top level directory of this project"
	echo "                '${TOPDIR}'"
	echo " - NUM_JOBS .... number of parallel jobs during compilation:"
	echo "                '${NUM_JOBS}'"
	echo " - DEBUG ....... debug messages of this script:"
	echo "                '${dbgen}'"
	echo ""
	_get_build_status
}

_is_ninja_build() {
	[ -f "${BUILD_DIR}/rules.ninja" ] || \
	[ -f "${BUILD_DIR}/build.ninja" ]
}

_is_make_build() {
	[ -f "${BUILD_DIR}/Makefile" ]
}

_get_build_status() {
	if _is_make_build; then
		echo "Build environment initialized using make"
	elif _is_ninja_build; then
		echo "Build environment initialized using ninja"
	else
		echo "Build environment not yet initialized"
	fi
}

_parse_args() {
	local ch=""

	_dbg "Arguments: $@"

	while getopts 'cigO:vNM' ch; do
		case "$ch" in
		c) coverage_enabled=1;debug_enabled=1;;
		i) incremental_build=1;;
		g) debug_enabled=1;;
		O) optimization="$OPTARG";;
		v) verbose_build=1;;
		N) use_ninja=1;;
		M) force_make=1;;
		h|?) _usage; exit 0;;
		*) _usage; exit 1;;
		esac
	done
}

_fail() {
	echo "FAILURE: $@" >&2
	exit 1
}

_dbg() {
	if [ -n "$DEBUG" ]; then
		echo "DBG: $@" >&2
	fi
}

_set_flags() {
	if [ $debug_enabled -eq 1 ]; then
		export CFLAGS="$CFLAGS -Wall -Wextra -g "
		_dbg "flags: Debug configuration"
	fi
	if [ -n "$optimization" ]; then
		export CFLAGS="$CFLAGS -O${optimization} "
		_dbg "flags: Use optimization level $optimization"
	fi
	if [ -n "$strict" ]; then
		export CFLAGS="$CFLAGS -Werror "
		_dbg "flags: Use strict mode"
	fi

	_dbg "flags: CFLAGS=$CFLAGS"
}

_setup_cmake() {
	local buildtype="RelWithDebInfo"
	local generator="" # default is make
	local args=""

	_dbg "Prepare CMake build environment in ${BUILD_DIR}"

	mkdir -p "${BUILD_DIR}/"
	if [ -n "$optimization" ]; then
		buildtype=""
	fi
	if [ $debug_enabled -eq 1 ]; then
		buildtype="Debug"
	fi
	if [ $coverage_enabled -eq 1 ]; then
		args="$args -DCOVERAGE=ON"
	fi
	if [ -z "$buildtype" ]; then
		# Use debug with release configuration here
		export CFLAGS="$CFLAGS -DNDEBUG -g"
	fi
	if [ ${use_ninja} -eq 1 ]; then
		generator="-G Ninja"
	fi

	cd "${BUILD_DIR}/" || _fail "Failed to switch to the build directory '${BUILD_DIR}'"

	_cmake_wrap "$TOPDIR" "$@" \
		$generator \
		"-DCMAKE_BUILD_TYPE=${buildtype}" \
		"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON" \
		$args
}

_cmake_wrap() {
	_dbg "CMake command: $@"
	cmake "$@"
}

_compile() {
	if [ ${use_ninja} -eq 1 ]; then
		_ninja_build
	elif [ ${force_make} -eq 1 ]; then
		_make
	else  # autodetect
		if _is_ninja_build; then
			_ninja_build
		else
			_make
		fi
	fi
}

_ninja_build() {
	local args=""
	if [ ${verbose_build} -eq 1 ]; then
		args="$args -v"
		NUM_JOBS=1
		_dbg "Set NUM_JOBS variable to 1"
	fi
	ninja -C "${BUILD_DIR}" -j "${NUM_JOBS}" $args
}

_make() {
	local args=""
	if [ ${verbose_build} -eq 1 ]; then
		args="$args VERBOSE=1"
		NUM_JOBS=1
		_dbg "Set NUM_JOBS variable to 1"
	fi
	make -C "${BUILD_DIR}" -j "${NUM_JOBS}" $args
}

# currently unused
_strip() {
	local appfile="${1?}"

	"${STRIP?}" --strip-debug -o "${appfile}" "${appfile}.debug" || return 1
	"${OBJCOPY?}" --add-gnu-debuglink="${appfile}.debug" "${appfile}"
}

_build_needs_init() {
	if _is_ninja_build || _is_make_build; then
		return 1  # false
	else
		_dbg "Build needs to be initialized"
		return 0  # true
	fi
}

main() {
	_parse_args "$@"
	if [ ${incremental_build} -eq 0 ] || _build_needs_init; then
		( _set_flags && _setup_cmake ) || _fail "Failed to prepare CMake build environment"
	fi
	_compile || _fail "Compilation failed"
	exit 0
}

main "$@"
