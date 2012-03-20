#!/bin/bash

if [ ! `which git` ] ; then
	echo "FATAL: This script requires git - install it from http://git-scm.org"
	exit 1
fi

DEP_DIR=../lib
DEP_SLNS=$DEP_DIR/petroules-qt-solutions
DEP_SINGLEAPP=$DEP_DIR/qtsingleapplication

function update-qtsolutions()
{
	# Remove existing directories
	if [ -d "$DEP_SLNS" ] ; then
		rm -rf "$DEP_SLNS"
	fi

	if [ -d "$DEP_SINGLEAPP" ] ; then
		rm -rf "$DEP_SINGLEAPP"
	fi

	echo "Fetching Petroules Utilities..."

	# Download Qt Solutions from Git
	pushd `dirname "$DEP_SLNS"` >/dev/null
	git clone git://gitorious.org/+petroules/qt-solutions/petroules-qt-solutions.git
	popd >/dev/null

	# Move QtSingleApplication into place
	mv "$DEP_SLNS/qtsingleapplication" "$DEP_DIR"
	rm -rf "$DEP_SLNS"
	rm -rf "$DEP_SINGLEAPP/doc"
	rm -rf "$DEP_SINGLEAPP/examples"
	rm "$DEP_SINGLEAPP/configure"*
	rm "$DEP_SINGLEAPP/INSTALL.txt"
	rm "$DEP_SINGLEAPP/README.txt"
}

if [ "$1" = "--update-dependencies" ] ; then
	update-qtsolutions
elif [ "$1" = "--remove-dependencies" ] ; then
	rm -rf "$DEP_SLNS"
	rm -rf "$DEP_SINGLEAPP"
elif [ "$1" = "--update-translations" ] ; then
	echo "Nothing to do..."
else
	echo "Usage: $0 [OPTION]"
	echo ""
	echo "  --update-dependencies"
	echo "      Updates all 3rd party dependencies to their latest versions"
	echo ""
	echo "  --remove-dependencies"
	echo "      Removes all 3rd party dependencies - will break the build"
	echo ""
	echo "  --update-translations"
	echo "      Updates all Qt translation files"
fi
