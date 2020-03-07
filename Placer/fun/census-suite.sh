#!/bin/bash
# Copyright 2020 Digital Aggregates Corporation, Arvada CO USA.
# Author: coverclock@diag.com
OUT=out/host
mkdir -p ${OUT}/tmp
rm -f ${OUT}/sql/census.db
cp /dev/null ${OUT}/tmp/CENSUS_TEST_FILE
census -D ${OUT}/sql/census.db -c -i -v .
census -D ${OUT}/sql/census.db -v -5
rm -f ${OUT}/tmp/CENSUS_TEST_FILE
census -D ${OUT}/sql/census.db -r -v .
census -D ${OUT}/sql/census.db -v -6
census -D ${OUT}/sql/census.db -v -0
