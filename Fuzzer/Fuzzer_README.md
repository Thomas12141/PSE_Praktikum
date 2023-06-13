First, open a terminal in the direktory of the fuzzer. Enter into the terminal:
~~~
sudo bash -c 'echo core >/proc/sys/kernel/core_pattern'
~~~
Then open a new terminal and enter:
~~~
cd /sys/devices/system/cpu
sudo bash -c 'echo performance | tee cpu*/cpufreq/scaling_governor'
~~~
Finally let fuzzer run with:
~~~
./afl_run.sh
~~~