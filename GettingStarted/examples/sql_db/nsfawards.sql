drop database nsfawards;
create database nsfawards;

create table person(
    name        char(35) primary key,
    gender      char(1),
    jobclass    char(1)
);

create table sponsor(
    name        char(50) primary key,
    addr        char(40),
    city        char(20),
    state       char(2),
    zip         char(5),
    key geo_loc(state, city)
);

create table nsforg(
    orgid       char(3) primary key,
    name        char(40)
);

create table nsfprog(
    progid      char(4) primary key,
    descr       char(40)
);

create table nsfapp(
    appid       char(10) primary key,
    descr       char(40)
);

create table award(
    awardno     integer primary key,
    title       char(200),
    award_date  date key,
    instr       char(3),
    start_date  date,
    exp_date    date key,
    amount      double key,
    prgm_mgr    char(35) references person,
    sponsor_nm  char(50) references sponsor,
    orgid       char(3)  references nsforg
);

create table investigator(
    awardno     integer  references award,
    name        char(35) references person
);

create table field_apps(
    awardno     integer  references award,
    appid       char(10) references nsfapp
);   

create table progrefs(
    awardno     integer  references award,
    progid      char(4)  references nsfprog
);
