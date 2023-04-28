database ia_db1 inmemory persistent {
  data file "ia.d01" contains unit;
  data file "ia.d02" contains measurement;
  key file "ia.k01" contains unit.name;
  
  record unit {
    key char name[32];
    char unitno[8];
    char phone[16];
    char fax[16];
    char email[32];
  }
  
  record measurement {
    int32_t tm;
    int32_t kwh;
  }
  
  set unit_measurement {
    order last;
    owner unit;
    member measurement;
  }
}
