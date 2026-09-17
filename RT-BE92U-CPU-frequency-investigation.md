RT-BE92U CPU frequency investigation
=====================================

Source evidence
---------------
- Target profile: BCM6765 (targets/96765GW/96765GW.RT-BE92U).
- Kernel config has CONFIG_CPU_FREQ disabled.
- Kernel config has CONFIG_BCM_CPUFREQ disabled.
- No cpufreq governor, scaling_max_freq or scaling_min_freq interface is built.
- CPU idle is enabled through CONFIG_CPU_IDLE and CONFIG_BCM_ARM_CPUIDLE.
- Broadcom sysUtil_getCpuInfo() only reads the CPU Clock value from
  /proc/socinfo; the source contains no setter for the CPU clock.

Conclusion
----------
The RT-BE92U is not an x86-style turbo platform exposed through Linux cpufreq.
Its nominal CPU specification is quad-core 2.0 GHz, while the actual running
clock should be checked on the router with /proc/socinfo. The current firmware
has no software frequency governor that can be switched to performance mode.
Adding generic CPUFreq would not be a safe optimization: the BCM6765-specific
clock, voltage and firmware/PLL contract are not present in this kernel path.

Keeping every core awake is also not equivalent to increasing frequency. It
would require disabling CPU idle, increase heat and power, and normally does
not improve hardware-offloaded routing. That change is intentionally not in
the firmware.

Runtime verification after flashing
------------------------------------
cat /proc/socinfo
cat /proc/cpuinfo | grep -E 'processor|BogoMIPS|CPU MHz'
find /sys/devices/system/cpu -path '*/cpufreq/*' -maxdepth 6 -type f -print

Expected result for this build: /proc/socinfo reports the board CPU clock, and
there is no writable cpufreq policy directory. Compare CPU temperature and
WAN-LAN/LAN-LAN iperf3 before considering any clock or idle modification.

References
----------
- https://www.asus.com/uk/networking-iot-servers/wifi-routers/asus-wifi-routers/asus-rt-be92u/techspec/
- https://docs.kernel.org/admin-guide/pm/cpufreq.html
