#!/bin/bash
# Workload E (zipfian): runs numa/numa, numa/regular, regular/numa, regular/regular
# into Result/AN_off/ycsb_E-50-50-50_E-100-0-50_zipfian.csv
exec bash /home/kiwo9430/NUMATyping/scripts/ycsb_run_workload.sh "E-50-50-50,E-100-0-50" zipfian
