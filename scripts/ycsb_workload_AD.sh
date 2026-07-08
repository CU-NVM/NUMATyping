#!/bin/bash
# Workload AD (zipfian): runs numa/numa, numa/regular, regular/numa, regular/regular
# into Result/AN_off/ycsb_A-50-50-50_D-100-0-50_zipfian.csv
exec bash /home/kiwo9430/NUMATyping/scripts/ycsb_run_workload.sh "A-50-50-50,D-100-0-50" zipfian
