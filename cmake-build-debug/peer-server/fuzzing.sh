#!/usr/bin/env bash
exec 3<>/dev/tcp/127.0.0.1/8080
cat /dev/urandom >&3