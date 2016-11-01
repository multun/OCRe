
ABC=abcdefghijklmnopqrstuvwxyz

for f in fonts/*.ttf; do
    for (( i=0; i<${#ABC}; i++ )); do
	convert -size 28x28 -background white -font "$f" \
		-pointsize 25 -fill black -gravity Center \
		caption:"${ABC:$i:1}" -flatten \
		"TrainingSet/${ABC:$i:1}_$(basename $f .${f##*.}).bmp"
    done
done
