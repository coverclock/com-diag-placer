#!/bin/bash
# Copyright 2020 Digital Aggregates Corporation, Arvada CO USA.
# Author: coverclock@diag.com
OUT=./$(make out)
mkdir -p ${OUT}/tmp
rm -f ${OUT}/sql/survey.db
cp /dev/null ${OUT}/tmp/SURVEY_TEST_FILE_1
cp /dev/null ${OUT}/tmp/SURVEY_TEST_FILE_2
ln ${OUT}/tmp/SURVEY_TEST_FILE_2 ${OUT}/tmp/SURVEY_TEST_FILE_3
ln ${OUT}/tmp/SURVEY_TEST_FILE_2 ${OUT}/tmp/SURVEY_TEST_FILE_4
ln ${OUT}/tmp/SURVEY_TEST_FILE_2 ${OUT}/tmp/SURVEY_TEST_FILE_5
survey -D ${OUT}/sql/survey.db -c -i -v .
survey -D ${OUT}/sql/survey.db -v -5
rm ${OUT}/tmp/SURVEY_TEST_FILE_1
survey -D ${OUT}/sql/survey.db -r -v .
survey -D ${OUT}/sql/survey.db -v -6
survey -D ${OUT}/sql/survey.db -v -0
survey -D ${OUT}/sql/survey.db -7 -P $(realpath ${OUT}/tmp/SURVEY_TEST_FILE_2)
survey -D ${OUT}/sql/survey.db -7 -P $(realpath ${OUT}/tmp/SURVEY_TEST_FILE_3)
survey -D ${OUT}/sql/survey.db -7 -P $(realpath ${OUT}/tmp/SURVEY_TEST_FILE_4)
survey -D ${OUT}/sql/survey.db -7 -P $(realpath ${OUT}/tmp/SURVEY_TEST_FILE_5)
survey -D ${OUT}/sql/survey.db -8 -I $(ls -i ${OUT}/tmp/SURVEY_TEST_FILE_2 | cut -d ' ' -f 1)
rm ${OUT}/tmp/SURVEY_TEST_FILE_2
rm ${OUT}/tmp/SURVEY_TEST_FILE_3
rm ${OUT}/tmp/SURVEY_TEST_FILE_4
rm ${OUT}/tmp/SURVEY_TEST_FILE_5
