#!/bin/sh

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr SFM 2000 n=4 alpha=5 W=8000 vnode=40 t=2000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr CH 2000 n=4 vnode=40

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr SFM 5000 n=4 alpha=5 W=20000 vnode=40 t=5000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr CH 5000 n=4 vnode=40

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr SFM 10000 n=4 alpha=5 W=40000 vnode=40 t=10000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr CH 10000 n=4 vnode=40

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr SFM 15000 n=4 alpha=5 W=60000 vnode=40 t=15000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr CH 15000 n=4 vnode=40

./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr SFM 20000 n=4 alpha=5 W=80000 vnode=40 t=20000;
./webcachesim /mnt/c/Users/28347/Documents/CDN/trace/cdn1_100m.tr CH 20000 n=4 vnode=40

