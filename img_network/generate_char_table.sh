#!/bin/bash
out=${1?missing output filename}
. $(dirname $0)/charset.sh

function cformat()
{
    printf '"'
    printf "$1" | hexdump -v -e '1/1 " %02X"' | sed 's/ /\\x/g'
    printf '"'
}

function format_array()
{
    local aname=$1[@]
    cset=( "${!aname}" )
    for (( i=0; i<"${#cset[@]}"; i++ )) do
        truename="${cset[i]}"

	dirname=$(encode "${truename}")
	echo "  {\"$dirname\", $(cformat $truename), $2}, /* $(echo $truename | sed 's/\\/\\\\/g') */"
    done
}

function format_all
{
    format_array regular_charset CAT_REGULAR
    format_array low_charset CAT_LOW
    format_array high_charset CAT_HIGH
}

cat > "${out}"<<EOF
#include "training.h"
#include <stdlib.h>

size_t cats_bindings_size = ${#charset[@]};
t_cat_binding cats_bindings[] = {
$(format_all)
};
EOF
