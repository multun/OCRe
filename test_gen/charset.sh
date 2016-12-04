charset=( {a..z} {A..Z} {0..9} ç Ç é É è È ë Ë ê Ê ï Ï î Î ü Ü æ Æ œ Œ ﬁ ĳ ﬂ ﬀ @ + - / = : \; , . ! ? \\)
for i in "${charset[@]}"; do
    echo $i
done

echo "length is ${#charset[@]}"
