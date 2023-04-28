static const char *const tims_ddl=
   "/*----------------------------------------------------------------\n"
   "    Technical Information Management System (TIMS) Database\n"
   "----------------------------------------------------------------*/\n"
   "database tims {\n"
   "    data file \"tims.d01\" contains system, key_word, intersect;\n"
   "    data file \"tims.d02\" contains author, borrower, info, infotext;\n"
   "    vardata file \"tims.v01\" contains info_title;\n"
   "    vardata file \"tims.v02\" contains id_code;\n"
   "    key  file \"tims.k01\" contains id_code;\n"
   "    key  file \"tims.k02\" contains frnd, name, kword;\n"
   "\n"
   "    record author {\n"
   "        unique key char name[32];    /* author's name: \"last, first\" */\n"
   "    }                                /* or editor's name */\n"
   "    record info {\n"
   "        unique key varchar id_code[48]; /* dewey dec. or own coding tech. */\n"
   "        varchar info_title[80];         /* title of book, article, mag. */\n"
   "        char publisher[32];          /* name of publisher - prob. coded */\n"
   "        char pub_date[12];           /* date of publication  \n"
   "                                        (e.g. most recent copyright) */\n"
   "        int16_t info_type;           /* 0 = book, 1 = magazine, 2 = article */\n"
   "    }\n"
   "    record borrower {\n"
   "        key char frnd[32];           /* name of borrower */\n"
   "        int32_t date_borrowed;       /* dates are stored initially as */\n"
   "        int32_t date_returned;       /* numeric YYMMDD (e.g. 870226) */\n"
   "    }\n"
   "    record infotext {\n"
   "        char line[80];               /* line of abstract text */\n"
   "    }\n"
   "    record key_word {\n"
   "        unique key char kword[32];   /* subject key words or classification */\n"
   "    }\n"
   "    record intersect {\n"
   "        int16_t int_type;            /* copy of info_type to save I/O */\n"
   "    }                                /* when looking only for, say, books */\n"
   "    set author_list {\n"
   "        order ascending;\n"
   "        owner system;\n"
   "        member author by name;\n"
   "    }\n"
   "    set has_published {\n"
   "        order ascending;\n"
   "        owner author;\n"
   "        member info by info_title;\n"
   "    }\n"
   "    set article_list {\n"
   "        order last;\n"
   "        owner info;\n"
   "        member info;\n"
   "    }\n"
   "    set loaned_books {\n"
   "        order last;\n"
   "        owner info;\n"
   "        member borrower;\n"
   "    }\n"
   "    set abstract {\n"
   "        order last;\n"
   "        owner info;\n"
   "        member infotext;\n"
   "    }\n"
   "    set key_to_info {\n"
   "        order last;\n"
   "        owner key_word;\n"
   "        member intersect;\n"
   "    }\n"
   "    set info_to_key {\n"
   "        order last;\n"
   "        owner info;\n"
   "        member intersect;\n"
   "    }\n"
   "    set loan_history {\n"
   "        order last;\n"
   "        owner system;\n"
   "        member borrower;\n"
   "    }\n"
   "}\n";