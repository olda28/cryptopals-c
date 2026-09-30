#!/bin/bash
ln -sfn "$(basename "$1")" "$(dirname "$1")"/current.c
