#!/bin/sh

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr SFM 5000 n=4 alpha=5 W=10000 vnode=40 t=5000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr CH 5000 n=4 vnode=40

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr SFM 2000 n=4 alpha=5 W=10000 vnode=40 t=10000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr CH 2000 n=4 vnode=40

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr SFM 10000 n=4 alpha=5 W=20000 vnode=40 t=10000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr CH 10000 n=4 vnode=40

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr SFM 15000 n=4 alpha=5 W=25000 vnode=40 t=15000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr CH 15000 n=4 vnode=40



./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr SFM 10000 n=4 alpha=5 W=20000 vnode=40 t=5000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/wiki_100million_1/sim_100million_1.tr CH 10000 n=4 vnode=40

