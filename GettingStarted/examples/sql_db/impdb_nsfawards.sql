OPEN nsfawards EXCLUSIVE;
INITIALIZE nsfawards;
.m
.m **** Importing data into the NSFAWARDS database
.m **** This is a large database so this may take a while
.m
.T start
.m Importing data into person table...
import into person from file "person.txt";
commit;
.m Importing data into sponsor table...
import into sponsor from file "sponsor.txt";
commit;
.m Importing data into nsforg table...
import into nsforg from file "nsforg.txt";
commit;
.m Importing data into nsfprog table...
import into nsfprog from file "nsfprog.txt";
commit;
.m Importing data into nsfapp table...
import into nsfapp from file "nsfapp.txt";
commit;
.m Importing data into award table...
import into award from file "award.txt";
commit;
.m Importing data into investigator table...
import into investigator from file "investgr.txt";
commit;
.m Importing data into field_apps table...
import into field_apps from file "fldapps.txt";
commit;
.m Importing data into progrefs table...
import into progrefs from file "progrefs.txt";
commit;
.m
.m **** NSFAWARDS database import complete
.m
.T stop
