#!/bin/bash
# Workload F (zipfian): runs numa/numa, numa/regular, regular/numa, regular/regular
# into Result/AN_off/ycsb_F-50-50-50_F-100-0-50_zipfian.csv
exec bash /home/kiwo9430/NUMATyping/scripts/ycsb_run_workload.sh "F-50-50-50,F-100-0-50" zipfian
