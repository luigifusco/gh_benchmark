#!/bin/bash
export CUDA_VISIBLE_DEVICES=$SLURM_LOCALID
exec $*