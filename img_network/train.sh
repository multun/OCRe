duration=${1?missing training duration per pool}
pool_size=${2?missing pool size}

source charset.sh

set_size=${#enc_charset[@]}
pool_count=$(($set_size/$pool_size))
[[ $set_size/$pool_size -ne 0 ]] && pool_count=$(($pool_count + 1))

function pool_op()
{
    local pool_id=$1
    local pool_op=$2

    off=$(($pool_size * $pool_id))
    for (( j=0; j < pool_size && (off + j) < set_size; j++ )); do
	local ni=$((off + j))
	echo "${pool_op}ing ${charset[$ni]}'s daemon"
        systemctl --user "${pool_op}" "training_daemon@$(systemd-escape ${enc_charset[$ni]})"
    done
}

for (( i = 0; i < $pool_count; i++)); do
    echo "==== training pool $i"
    pool_op $i start
    sleep $duration
    pool_op $i stop
done
