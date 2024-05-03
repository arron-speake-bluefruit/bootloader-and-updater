#!/bin/bash
#
# This script generates a small C source file containing a description of the current git
# description.
#

set -e

# check argument count
if [ $# -ne 1 ]; then
    echo "$0: error: expected 1 argument" >&2
    exit 1
fi

TARGET_PATH="$1"

# generate new file content
PROLOGUE=$'#include "git_version.h"\nconst char* const git_version = \"'
EPILOGUE=$'";'
VERSION="$(git describe --dirty --always)"
NEW_CONTENT="$PROLOGUE$VERSION$EPILOGUE"

# if the target file doesn't exist, create it immediately
if [ ! -e $TARGET_PATH ]; then
    echo "$NEW_CONTENT" >$TARGET_PATH
    echo "$0: $TARGET_PATH created (version: $VERSION)" >&2
    exit 0
fi

# get the previous file content
OLD_CONTENT=$(cat $TARGET_PATH)

if [ "$NEW_CONTENT" == "$OLD_CONTENT" ]; then
    # if the content is the same, don't do anything
    echo "$0: $TARGET_PATH not changed (version: $VERSION)" >&2
else
    # if the content has changed, update the file
    echo "$NEW_CONTENT" >$TARGET_PATH
    echo "$0: $TARGET_PATH changed (version: $VERSION)" >&2
fi
