#!/bin/bash
OUT=./out/host
LOG=${OUT}/log
. ${OUT}/bin/setup
mkdir -p ${LOG}
(
    rm -f out/host/sql/census.db
    cp /dev/null ${LOG}/CENSUS_TEST_FILE
    census -D out/host/sql/census.db -c -i -v .
    census -D out/host/sql/census.db -v -5
    rm -f ${LOG}/CENSUS_TEST_FILE
    census -D out/host/sql/census.db -r -v .
    census -D out/host/sql/census.db -v -6
    census -D out/host/sql/census.db -v -0
) 2>&1 | tee ${LOG}/census.log
