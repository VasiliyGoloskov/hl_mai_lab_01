INSERT INTO `User` (`first_name`,`last_name`,`email`,`login`,`password`)
VALUES
('Ivan', 'Smirnov', 'ivan_s@mail.ru', 'smriv', 'hardpassword'),
('Maria', 'Ivanova', 'maria_i@mail.ru', 'ivanova', 'strongpass'),
('Sergey', 'Fedorov', 'sergey_f@mail.ru', 'fedorov', 'qwerty123'),
('Olga', 'Semenova', 'olga_s@mail.ru', 'semenova', 'letmein'),
('Nikolay', 'Lebedev', 'nikolay_l@mail.ru', 'lebedev', 'welcome123'),
('Anastasia', 'Egorova', 'anastasia_e@mail.ru', 'egorova', 'mypassword'),
('Igor', 'Pavlov', 'igor_p@mail.ru', 'pavlov', 'myp@ssw0rd'),
('Margarita', 'Kuznetsova', 'margarita_k@mail.ru', 'kuznetsova', 'secure123'),
('Artem', 'Andreev', 'artem_a@mail.ru', 'andreev', 'pass123'),
('Elena', 'Sorokina', 'elena_s@mail.ru', 'sorokina', '12345'),
('Vladimir', 'Ilyin', 'vladimir_i@mail.ru', 'ilin', 'ilovecoding'),
('Igor', 'Pavlenko', 'igor_pav@mail.ru', 'pavlenko', 'myp@ssssw0rd'),
('Daria', 'Smirnova', 'daria_s@mail.ru', 'smirnov', 'password123'),
('Andrey', 'Mikhaylov', 'andrey_m@mail.ru', 'mikhaylov', 'pass1234'),
('Svetlana', 'Fedorova', 'svetlana_f@mail.ru', 'fedorova', 'qwerty123'),
('Ivan', 'Semenov', 'ivan_s@mail.ru', 'semenov', 'letmein');


INSERT INTO Route (`host_id`,`title`,`type`,`creation_date`,`start_point`,`finish_point`)
VALUES
(1, 'Walk on Red Square', 'pedestrian', '2023-09-15', 'Moscow', 'Red Square'),
(2, 'Bike Ride to Tsarskoye Selo', 'bicycle', '2023-09-16', 'St. Petersburg', 'Tsarskoye Selo'),
(3, 'Travel around Kyiv', 'car', '2023-09-17', 'Kyiv', 'Podol'),
(4, 'Sunset Meetup in Minsk', 'pedestrian', '2023-09-18', 'Minsk', 'Embankment'),
(5, 'Excursion in Novosibirsk', 'car', '2023-09-19', 'Novosibirsk', 'Central District'),
(6, 'Stroll around Yekaterinburg', 'pedestrian', '2023-09-20', 'Yekaterinburg', 'City Center'),
(6, 'Kayaking in Krasnoyarsk', 'water', '2023-09-21', 'Krasnoyarsk', 'Yenisei River'),
(7, 'Water Adventures in Vladivostok', 'water', '2023-09-22', 'Vladivostok', 'Golden Horn'),
(7, 'Walk in Rostov-on-Don', 'pedestrian', '2023-09-23', 'Rostov-on-Don', 'Embankment'),
(7, 'Bike Ride in Odessa', 'bicycle', '2023-09-24', 'Odessa', 'Arcadia Beach');

INSERT INTO Trip (host_id, route_id, name, type, trip_date)
VALUES
(1, 1, 'Journey into History', 'long', '2023-09-18'),
(2, 2, 'Bike Trip to Suburbs', 'short', '2023-09-19'),
(3, 3, 'City Tour', 'medium', '2023-09-20'),
(4, 4, 'Sunset Meetup', 'long', '2023-09-21'),
(5, 5, 'City Excursion', 'medium', '2023-09-22'),
(6, 6, 'Through Old Streets', 'long', '2023-09-23'),
(6, 7, 'Kayaking Adventure', 'long', '2023-09-24'),
(7, 8, 'Water Adventures', 'long', '2023-09-25'),
(7, 9, 'Sunrise Meetup', 'long', '2023-09-26'),
(7, 10, 'Seaside Bike Ride', 'medium', '2023-09-27');