#!/bin/bash

[ -n "${NUM_JOBS}" ] || NUM_JOBS=5
[ -n "${TOPDIR}" ] || TOPDIR="$(pwd)"
[ -n "${BUILD_DIR}" ] || BUILD_DIR="${TOPDIR}/.build"

debug_enabled=0
coverage_enabled=0
incremental_build=0
verbose_build=0
optimization=

_usage() {
	local dbgen="disabled"
	[ -z "${DEBUG}" ] || dbgen="enabled"

	echo "$0 [-cigv] [-O LVL]"
	echo ""
	echo "Options:"
	echo " -c ....... capture coverage data"
	echo " -i ....... build incrementally"
	echo " -g ....... enable debug mode"
	echo " -O LVL ... level of optimization"
	echo " -v ....... verbose build"
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
	fi
	if [ -n "$optimization" ]; then
		export CFLAGS="$CFLAGS -O${optimization} "
	fi

	_dbg "CFLAGS: $CFLAGS"
}

_setup_cmake() {
	local buildtype="RelWithDebInfo"
	local args=""

	_dbg "Prepare CMake build environment in ${BUILD_DIR}"

	mkdir -p "${BUILD_DIR}/"
	if [ $debug_enabled -eq 1 ]; then
		buildtype="Debug"
	fi
	if [ $coverage_enabled -eq 1 ]; then
		args="$args -DCOVERAGE=ON"
	fi

	cd "${BUILD_DIR}/" || _fail "Failed to switch to the build directory '${BUILD_DIR}'"

	_cmake_wrap "$TOPDIR" "$@" \
		"-DCMAKE_BUILD_TYPE=${buildtype}" \
		"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON" \
		$args
}

_cmake_wrap() {
	_dbg "CMake command: $@"
	cmake "$@"
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

_parse_args() {
	local ch=""

	_dbg "Arguments: $@"

	while getopts 'cigO:v' ch; do
		case "$ch" in
		c) coverage_enabled=1;debug_enabled=1;;
		i) incremental_build=1;;
		g) debug_enabled=1;;
		O) optimization="$OPTARG";;
		v) verbose_build=1;;
		h|?) _usage; exit 0;;
		*) _usage; exit 1;;
		esac
	done
}

main() {
	_parse_args "$@"
	if [ ${incremental_build} -eq 0 ]; then
		( _set_flags && _setup_cmake ) || _fail "Failed to prepare CMake build environment"
	fi
	_make || _fail "Compilation failed"
	exit 0
}

main "$@"
