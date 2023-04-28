#!/bin/sh

if [ "$1" ] && [ "$RDM_HOME"=="" ]; then
   echo 'USAGE: Setup_SQLSamples.sh <RDM Install directory>'
   exit 1
fi

cd ${RDM_HOME}/GettingStarted/examples/sql_db

echo 'Setting up NSFAWARDS example database…'

rdmsql -b nsfawards.sql
rdmsql -b impdb_nsfawards.sql

echo 'Setting up BOOKSHOP example database…'

rdmsql -b bookshop.sql
rdmsql -b impdb_bookshop.sql

echo 'Setup Complete!'

cd ${RDM_HOME}/GettingStarted

exit 0
