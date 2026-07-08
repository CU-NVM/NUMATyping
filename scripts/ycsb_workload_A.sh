#!/bin/bash
# Workload A (zipfian): runs numa/numa, numa/regular, regular/numa, regular/regular
# into Result/AN_off/ycsb_A-50-50-50_A-100-0-50_zipfian.csv
exec bash /home/kiwo9430/NUMATyping/scripts/ycsb_run_workload.sh "A-50-50-50,A-100-0-50" zipfian
