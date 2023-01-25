#!/bin/bash

for i in $(seq 1 200)
do
    curl http://loadbalancer.server/lorem_ipsum.html
done