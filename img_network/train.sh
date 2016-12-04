units_dir=$(dirname $0)/units
duration=${1?missing training duration per pool}
pool_size=${2?missing pool size}
cycles=${3?missing cycles count}

source charset.sh

set_size=${#enc_charset[@]}
pool_count=$(($set_size/$pool_size))
[[ $set_size/$pool_size -ne 0 ]] && pool_count=$(($pool_count + 1))

systemctl --user daemon-reload

function pool_op()
{
  local pool_id=$1
  local pool_op=$2

  off=$(($pool_size * $pool_id))
  for (( j=0; j < pool_size && (off + j) < set_size; j++ )); do
    local ni=$((off + j))
    unit_name="${enc_charset[$ni]}"
    mkdir -p "${units_dir}/${unit_name}"
    echo "${pool_op}ing ${charset[$ni]}'s daemon"
    systemctl --user "${pool_op}" "training_daemon@$(systemd-escape ${unit_name})"
  done
}

for (( epoch = 0; epoch < $cycles; epoch++)); do
  for (( i = 0; i < $pool_count; i++)); do
    printf "==[%2d]= training pool %d\n" $epoch $i
    pool_op $i start
    sleep $duration
    pool_op $i stop
  done
done
