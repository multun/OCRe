set -e

size='16'

out_path="training_set"
fonts_dir="/usr/share/fonts/TTF"

mkdir -p "$out_path" "$fonts_dir"

alphabet='abcdefghijklmnopqrstuvwxyz'

for f in ${fonts_dir}/*; do
    echo "generating for font ${f}..."
    for (( i=0; i<${#alphabet}; i++ )); do
	cur_letter="${alphabet:$i:1}"
	out_dir="${out_path}/${cur_letter}"
	mkdir -p "${out_dir}"
	base_name="$(basename $f .${f##*.})"
	out_bmp="/tmp/${base_name}.bmp"
	echo convert -size 100x100 -background white -font "$f" -pointsize 25 \
		-depth 24 -compress none -fill black -gravity Center \
		caption:"${cur_letter}" -flatten "${out_bmp}"
	convert -size 100x100 -background white -font "$f" -pointsize 25 \
		-depth 24 -compress none -fill black -gravity Center \
		caption:"${cur_letter}" -flatten "bmp3:${out_bmp}"
	./bmp_to_custom "${out_bmp}" "${size}" "${out_dir}/${base_name}.cst"
	rm "${out_bmp}"
    done
done
