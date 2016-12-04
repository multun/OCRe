proj_root=$(realpath $(dirname $0)/../../)
training_set="${proj_root}/test_gen/training_set"
bin_path="${proj_root}/img_network/fuckit"
working_dir="${proj_root}/img_network/units"
cat > ~/.config/systemd/user/training_daemon@.service<<EOF
[Unit]
Description=OCR training daemon for category %i

[Service]
Type=simple
WorkingDirectory=${working_dir}
ExecStart=${bin_path} %i ${training_set} 1>&2
KillSignal=SIGINT
EOF
