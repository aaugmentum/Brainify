delete from answer;
delete from question;
delete from game;
delete from user;

insert into user values ('Davron', '12345');
insert into user values ('Aleksey', '12345');
insert into user values ('Azamat', '12345');
insert into user values ('Kamron', '12345');

-- insert into game values ('201', 'Davron', 'English vocabulary');
-- insert into game values ('202', 'Azamat', 'Java documentation');
-- insert into game values ('203', 'Kamron', 'Alphabet');
insert into game values ('204', 'Davron', 'AE');
insert into game values ('205', 'Kamron', 'OOP');
insert into game values ('206', 'Aleksey', 'D');
insert into game values ('207', 'Kamron', 'CA');
insert into game values ('208', 'Azamat', 'OS');
insert into game values ('209', 'Davron', 'SA');

-- insert into question values ('301', '201', 'Roses are red, violets are ...?');
-- insert into question values ('302', '201', 'Meaning of word sanctum');
-- insert into question values ('303', '202', 'Java is not ...?');
-- insert into question values ('304', '202', 'JVM is ...?');
-- insert into question values ('305', '203', 'First letter of alphabet');
-- insert into question values ('306', '203', 'Last letter of alphabet');
-- insert into question values ('307', '203', 'The letter which has two v');
insert into question values ('308', '204', 'Roses are red, violets are ...?');
insert into question values ('309', '205', 'Encapsulation is ...?');
insert into question values ('310', '206', 'What is SQL?');
insert into question values ('311', '206', 'What are 2 types of DML?');
insert into question values ('312', '207', 'What is ADT?');
insert into question values ('313', '207', 'What is big Oh of Merge Sort?');
insert into question values ('314', '208', 'What is POSIX?');
insert into question values ('315', '208', 'What is time zero for Unix?');
insert into question values ('316', '208', 'What function initializes the segmentation registers?');
insert into question values ('317', '208', 'Address of bootloader?');
insert into question values ('318', '208', 'What is the default size of a block?');
insert into question values ('319', '208', 'Data structure of file or a directory in UNIX.');
insert into question values ('320', '208', 'Using which command we can find inode number of the file?');
insert into question values ('321', '208', 'How many types of files recognized by Ext2?');
insert into question values ('322', '208', 'Part of file with null chars, not stored in any data block.');
insert into question values ('323', '209', 'Choose what is not part of SDLC');

-- insert into answer values ('401', '301', '0', 'red');
-- insert into answer values ('402', '301', '0', 'green');
-- insert into answer values ('403', '301', '1', 'blue');
-- insert into answer values ('404', '301', '0', 'purple');

-- insert into answer values ('405', '302', '1', 'Sacred place');
-- insert into answer values ('406', '302', '0', 'Washroom');
-- insert into answer values ('407', '302', '0', 'Human organ');
-- insert into answer values ('408', '302', '0', 'Place in Stockholm');

-- insert into answer values ('409', '303', '0', 'Programming language');
-- insert into answer values ('410', '303', '0', 'Robust');
-- insert into answer values ('411', '303', '0', 'Cross-platform');
-- insert into answer values ('412', '303', '1', 'Slow');

-- insert into answer values ('413', '304', '1', 'Java Virtual Machine');
-- insert into answer values ('414', '304', '0', 'Johnsons Virtual Multiplexer');
-- insert into answer values ('415', '304', '0', 'Johannesburg Vital Medicine');
-- insert into answer values ('416', '304', '0', 'Jumpy frog Virtual Machine');

-- insert into answer values ('417', '305', '0', 'B');
-- insert into answer values ('418', '305', '0', 'Z');
-- insert into answer values ('419', '305', '1', 'A');
-- insert into answer values ('420', '305', '0', 'O');

-- insert into answer values ('421', '306', '1', 'Z');
-- insert into answer values ('422', '306', '0', 'A');
-- insert into answer values ('423', '306', '0', 'F');
-- insert into answer values ('424', '306', '0', 'X');

-- insert into answer values ('425', '307', '0', 'Y');
-- insert into answer values ('426', '307', '1', 'W');
-- insert into answer values ('427', '307', '0', 'H');
-- insert into answer values ('428', '307', '0', 'Z');
-- ! AE
insert into answer values ('429', '308', '0', 'red');
insert into answer values ('430', '308', '0', 'green');
insert into answer values ('431', '308', '1', 'blue');
insert into answer values ('432', '308', '0', 'purple');
-- ! OOP
insert into answer values ('433', '309', '0', 'used to tranfer data');
insert into answer values ('434', '309', '1', 'used to hide data');
insert into answer values ('435', '309', '0', 'keyword in C language');
insert into answer values ('436', '309', '0', 'ability to process object differently');
-- ! D
insert into answer values ('437', '310', '0', 'Sequential Query List');
insert into answer values ('438', '310', '0', 'Sequential Queue Language');
insert into answer values ('439', '310', '0', 'Structured Queue List');
insert into answer values ('440', '310', '1', 'Structured Query Language');

insert into answer values ('441', '311', '0', 'Logic, Generic');
insert into answer values ('442', '311', '0', 'Flow-based, Reactive');
insert into answer values ('443', '311', '1', 'Procedural, Declarative');
insert into answer values ('444', '311', '0', 'Functional, Concurrent');
-- ! CA
insert into answer values ('445', '312', '0', 'Abstract Diagram Type');
insert into answer values ('446', '312', '1', 'Abstract Data Type');
insert into answer values ('447', '312', '0', 'Abstract Dataflow Topology');
insert into answer values ('448', '312', '0', 'Arbitration of Data Technology');

insert into answer values ('449', '313', '1', 'O(nlogn)');
insert into answer values ('450', '313', '0', 'O(n)');
insert into answer values ('451', '313', '0', 'O(logn)');
insert into answer values ('452', '313', '0', 'O(n!)');
-- ! OS
insert into answer values ('453', '314', '0', 'Package of Operating System I/O');
insert into answer values ('454', '314', '1', 'Portable Operating System Interface');
insert into answer values ('455', '314', '0', 'Plan Of Semaphore Implementation');
insert into answer values ('456', '314', '0', 'Portion Of Sequential Information');

insert into answer values ('457', '315', '0', '00:00:00 Hours, Dec 1, 1980');
insert into answer values ('458', '315', '0', '00:00:00 Hours, Jan 1, 1975');
insert into answer values ('459', '315', '0', '00:00:00 Hours, Dec 1, 1970');
insert into answer values ('460', '315', '1', '00:00:00 Hours, Jan 1, 1970');

insert into answer values ('461', '316', '1', 'first startup_32');
insert into answer values ('462', '316', '0', 'second startup_32');
insert into answer values ('463', '316', '0', 'setup_idt()');
insert into answer values ('464', '316', '0', 'start_kernel()');

insert into answer values ('465', '317', '0', '0x0009a000');
insert into answer values ('466', '317', '1', '0x00007c00');
insert into answer values ('467', '317', '0', '0x0009b000');
insert into answer values ('468', '317', '0', '0x00010000');

insert into answer values ('469', '318', '0', '1 KB');
insert into answer values ('470', '318', '1', '4 KB');
insert into answer values ('471', '318', '0', '1024 KB');
insert into answer values ('472', '318', '0', '128 KB');

insert into answer values ('473', '319', '0', 'Block');
insert into answer values ('474', '319', '0', 'Descriptor');
insert into answer values ('475', '319', '1', 'Inode');
insert into answer values ('476', '319', '0', 'Hardlink');

insert into answer values ('477', '320', '1', 'ls -i');
insert into answer values ('478', '320', '0', 'ls -inode');
insert into answer values ('479', '320', '0', 'i|more');
insert into answer values ('480', '320', '0', 'cat -i');

insert into answer values ('481', '321', '0', '4');
insert into answer values ('482', '321', '0', '8');
insert into answer values ('483', '321', '0', '3');
insert into answer values ('484', '321', '1', '6');

insert into answer values ('485', '322', '1', 'File hole');
insert into answer values ('486', '322', '0', 'Missed file');
insert into answer values ('487', '322', '0', 'Null block');
insert into answer values ('488', '322', '0', 'Null file');
-- ! SA
insert into answer values ('489', '323', '0', 'Planning');
insert into answer values ('490', '323', '0', 'Analysis');
insert into answer values ('491', '323', '0', 'Design');
insert into answer values ('492', '323', '1', 'Instantiating');