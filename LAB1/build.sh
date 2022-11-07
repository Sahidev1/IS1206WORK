#!/bin/bash

gcc part_1.c -o part_1.out
gcc part_2_producer.c -o producer.out
gcc part_2_consumer.c -o consumer.out

echo "Build complete"