#!/bin/sh

regular_charset=( {a..z} {A..Z} {0..9} ç Ç é É è È ë Ë ê Ê ï Ï î Î ü Ü æ Æ œ Œ ﬁ ĳ ﬂ ﬀ @ \+ \- \/ \= \: \; \! \? \\)
low_charset=( \, \. )
high_charset=( )

charset=( "${regular_charset[@]}" "${low_charset[@]}" "${high_charset[@]}" )

enc_charset=( )

function encode
{
    echo $(echo $1 | base64)
}

for e in "${charset[@]}"; do
    enc_charset+=( $(encode $e) )
done
