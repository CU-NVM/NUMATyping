#!/bin/bash
# Workload B (zipfian): runs numa/numa, numa/regular, regular/numa, regular/regular
# into Result/AN_off/ycsb_B-50-50-50_B-100-0-50_zipfian.csv
exec bash /home/kiwo9430/NUMATyping/scripts/ycsb_run_workload.sh "B-50-50-50,B-100-0-50" zipfian
