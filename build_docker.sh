#!/usr/bin/env bash

set -e

docker build -t gwangmin/orbslam:latest -f Dockerfile .
