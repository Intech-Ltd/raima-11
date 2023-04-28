static const char *const tims_imp=
   "database tims;\n"
   "\n"
   "foreach \"author.txt\" {\n"
   "    record author {\n"
   "        create on 1;\n"
   "        field name = 2;\n"
   "    }\n"
   "    connect author_list;\n"
   "}\n"
   "foreach \"info.txt\" {\n"
   "    record info {\n"
   "        create on 1;\n"
   "        field id_code = 4;\n"
   "        field info_title = 5;\n"
   "        field publisher = 6;\n"
   "        field pub_date = 7;\n"
   "        field info_type = 8;\n"
   "    }\n"
   "    record author {\n"
   "        find on 2;\n"
   "    }\n"
   "    connect has_published;\n"
   "}\n"
   "foreach \"text.txt\" {\n"
   "    record infotext {\n"
   "        field line = 2;\n"
   "    }\n"
   "    record info {\n"
   "        find on 1;\n"
   "    }\n"
   "    connect abstract;\n"
   "}\n"
   "foreach \"key_word.txt\" {\n"
        "    record key_word {\n"
   "        create on 1;\n"
   "        field kword = 2;\n"
   "    }\n"
   "}\n"
   "foreach \"intersec.txt\" {\n"
   "    record intersect {\n"
   "        field int_type = 3;\n"
   "    }\n"
   "    record info {\n"
   "        find on 2;\n"
   "    }\n"
   "    record key_word {\n"
   "        find on 1;\n"
        "    }\n"
   "    connect info_to_key;\n"
   "    connect key_to_info;\n"
   "}\n"
   "end;\n";
