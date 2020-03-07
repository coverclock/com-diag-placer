#!/bin/bash
OUT=./out/host
LOG=${OUT}/log
. ${OUT}/bin/setup
mkdir -p ${LOG}
(
    rm -f out/host/sql/survey.db
    cp /dev/null ${LOG}/SURVEY_TEST_FILE_1
    cp /dev/null ${LOG}/SURVEY_TEST_FILE_2
    ln ${LOG}/SURVEY_TEST_FILE_2 ${LOG}/SURVEY_TEST_FILE_3
    ln ${LOG}/SURVEY_TEST_FILE_2 ${LOG}/SURVEY_TEST_FILE_4
    ln ${LOG}/SURVEY_TEST_FILE_2 ${LOG}/SURVEY_TEST_FILE_5
    survey -D out/host/sql/survey.db -c -i -v .
    survey -D out/host/sql/survey.db -v -5
    rm ${LOG}/SURVEY_TEST_FILE_1
    survey -D out/host/sql/survey.db -r -v .
    survey -D out/host/sql/survey.db -v -6
    survey -D out/host/sql/survey.db -v -0
    survey -D out/host/sql/survey.db -7 -P $(realpath ${LOG}/SURVEY_TEST_FILE_2)
    survey -D out/host/sql/survey.db -7 -P $(realpath ${LOG}/SURVEY_TEST_FILE_3)
    survey -D out/host/sql/survey.db -7 -P $(realpath ${LOG}/SURVEY_TEST_FILE_4)
    survey -D out/host/sql/survey.db -7 -P $(realpath ${LOG}/SURVEY_TEST_FILE_5)
    survey -D out/host/sql/survey.db -8 -I $(ls -i ${LOG}/SURVEY_TEST_FILE_2 | cut -d ' ' -f 1)
    rm ${LOG}/SURVEY_TEST_FILE_2
    rm ${LOG}/SURVEY_TEST_FILE_3
    rm ${LOG}/SURVEY_TEST_FILE_4
    rm ${LOG}/SURVEY_TEST_FILE_5
) 2>&1 | tee ${LOG}/survey.log
