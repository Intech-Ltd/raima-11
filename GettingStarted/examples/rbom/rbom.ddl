database rbom {
    data file "rbom.d01" contains ritem;
    data file "rbom.d02" contains rbill;
    key file  "rbom.k01" contains rid_code;
    key file  "rbom.k02" contains rbom;

    record ritem {
        unique key char rid_code[16];
        char    rdescription[58];
        double  rcost;
        int32_t rcomponent_count;
    }
    record rbill {
        char    rparent[16];
        char    rcomponent[16];
        int32_t rsequence;
        double  rquantity;
        int32_t rlevel;

        compound key rbom {
            rparent;
            rsequence;
        }
    }
}
