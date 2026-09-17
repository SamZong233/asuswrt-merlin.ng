RT-BE92U 10Gbps optimization survey
====================================

Current kernel fast path
------------------------
The RT-BE92U HND-96765 configuration already enables Broadcom Crossbow full
offload, FHW, PKTFLOW, Archer WLAN, checksum offload, software GSO threading,
SPU hardware offload, and the Broadcom Ethernet implementation. PREEMPT and
HZ=1000 are already enabled. BWDPI is disabled in the product profile.

Safe conclusions
----------------
1. Preserve the Broadcom hardware fast path for ordinary NAT/bridge traffic.
2. Avoid enabling CPU-bound QoS shaping (especially CAKE), VPN encryption,
   DPI, bridge netfilter or verbose logging when measuring raw 10Gbps speed.
3. Do not globally force RPS/XPS. Linux documentation notes that RPS is a
   software receive path and can be redundant or add IPIs when hardware RSS
   already distributes queues; it also requires runtime queue masks.
4. NF_FLOW_TABLE and BCM_NFT_OFFLOAD are not enabled. They should only be
   tested if this firmware's Broadcom integration explicitly wires them into
   the existing proprietary acceleration path.
5. BCM_ENET_TC_OFFLOAD is a QoS/tc offload experiment, not a guaranteed NAT
   throughput gain. Enabling it can change QoS behavior.
6. Jumbo-frame support is present, but it improves efficiency only when every
   hop supports the same larger MTU. It does not raise normal Internet MTU.
7. Changing HZ or preemption may trade latency against CPU overhead; it should
   be benchmarked on the real router and is not a blind 10Gbps optimization.

Recommended validation on hardware
-----------------------------------
Record a baseline and candidate result for WAN-LAN and LAN-LAN iperf3, CPU
load, latency and packet loss. Also collect ethtool feature/queue state,
/sys/class/net/*/queues, /proc/interrupts and Broadcom Crossbow/PKTFLOW
counters. Only retain a change that improves throughput without breaking
hardware offload or latency.

References
----------
- https://docs.kernel.org/networking/netdev-features.html
- https://docs.kernel.org/networking/multiqueue.html
- https://docs.kernel.org/networking/scaling.html
- https://docs.kernel.org/5.16/networking/nf_flowtable.html
