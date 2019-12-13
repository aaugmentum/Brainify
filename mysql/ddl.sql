drop table answer;
drop table question;
drop table user;
drop table game;

create table game(
  game_id varchar(8),
  title varchar(20) not null,
  primary key (game_id)
);

create table user(
  ID varchar(5),
  game_id varchar(8),
  name varchar(20) not null,
  password varchar(20) not null,
  primary key (ID, game_id),
  foreign key (game_id) references game(game_id)
);

create table question(
  question_id varchar(10),
  game_id varchar(8),
  question_text varchar(160) not null,
  primary key (question_id),
  foreign key (game_id) references game(game_id)
    on delete cascade
);

create table answer(
  answer_id varchar(12),
  question_id varchar(10),
  is_correct boolean not null,
  answer_text varchar(60) not null,
  primary key (answer_id),
  foreign key (question_id) references question(question_id)
    on delete cascade
);