drop table if exists answer;
drop table if exists question;
drop table if exists user;
drop table if exists game;

create table user(
  user_name varchar(20),
  password varchar(20) not null,
  primary key (user_name)
);

create table game(
  game_id varchar(8),
  user_name varchar(20),
  title varchar(20) not null,
  primary key (game_id),
  foreign key (user_name) references user(user_name)
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