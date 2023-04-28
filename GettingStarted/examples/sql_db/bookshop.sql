drop database bookshop;
create database bookshop;

create table acctmgr(
    mgrid       char(7) primary key,
    name        char(24),
    hire_date   date,
    commission  double
);

create table patron(
    patid       char(3) primary key,
    name        char(30),
    street      char(35),
    city        char(17),
    state       char(2),
    country     char(2),
    pc          char(10),
    email       char(63),
    phone       char(15),
    mgrid       char(7) references acctmgr on update cascade
);

create table author(
    last_name   char(13) primary key,
    full_name   char(35),    
    gender      char distinct values = 2,
    yr_born     smallint,
    yr_died     smallint,
    short_bio   long varchar,
    key yob_gender_key(yr_born, gender)
);

create table genres(
    text       char(31) primary key
);

create table subjects(
    text       char(51) primary key
);

create table book(
    bookid      char(14) primary key,
    last_name   char(13) 
        references author on delete cascade on update cascade,
    title       varchar(255) key[16],
    descr       char(61),
    publisher   varchar(136),
    publ_year   smallint range 1500 to 1980 key,
    lc_class    char(33),
    date_acqd   date,
    date_sold   date,
    price       double,
    cost        double
);

create table related_name(
    bookid      char(14) 
        references book on delete cascade on update cascade,
    name        char(61)
);
    
create table genres_books(
    bookid      char(14) 
        references book on delete cascade on update cascade,
    genre       char(31) 
        references genres
);

create table subjects_books(
    bookid      char(14) 
        references book on delete cascade on update cascade,
    subject     char(51) 
        references subjects
);

create table note(
    noteid      integer primary key,
    bookid      char(14) 
        references book on delete cascade on update cascade,
    patid       char(3) 
        references patron on delete cascade on update cascade
);

create table note_line(
    noteid      integer 
        references note on delete cascade on update cascade,
    text        char(61)
);

create table sale(
    bookid      char(14) 
        references book on delete cascade on update cascade,
    patid       char(3) 
        references patron on delete cascade on update cascade
);

create table auction(
    aucid       integer primary key,
    bookid      char(14) 
        references book on delete cascade on update cascade,
    mgrid       char(7) 
        references acctmgr on update cascade,
    start_date  date,
    end_date    date,
    reserve     double,
    curr_bid    double,
    bid_time    time
);

create table bid(
    aucid       integer 
        references auction on delete cascade on update cascade,
    patid       char(3) 
        references patron on delete cascade on update cascade,
    offer       double,
    bid_ts      timestamp
);
