#!/bin/bash

script_name=$(basename "$0")

if [ "$script_name" = "template_task.sh" ]; then
	echo "я бригадир, сам не рабюотаю"
	exit 1
fi

log_file="report_${script_name%.*}.log"
echo "[$$] $(date '+Y-%m-%d %H:%M:%S') Скрипт запущен" >> "$log_file"

delay=$(( RANDOM % 1771 + 30 ))
sleep $delay

minutes=$(( (delay + 59)/ 60 )) 
echo "[$$] $(date '+%Y-%m-%d %H:%M:%S') Скрипт завершился, работал $minutes минут" >> "$log_file"
