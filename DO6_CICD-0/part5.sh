#!/bin/bash
host=10.20.0.2

scp $@ root@"$host":/usr/local/bin
