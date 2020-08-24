#!/bin/bash
set -e
set -x

if [ "$1" = 'graph500_reference_bfs' ]; then
	/root/graph500/src/graph500_reference_bfs $2 $3
elif [ "$1" = 'graph500_reference_bfs_sssp' ]; then
	/root/graph500/src/graph500_reference_bfs_sssp $2 $3
else
	exec "$@"
fi
