#!/bin/bash
set -e

size=$(cat $(dirname "$0")/../img_network/img_width)

fonts_dir=${1?missing font directory}
#fonts_dir="fonts"
out_path=${2?missing output directory}
#out_path="training_set"

mkdir -p "$out_path" "$fonts_dir"

source ../img_network/charset.sh

charset_size=${#charset[@]}
echo "Suppporting ${charset_size} characters"
for f in ${fonts_dir}/*; do
    echo "generating for font ${f}..."
    for (( i=0; i<$charset_size; i++)); do
	#for cur_letter in "${enc_charset[@]}"; do
	cur_letter="${charset[$i]/\\/\\\\}"
	enc_cur_letter="${enc_charset[$i]}"
	out_dir="${out_path}/${enc_cur_letter}"
	mkdir -p "${out_dir}"
	base_name="$(basename $f .${f##*.})"
	out_cst="${out_dir}/${base_name}.cst"
	out_bmp="/tmp/${base_name}.bmp"
	[[ -f "${out_cst}" ]] && continue
	convert -size 100x100 -background white -font "$f" -pointsize 25 \
		-depth 24 -compress none -fill black -gravity Center \
		caption:"${cur_letter}" -flatten "bmp3:${out_bmp}"
        ./bmp_to_custom "${out_bmp}" "${size}" "${out_cst}"
	rm "${out_bmp}"
    done
done
