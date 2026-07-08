#!/bin/bash
# Workload C (zipfian): runs numa/numa, numa/regular, regular/numa, regular/regular
# into Result/AN_off/ycsb_C-50-50-50_C-100-0-50_zipfian.csv
exec bash /home/kiwo9430/NUMATyping/scripts/ycsb_run_workload.sh "C-50-50-50,C-100-0-50" zipfian
