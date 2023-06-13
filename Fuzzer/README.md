# Running the fuzzer
```bash
# Navigate to fuzzer directory
$ cd /path/to/pse-webserver/Fuzzer
# Redirect core dumps (optional fix)
$ sudo bash -c 'echo core >/proc/sys/kernel/core_pattern'
# Disable cpu scaling (optional fix)
$ cd /sys/devices/system/cpu
$ sudo bash -c 'echo performance | tee cpu*/cpufreq/scaling_governor'
# Navigate to fuzzer directory
$ cd /path/to/pse-webserver/Fuzzer
# Start the fuzzer
$ sh ./afl_run.sh
```