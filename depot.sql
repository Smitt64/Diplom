/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50151
Source Host           : localhost:3306
Source Database       : depot

Target Server Type    : MYSQL
Target Server Version : 50151
File Encoding         : 65001

Date: 2011-05-03 10:22:13
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `groups`
-- ----------------------------
DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `special` int(11) NOT NULL COMMENT 'На базе группы',
  `caption` varchar(20) CHARACTER SET utf8 NOT NULL COMMENT 'Название',
  `intake` date NOT NULL COMMENT 'Дата создания',
  `cource` smallint(6) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `special` (`special`),
  CONSTRAINT `groups_ibfk_1` FOREIGN KEY (`special`) REFERENCES `specialty` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=latin1 COLLATE=latin1_bin CHECKSUM=1;

-- ----------------------------
-- Records of groups
-- ----------------------------
INSERT INTO `groups` VALUES ('1', '1', '46 про 07', '2007-09-01', '4');
INSERT INTO `groups` VALUES ('2', '2', '48 бух 07', '2007-09-01', '4');
INSERT INTO `groups` VALUES ('3', '3', '41 нес 07', '2007-09-01', '4');
INSERT INTO `groups` VALUES ('4', '3', '44 дс 07', '2007-09-01', '4');
INSERT INTO `groups` VALUES ('5', '1', '45 про 07', '2007-09-01', '4');
INSERT INTO `groups` VALUES ('6', '3', '31 нэс 08', '2008-09-01', '3');
INSERT INTO `groups` VALUES ('7', '3', '34 дс 08', '2008-09-01', '3');
INSERT INTO `groups` VALUES ('8', '1', '35 про 08', '2008-09-01', '3');
INSERT INTO `groups` VALUES ('9', '1', '36 про 08', '2008-09-01', '3');
INSERT INTO `groups` VALUES ('10', '1', '37 про 08', '2008-09-01', '3');
INSERT INTO `groups` VALUES ('11', '3', '20 нэс 09', '2009-09-01', '2');
INSERT INTO `groups` VALUES ('12', '3', '21 нэс 09', '2009-09-01', '2');
INSERT INTO `groups` VALUES ('13', '3', '24 дс 09', '2009-09-01', '2');
INSERT INTO `groups` VALUES ('14', '1', '25 про 09', '2009-09-01', '2');
INSERT INTO `groups` VALUES ('15', '1', '26 про 09', '2009-09-01', '2');

-- ----------------------------
-- Table structure for `specialty`
-- ----------------------------
DROP TABLE IF EXISTS `specialty`;
CREATE TABLE `specialty` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `kod` int(11) NOT NULL COMMENT 'Код специальности',
  `caption` varchar(100) NOT NULL COMMENT 'Название специальности',
  `during` int(11) NOT NULL COMMENT 'Время обучения, лет',
  `prefix` varchar(5) NOT NULL,
  `num` int(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of specialty
-- ----------------------------
INSERT INTO `specialty` VALUES ('1', '220301', 'Программное обеспечение вычислительной техники и автоматизированных систем', '4', 'ПРО', '6');
INSERT INTO `specialty` VALUES ('2', '2204', 'Менеджмент и бухгалтерский учет', '4', 'БУХ', '8');
INSERT INTO `specialty` VALUES ('3', '15101', 'Технология машиностроения', '4', 'НЭС', '1');
INSERT INTO `specialty` VALUES ('4', '220301', 'Автоматизация те6хнологических процессов и производств', '4', 'АТП', '2');

-- ----------------------------
-- Table structure for `student`
-- ----------------------------
DROP TABLE IF EXISTS `student`;
CREATE TABLE `student` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) CHARACTER SET utf8 NOT NULL COMMENT 'Имя студента',
  `surname` varchar(20) CHARACTER SET utf8 NOT NULL COMMENT 'Фамилия студента',
  `patronymic` varchar(20) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT 'Отчество студента',
  `group` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `student_ibfk_1` (`group`),
  CONSTRAINT `student_ibfk_1` FOREIGN KEY (`group`) REFERENCES `groups` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=433 DEFAULT CHARSET=latin1 COLLATE=latin1_bin;

-- ----------------------------
-- Records of student
-- ----------------------------
INSERT INTO `student` VALUES ('1', 'Никита', 'Серпков', 'Игоревич', '1');
INSERT INTO `student` VALUES ('2', 'Надежда', 'Рева', '', '1');
INSERT INTO `student` VALUES ('3', 'Роман', 'Серенков', '', '1');
INSERT INTO `student` VALUES ('4', 'Эдуард', 'Гришин', '', '1');
INSERT INTO `student` VALUES ('5', 'Павел', 'Голиков', '', '1');
INSERT INTO `student` VALUES ('6', 'Наталья', 'Серегина', '', '1');
INSERT INTO `student` VALUES ('7', 'Сергей', 'Тимощенко', '', '1');
INSERT INTO `student` VALUES ('8', 'Тимур', 'Хакимов', '', '1');
INSERT INTO `student` VALUES ('9', 'Влад', 'Белюскин', '', '1');
INSERT INTO `student` VALUES ('10', 'Анатолий', 'Рыбальченко', '', '1');
INSERT INTO `student` VALUES ('11', 'Вадим', 'Бауткин', '', '1');
INSERT INTO `student` VALUES ('12', 'Виктор', 'Свиридов', '', '1');
INSERT INTO `student` VALUES ('13', 'Павел', 'Кузмин', '', '1');
INSERT INTO `student` VALUES ('14', 'Александр', 'Клецкин', '', '1');
INSERT INTO `student` VALUES ('15', 'Александр', 'Неустроев', '', '1');
INSERT INTO `student` VALUES ('16', 'Екатерина', 'Шмычкова', '', '1');
INSERT INTO `student` VALUES ('17', 'Михаил', 'Бруев', '', '1');
INSERT INTO `student` VALUES ('18', 'Екатерина', 'Дзюба', '', '1');
INSERT INTO `student` VALUES ('19', 'Сергей', 'Субботкин', '', '1');
INSERT INTO `student` VALUES ('20', 'Ирина', 'Романенко', '', '1');
INSERT INTO `student` VALUES ('21', 'Вадим', 'Захаров', '', '1');
INSERT INTO `student` VALUES ('22', 'Екатерина', 'Дерюго', '', '1');
INSERT INTO `student` VALUES ('23', 'Илья', 'Малючков', '', '1');
INSERT INTO `student` VALUES ('24', 'Сергей', 'Родиков', '', '1');
INSERT INTO `student` VALUES ('25', 'Андрей', 'Пигарев', '', '1');
INSERT INTO `student` VALUES ('26', 'Надежда', 'Пец', '', '1');
INSERT INTO `student` VALUES ('27', 'Сергей', 'Махоткин', '', '1');
INSERT INTO `student` VALUES ('28', 'О', 'Обрамова', 'Н', '2');
INSERT INTO `student` VALUES ('29', 'М', 'Олышова', 'В', '2');
INSERT INTO `student` VALUES ('30', 'А', 'Аненкова', 'А', '2');
INSERT INTO `student` VALUES ('31', 'Т', 'Бармашова', 'В', '2');
INSERT INTO `student` VALUES ('32', 'М', 'Бондик', 'В', '2');
INSERT INTO `student` VALUES ('33', 'Д', 'Бытина', 'П', '2');
INSERT INTO `student` VALUES ('34', 'Ю', 'Правдина', 'Ф', '2');
INSERT INTO `student` VALUES ('35', 'Е', 'Дашунина', 'А', '2');
INSERT INTO `student` VALUES ('36', 'А', 'Денисова', 'С', '2');
INSERT INTO `student` VALUES ('37', 'Н', 'Ивлева', 'В', '2');
INSERT INTO `student` VALUES ('38', 'И', 'Кизенок', 'В', '2');
INSERT INTO `student` VALUES ('39', 'О', 'Кизенок', 'В', '2');
INSERT INTO `student` VALUES ('40', 'А', 'Коробанова', 'А', '2');
INSERT INTO `student` VALUES ('41', 'О', 'Кузнецова', 'Ю', '2');
INSERT INTO `student` VALUES ('42', 'Л', 'Кутареева', 'Ю', '2');
INSERT INTO `student` VALUES ('43', 'Н', 'Липкина', 'С', '2');
INSERT INTO `student` VALUES ('44', 'И', 'Макланова', 'А', '2');
INSERT INTO `student` VALUES ('45', 'Е', 'Малахова', 'Ю', '2');
INSERT INTO `student` VALUES ('46', 'А', 'Марковская ', 'С', '2');
INSERT INTO `student` VALUES ('47', 'А', 'Мадуева', 'Ю', '2');
INSERT INTO `student` VALUES ('48', 'Д', 'Петухова', 'В', '2');
INSERT INTO `student` VALUES ('49', 'Ю', 'Прудникова', 'Н', '2');
INSERT INTO `student` VALUES ('50', 'В', 'Савенкова', 'Н', '2');
INSERT INTO `student` VALUES ('51', 'Я', 'Усова', 'С', '2');
INSERT INTO `student` VALUES ('52', 'Н', 'Фролова', 'Н', '2');
INSERT INTO `student` VALUES ('53', 'Я', 'Яринок', 'О', '2');
INSERT INTO `student` VALUES ('54', 'Иван', 'Алимкин', 'Николаевич', '3');
INSERT INTO `student` VALUES ('55', 'Илья', 'Анискин', 'Александрович', '3');
INSERT INTO `student` VALUES ('56', 'Кирилл', 'Афанасьев ', 'Андреевич', '3');
INSERT INTO `student` VALUES ('57', 'Евгений', 'Барышев', 'Александрович', '3');
INSERT INTO `student` VALUES ('58', 'Дмитрий', 'Горбачев', 'Александрович', '3');
INSERT INTO `student` VALUES ('59', 'Артем', 'Гутев', 'Михайлович', '3');
INSERT INTO `student` VALUES ('60', 'Александр', 'Дерябин', 'Сергеевич', '3');
INSERT INTO `student` VALUES ('61', 'Дмитрий', 'Ермаков', 'Михайлович', '3');
INSERT INTO `student` VALUES ('62', 'Владимир', 'Железняков', 'Николаевич', '3');
INSERT INTO `student` VALUES ('63', 'Вячеслав', 'Захарченко', 'Николаевич', '3');
INSERT INTO `student` VALUES ('64', 'Михаил', 'Истратов', 'Алексанлрович', '3');
INSERT INTO `student` VALUES ('65', 'Максим', 'Кабунин', 'Андреевич', '3');
INSERT INTO `student` VALUES ('66', 'Вячеслав', 'Кашубо', 'Александрович', '3');
INSERT INTO `student` VALUES ('67', 'Кирилл', 'Кильпиков', 'Владимирович', '3');
INSERT INTO `student` VALUES ('68', 'Павел', 'Колесник', 'Павлович', '3');
INSERT INTO `student` VALUES ('69', 'Юлия', 'Комарова', 'Алексеевна', '3');
INSERT INTO `student` VALUES ('70', 'Сергей', 'Котиков', 'Олегович', '3');
INSERT INTO `student` VALUES ('71', 'Илья', 'Красавин', 'Владимирович', '3');
INSERT INTO `student` VALUES ('72', 'Дмитрий', 'Крюков', 'Михайлович', '3');
INSERT INTO `student` VALUES ('73', 'Евгений', 'Кузнецов', 'Геннадьевич', '3');
INSERT INTO `student` VALUES ('74', 'Дмитрий', 'Листратов', 'Сергеевич', '3');
INSERT INTO `student` VALUES ('75', 'Евгений', 'Макаров', 'Сергеевич', '3');
INSERT INTO `student` VALUES ('76', 'Максим', 'Науменков', 'Алексеевич', '3');
INSERT INTO `student` VALUES ('77', 'Сергей', 'Никоноренков', 'Анатольевич', '3');
INSERT INTO `student` VALUES ('78', 'Роман', 'Романов', 'Вячеславович', '3');
INSERT INTO `student` VALUES ('79', 'Алексей', 'Самонин', 'Михайлович', '3');
INSERT INTO `student` VALUES ('80', 'Кирилл', 'Семин', 'Сергеевич', '3');
INSERT INTO `student` VALUES ('81', 'Евгений', 'Солдатов', 'Николаевич', '3');
INSERT INTO `student` VALUES ('82', 'Сергей', 'Тамилин', 'Михайлович', '3');
INSERT INTO `student` VALUES ('83', 'Ярослав', 'Усенков', 'Игоревич', '3');
INSERT INTO `student` VALUES ('84', 'Сергей', 'Цуриков', 'Владимирович', '3');
INSERT INTO `student` VALUES ('85', 'Кирилл', 'Анохин', 'Констаноинович', '4');
INSERT INTO `student` VALUES ('86', 'Алексей', 'Блажиевский', 'Юрьевич', '4');
INSERT INTO `student` VALUES ('87', 'Владимир', 'Бондаренко', 'Юрьевич', '4');
INSERT INTO `student` VALUES ('88', 'Вадим', 'Героцкий', 'Анатольевич', '4');
INSERT INTO `student` VALUES ('89', 'Маргарита', 'Горбачева', 'Юрьевна', '4');
INSERT INTO `student` VALUES ('90', 'Игорь', 'Дружинин', 'Евгеньевич', '4');
INSERT INTO `student` VALUES ('91', 'Николай', 'Егоров', 'Николаевич', '4');
INSERT INTO `student` VALUES ('92', 'Сергей', 'Ефимочкин', 'Дмитриевич', '4');
INSERT INTO `student` VALUES ('93', 'Игорь', 'Зеленый', 'Александрович', '4');
INSERT INTO `student` VALUES ('94', 'Сергей', 'Ищенко', 'Сергеевич', '4');
INSERT INTO `student` VALUES ('95', 'Денис', 'Красиков', 'Васильевич', '4');
INSERT INTO `student` VALUES ('96', 'Никита', 'Крохин', 'Игоревич', '4');
INSERT INTO `student` VALUES ('97', 'Юрий', 'Левкин', 'Владимирович', '4');
INSERT INTO `student` VALUES ('98', 'Дмитрий', 'Лобанов', 'Михайлович', '4');
INSERT INTO `student` VALUES ('99', 'Андрей', 'Лукьянов', 'Александрович', '4');
INSERT INTO `student` VALUES ('100', 'Андрей', 'Матвеев', 'Александрович', '4');
INSERT INTO `student` VALUES ('101', 'Алексей', 'Наумов', 'Михайлович', '4');
INSERT INTO `student` VALUES ('102', 'Антон', 'Ольховский', 'Дмитриевич', '4');
INSERT INTO `student` VALUES ('103', 'Иван', 'Павлютин', 'Алексеевич', '4');
INSERT INTO `student` VALUES ('104', 'Игорь', 'Рогулев', 'Олегович', '4');
INSERT INTO `student` VALUES ('105', 'Денис', 'Родин', 'Владимирович', '4');
INSERT INTO `student` VALUES ('106', 'Кирилл', 'Семенов', 'Владимирович', '4');
INSERT INTO `student` VALUES ('107', 'Константин', 'Степичев', 'Юрьевич', '4');
INSERT INTO `student` VALUES ('108', 'Павел', 'Угначев', 'Николаевич', '4');
INSERT INTO `student` VALUES ('109', 'Руслан', 'Фурсов', 'Юрьевич', '4');
INSERT INTO `student` VALUES ('110', 'Николай', 'Цыбульский', 'Николаевич', '4');
INSERT INTO `student` VALUES ('111', 'Руслан', 'Чепиков', 'Сергеевич', '4');
INSERT INTO `student` VALUES ('112', 'Павел', 'Черепок', 'Николаевич', '4');
INSERT INTO `student` VALUES ('113', 'Александр', 'Черкасов', 'Валерьевич', '4');
INSERT INTO `student` VALUES ('114', 'Александр', 'Шведов', 'Александрович', '4');
INSERT INTO `student` VALUES ('115', 'Максим', 'Богданов', 'Александрович', '5');
INSERT INTO `student` VALUES ('116', 'Виталий', 'Бычков', 'Витальевич', '5');
INSERT INTO `student` VALUES ('117', 'Максим', 'Гавриков', 'Юрьевич', '5');
INSERT INTO `student` VALUES ('118', 'Дмитрий', 'Горбачев', 'Андреевич', '5');
INSERT INTO `student` VALUES ('119', 'Андрей', 'Драк', 'Игоревич', '5');
INSERT INTO `student` VALUES ('120', 'Евгений', 'Зайцев', 'Дмитриевич', '5');
INSERT INTO `student` VALUES ('121', 'Сергей', 'Козловцев', 'Леонидович', '5');
INSERT INTO `student` VALUES ('122', 'Владислав', 'Кретов', 'Николаевич', '5');
INSERT INTO `student` VALUES ('123', 'Евгений', 'Кузнецов', 'Владимирович', '5');
INSERT INTO `student` VALUES ('124', 'Елена', 'Курченко', 'Андреевна', '5');
INSERT INTO `student` VALUES ('125', 'Антон', 'Левых', 'Николаевич', '5');
INSERT INTO `student` VALUES ('126', 'Дмитрий', 'Лучин', 'Александрович', '5');
INSERT INTO `student` VALUES ('127', 'Виктор', 'Матюшин', 'Владимирович', '5');
INSERT INTO `student` VALUES ('128', 'Александр', 'Митин', 'Сергеевич', '5');
INSERT INTO `student` VALUES ('129', 'Евгений', 'Мишечкин', 'Юрьевич', '5');
INSERT INTO `student` VALUES ('130', 'Александр', 'Надточей', 'Юрьевич', '5');
INSERT INTO `student` VALUES ('131', 'Сергей', 'Напреев', 'Андреевич', '5');
INSERT INTO `student` VALUES ('132', 'Александр', 'Новицкий', 'Васильевич', '5');
INSERT INTO `student` VALUES ('133', 'Александр', 'Орлов', 'Владимирович', '5');
INSERT INTO `student` VALUES ('134', 'Илья', 'Пасецкий', 'Андреевич', '5');
INSERT INTO `student` VALUES ('135', 'Сергей', 'Пестрецов', 'Алексеевич', '5');
INSERT INTO `student` VALUES ('136', 'Александр', 'Петров', 'Викторович', '5');
INSERT INTO `student` VALUES ('137', 'Евгений', 'Поляков', 'Витальевич', '5');
INSERT INTO `student` VALUES ('138', 'Алена', 'Редина', 'Сергеевна', '5');
INSERT INTO `student` VALUES ('139', 'Анастасия', 'Сазонова', 'Игоревна', '5');
INSERT INTO `student` VALUES ('140', 'Андрей', 'Скородумов', 'Анатольевич', '5');
INSERT INTO `student` VALUES ('141', 'Марина', 'Смирнова', 'Николаевна', '5');
INSERT INTO `student` VALUES ('142', 'Алексей', 'Сташина', 'Николаевич', '5');
INSERT INTO `student` VALUES ('143', 'Александр', 'Трифонов', 'Николаевич', '5');
INSERT INTO `student` VALUES ('144', 'Егор', 'Ходанов', 'Викторович', '5');
INSERT INTO `student` VALUES ('145', 'Валерий', 'Шатунов', 'Олегович', '5');
INSERT INTO `student` VALUES ('146', 'Андрей', 'Шедов', 'Алексеевич', '5');
INSERT INTO `student` VALUES ('147', 'Яна', 'Шкляр', 'Сергеевна', '5');
INSERT INTO `student` VALUES ('148', 'Дмитрий', 'Шмыгаль', 'Владимирович', '5');
INSERT INTO `student` VALUES ('149', 'Роман', 'Артюшков', 'Григорьевич', '6');
INSERT INTO `student` VALUES ('150', 'Вадим', 'Гарбузов', 'Игоревич', '6');
INSERT INTO `student` VALUES ('151', 'Андрей', 'Головко', 'Андреевич', '6');
INSERT INTO `student` VALUES ('152', 'Максим', 'Гриневич', 'Николаевич', '6');
INSERT INTO `student` VALUES ('153', 'Игорь', 'Довголюк', 'Сергеевич', '6');
INSERT INTO `student` VALUES ('154', 'Денис', 'Евланов', 'Александрович', '6');
INSERT INTO `student` VALUES ('155', 'Максим', 'Егорушкин', 'Юрьевич', '6');
INSERT INTO `student` VALUES ('156', 'Алексей', 'Зак', 'Борисович', '6');
INSERT INTO `student` VALUES ('157', 'Никита', 'Колонцов', 'Викторович', '6');
INSERT INTO `student` VALUES ('158', 'Игорь', 'Кондратьев', 'Викторович', '6');
INSERT INTO `student` VALUES ('159', 'Сергей', 'Кочергин', 'Анатольевич', '6');
INSERT INTO `student` VALUES ('160', 'Вячеслав', 'Кулешов', 'Олегович', '6');
INSERT INTO `student` VALUES ('161', 'Сергей', 'Лебедкин', 'Сергеевич', '6');
INSERT INTO `student` VALUES ('162', 'Артем', 'Левый', 'Юрьевич', '6');
INSERT INTO `student` VALUES ('163', 'Антон', 'Ляхов', 'Дмитриевич', '6');
INSERT INTO `student` VALUES ('164', 'Денис', 'Ляхов', 'Александрович', '6');
INSERT INTO `student` VALUES ('165', 'Антон', 'Маденов', 'Сергеевич', '6');
INSERT INTO `student` VALUES ('166', 'Андрей', 'Макаров', 'Алексеевич', '6');
INSERT INTO `student` VALUES ('167', 'Артем', 'Матюшкин', 'Александрович', '6');
INSERT INTO `student` VALUES ('168', 'Константин', 'Мызников', 'Владимирович', '6');
INSERT INTO `student` VALUES ('169', 'Алексей', 'Орлов', 'Владимирович', '6');
INSERT INTO `student` VALUES ('170', 'Константин', 'Осин', 'Иванович', '6');
INSERT INTO `student` VALUES ('171', 'Роман', 'Поленок', 'Петрович', '6');
INSERT INTO `student` VALUES ('172', 'Тимофей', 'Серегин', 'Викторович', '6');
INSERT INTO `student` VALUES ('173', 'Роман', 'Симонов', 'Александрович', '6');
INSERT INTO `student` VALUES ('174', 'Илья', 'Фатин', 'Викторович', '6');
INSERT INTO `student` VALUES ('175', 'Александр', 'Хомутов', 'Александрович', '6');
INSERT INTO `student` VALUES ('176', 'Станислав', 'Ященко', 'Юрьевич', '6');
INSERT INTO `student` VALUES ('177', 'Евгений', 'Аверкин', 'Викторович', '7');
INSERT INTO `student` VALUES ('178', 'Сергей', 'Азарченков', 'Владимирович', '7');
INSERT INTO `student` VALUES ('179', 'Никита', 'Белов', 'Сергеевич', '7');
INSERT INTO `student` VALUES ('180', 'Алексей', 'Волобуев', 'Олегович', '7');
INSERT INTO `student` VALUES ('181', 'Юрий', 'Голдобов', 'Анатольевич', '7');
INSERT INTO `student` VALUES ('182', 'Вадим', 'Громыко', 'Вячеславович', '7');
INSERT INTO `student` VALUES ('183', 'Сергей', 'Емельяненко', 'Александрович', '7');
INSERT INTO `student` VALUES ('184', 'Евгений', 'Иванушкин', 'Геннадьевич', '7');
INSERT INTO `student` VALUES ('185', 'Сергей', 'Ильюхин', 'Викторович', '7');
INSERT INTO `student` VALUES ('186', 'Илья', 'Исюк', 'Сергеевич', '7');
INSERT INTO `student` VALUES ('187', 'Андрей', 'Кротов', 'Андреевич', '7');
INSERT INTO `student` VALUES ('188', 'Сергей', 'Кулаков', 'Сергеевич', '7');
INSERT INTO `student` VALUES ('189', 'Олег', 'Лушенков', 'Сергеевич', '7');
INSERT INTO `student` VALUES ('190', 'Максим', 'Машков', 'Николаевич', '7');
INSERT INTO `student` VALUES ('191', 'Игорь', 'Минченко', 'Игоревич', '7');
INSERT INTO `student` VALUES ('192', 'Владимир', 'Мягков', 'Сергеевич', '7');
INSERT INTO `student` VALUES ('193', 'Филипп', 'Поляков', 'Александрович', '7');
INSERT INTO `student` VALUES ('194', 'Михаил', 'Рожнов', 'Юрьевич', '7');
INSERT INTO `student` VALUES ('195', 'Никита', 'Справцев', 'Александрович', '7');
INSERT INTO `student` VALUES ('196', 'Александр', 'Сусоев', 'Николаевич', '7');
INSERT INTO `student` VALUES ('197', 'Валерий', 'Титарев', 'Станиславович', '7');
INSERT INTO `student` VALUES ('198', 'Александр', 'Хохлов', 'Николаевич', '7');
INSERT INTO `student` VALUES ('199', 'Сергей', 'Чертков', 'Юрьевич', '7');
INSERT INTO `student` VALUES ('200', 'Андрей', 'Шепелев', 'Игоревич', '7');
INSERT INTO `student` VALUES ('201', 'Максим', 'Ширков', 'Сергеевич', '7');
INSERT INTO `student` VALUES ('202', 'Максим', 'Михальченко', 'Николаевич', '7');
INSERT INTO `student` VALUES ('203', 'Леонид', 'Морозов', 'Леонидович', '7');
INSERT INTO `student` VALUES ('204', 'Никита', 'Чепиков', 'Сергеевич', '7');
INSERT INTO `student` VALUES ('205', 'Сергей', 'Туракулов', 'Вячеславович', '7');
INSERT INTO `student` VALUES ('206', 'Екатерина', 'Аверина', 'Сергеевна', '8');
INSERT INTO `student` VALUES ('207', 'Виктор', 'Артюшенко', 'Николаевич', '8');
INSERT INTO `student` VALUES ('208', 'Сергей', 'Баранов', 'Владимирович', '8');
INSERT INTO `student` VALUES ('209', 'Александра', 'Бескровная', 'Михайловна', '8');
INSERT INTO `student` VALUES ('210', 'Андрей', 'Бондарь', 'Владимирович', '8');
INSERT INTO `student` VALUES ('211', 'Виктор', 'Гвоздиков', 'Николаевич', '8');
INSERT INTO `student` VALUES ('212', 'Иван', 'Гусев', 'Леонидович', '8');
INSERT INTO `student` VALUES ('213', 'Евгений', 'Дерюгин', 'Юрьевич', '8');
INSERT INTO `student` VALUES ('214', 'Виктор', 'Жуков', 'Вячеславович', '8');
INSERT INTO `student` VALUES ('215', 'Марина', 'Иванова', 'Владимировна', '8');
INSERT INTO `student` VALUES ('216', 'Елена', 'Камененко', 'Валерьевна', '8');
INSERT INTO `student` VALUES ('217', 'Евгений', 'Копачев', 'Владимирович', '8');
INSERT INTO `student` VALUES ('218', 'Дмитрий', 'Коссович', 'Дмитриевич', '8');
INSERT INTO `student` VALUES ('219', 'Алексей', 'Кудряшов', 'Олегович', '8');
INSERT INTO `student` VALUES ('220', 'Анна', 'Макарова', 'Владимировна', '8');
INSERT INTO `student` VALUES ('221', 'Дмитрий', 'Мачехин', 'Владимирович', '8');
INSERT INTO `student` VALUES ('222', 'Татьяна', 'Попченко', 'Петровна', '8');
INSERT INTO `student` VALUES ('223', 'Сергей', 'Семенов', 'Игоревич', '8');
INSERT INTO `student` VALUES ('224', 'Дарья', 'Соловьева', 'Юрьевна', '8');
INSERT INTO `student` VALUES ('225', 'Александр', 'Стефанков', 'Владимирович', '8');
INSERT INTO `student` VALUES ('226', 'Андрей', 'Таешников', 'Васильевич', '8');
INSERT INTO `student` VALUES ('227', 'Александр', 'Тозик', 'Николаевич', '8');
INSERT INTO `student` VALUES ('228', 'Павел', 'Трушин', 'Сергеевич', '8');
INSERT INTO `student` VALUES ('229', 'Денис', 'Федин', 'Геннадьевич', '8');
INSERT INTO `student` VALUES ('230', 'Андрей', 'Федичкин', 'Олегович', '8');
INSERT INTO `student` VALUES ('231', 'Ксения', 'Федоренко', 'Владимировна', '8');
INSERT INTO `student` VALUES ('232', 'Ольга', 'Харитонова', 'Геннадьевна', '8');
INSERT INTO `student` VALUES ('233', 'Мария', 'Цыганкова', 'Владимировна', '8');
INSERT INTO `student` VALUES ('234', 'Анна', 'Чуфистова', 'Владимировна', '8');
INSERT INTO `student` VALUES ('235', 'Денис', 'Васин', 'Александрович', '9');
INSERT INTO `student` VALUES ('236', 'Алексей', 'Васюков', 'Андреевич', '9');
INSERT INTO `student` VALUES ('237', 'Николай', 'Власов', 'Игоревич', '9');
INSERT INTO `student` VALUES ('238', 'Роман', 'Высоцкий', 'Геннадьевич', '9');
INSERT INTO `student` VALUES ('239', 'Григорий', 'Дарковский', 'Юрьевич', '9');
INSERT INTO `student` VALUES ('240', 'Игорь', 'Дуденков', 'Андреевич', '9');
INSERT INTO `student` VALUES ('241', 'Репсиме', 'Елиазян', 'Петросовна', '9');
INSERT INTO `student` VALUES ('242', 'Николай', 'Зимонин', 'Николаевич', '9');
INSERT INTO `student` VALUES ('243', 'Константин', 'Кузовов', 'Николаевич', '9');
INSERT INTO `student` VALUES ('244', 'Ярослав', 'Лукашин', 'Викторович', '9');
INSERT INTO `student` VALUES ('245', 'Ярослав', 'Молчанов', 'Валерьевич', '9');
INSERT INTO `student` VALUES ('246', 'Кирилл', 'Орлов', 'Александрович', '9');
INSERT INTO `student` VALUES ('247', 'Дмитрий', 'Передков', 'Алексеевич', '9');
INSERT INTO `student` VALUES ('248', 'Юлия ', 'Петракова', 'Васильевна', '9');
INSERT INTO `student` VALUES ('249', 'Никита', 'Петухов', 'Александрович', '9');
INSERT INTO `student` VALUES ('250', 'Евгений', 'Посконный', 'Юрьевич', '9');
INSERT INTO `student` VALUES ('251', 'Владимир', 'Рябинков', 'Владимирович', '9');
INSERT INTO `student` VALUES ('252', 'Денис', 'Сизый', 'Андреевич', '9');
INSERT INTO `student` VALUES ('253', 'Александр', 'Симутин', 'Юрьевич', '9');
INSERT INTO `student` VALUES ('254', 'Сергей', 'Скобляков', 'Александрович', '9');
INSERT INTO `student` VALUES ('255', 'Павел', 'Сычев ', 'Иванович', '9');
INSERT INTO `student` VALUES ('256', 'Татьяна', 'Терешкина', 'Сергеевна', '9');
INSERT INTO `student` VALUES ('257', 'Евгения', 'Трифанова', 'Евгеньевна', '9');
INSERT INTO `student` VALUES ('258', 'Александр', 'Шабунин', 'Александрович', '9');
INSERT INTO `student` VALUES ('259', 'Дмитрий', 'Шашков', 'Геннадьевич', '9');
INSERT INTO `student` VALUES ('260', 'Елена', 'Щёголева', 'Сергеевна', '9');
INSERT INTO `student` VALUES ('261', 'Виктор', 'Ятченко ', 'Юрьевич', '9');
INSERT INTO `student` VALUES ('262', 'Ольга', 'Абрамченкова', 'Юрьевна', '10');
INSERT INTO `student` VALUES ('263', 'Кирилл', 'Абушик', 'Викторович', '10');
INSERT INTO `student` VALUES ('264', 'Андрей', 'Аврамец', 'Александрович', '10');
INSERT INTO `student` VALUES ('265', 'Екатерина', 'Александрова', 'Игоревна', '10');
INSERT INTO `student` VALUES ('266', 'Яков ', 'Балабко', 'Алексеевич', '10');
INSERT INTO `student` VALUES ('267', 'Виктория', 'Власовец', 'Сергеевна', '10');
INSERT INTO `student` VALUES ('268', 'Павел', 'Воронин', 'Афанасьевич', '10');
INSERT INTO `student` VALUES ('269', 'Елена', 'Гречихина', 'Константиновна', '10');
INSERT INTO `student` VALUES ('270', 'Людмила', 'Зубарева', 'Владимировна', '10');
INSERT INTO `student` VALUES ('271', 'Надежда', 'Кулешова', 'Андреевна', '10');
INSERT INTO `student` VALUES ('272', 'Анастасия', 'Латышева ', 'Анатольевна', '10');
INSERT INTO `student` VALUES ('273', 'Павел', 'Левчук', 'Александрович', '10');
INSERT INTO `student` VALUES ('274', 'Дарья', 'Львова', 'Сергеевна', '10');
INSERT INTO `student` VALUES ('275', 'Айхан', 'Мирзоев', 'Рухлан Оглы', '10');
INSERT INTO `student` VALUES ('276', 'Ольга', 'Мосина', 'Андреевна', '10');
INSERT INTO `student` VALUES ('277', 'Евгений', 'Носиков', 'Александрович', '10');
INSERT INTO `student` VALUES ('278', 'Юлия', 'Палачева', 'Дмитриевна', '10');
INSERT INTO `student` VALUES ('279', 'Владимир', 'Пересыпкин', 'Владимирович', '10');
INSERT INTO `student` VALUES ('280', 'Алена', 'Полякова', 'Игоревна', '10');
INSERT INTO `student` VALUES ('281', 'Алиса', 'Привалова', 'Юрьевна', '10');
INSERT INTO `student` VALUES ('282', 'Александр', 'Прокашев', 'Александрович', '10');
INSERT INTO `student` VALUES ('283', 'Анастасия', 'Пупова', 'Олеговна', '10');
INSERT INTO `student` VALUES ('284', 'Александр', 'Решетнев', 'Сергеевич', '10');
INSERT INTO `student` VALUES ('285', 'Семен', 'Сехин', 'Александрович', '10');
INSERT INTO `student` VALUES ('286', 'Валерий', 'Черкасов', 'Михайлович', '10');
INSERT INTO `student` VALUES ('287', 'Виталий', 'Юрченко', 'Викторович', '10');
INSERT INTO `student` VALUES ('288', 'Сергей ', 'Алфимов', 'Вячеславович', '11');
INSERT INTO `student` VALUES ('289', 'Алексей', 'Афанасенков', 'Геннадьевич', '11');
INSERT INTO `student` VALUES ('290', 'Дмитрий', 'Богадеев', 'Юрьевич', '11');
INSERT INTO `student` VALUES ('291', 'Герман', 'Гришин', 'Андреевич', '11');
INSERT INTO `student` VALUES ('292', 'Евгений', 'Джем', 'Николаевич', '11');
INSERT INTO `student` VALUES ('293', 'Никита', 'Зеркаленков', 'Михайлович', '11');
INSERT INTO `student` VALUES ('294', 'Кирилл', 'Иванов', 'Андреевич', '11');
INSERT INTO `student` VALUES ('295', 'Дмитрий', 'Ильюшин', 'Евгеньевич', '11');
INSERT INTO `student` VALUES ('296', 'Александр', 'Калашников', 'Васильевич', '11');
INSERT INTO `student` VALUES ('297', 'Сергей ', 'Коноплев', 'Павлович', '11');
INSERT INTO `student` VALUES ('298', 'Виктория', 'Кравчик ', 'Дмитриевна', '11');
INSERT INTO `student` VALUES ('299', 'Дмитрий', 'Кротов', 'Игоревич', '11');
INSERT INTO `student` VALUES ('300', 'Дмитрий', 'Кузовов', 'Игоревич', '11');
INSERT INTO `student` VALUES ('301', 'Владимир', 'Милехин', 'Сергеевич', '11');
INSERT INTO `student` VALUES ('302', 'Владислав', 'Мишин', 'Романович', '11');
INSERT INTO `student` VALUES ('303', 'Владислав', 'Никаноров', 'Сергеевич', '11');
INSERT INTO `student` VALUES ('304', 'Алексей', 'Подковко', 'Александрович', '11');
INSERT INTO `student` VALUES ('305', 'Екатерина', 'Полижаевская', 'Сергеевна', '11');
INSERT INTO `student` VALUES ('306', 'Вадим', 'Половинкин', 'Алексеевич', '11');
INSERT INTO `student` VALUES ('307', 'Артем', 'Прохода', 'Валерьевич', '11');
INSERT INTO `student` VALUES ('308', 'Виктор', 'Рябов', 'Александрович', '11');
INSERT INTO `student` VALUES ('309', 'Сергей ', 'Севостьянов', 'Витальевич', '11');
INSERT INTO `student` VALUES ('310', 'Сергей ', 'Сергейчук', 'Вадимович', '11');
INSERT INTO `student` VALUES ('311', 'Андрей', 'Сидоренко', 'Петрович', '11');
INSERT INTO `student` VALUES ('312', 'Александр', 'Татаренко', 'Владимирович', '11');
INSERT INTO `student` VALUES ('313', 'Юлия', 'Титкова ', 'Дмитриевна', '11');
INSERT INTO `student` VALUES ('314', 'Алексей', 'Фещенко', 'Васильевич', '11');
INSERT INTO `student` VALUES ('315', 'Антон', 'Фещенко', 'Васильевич', '11');
INSERT INTO `student` VALUES ('316', 'Глеб', 'Фурманов', 'Владимирович', '11');
INSERT INTO `student` VALUES ('317', 'Михаил', 'Шатсков', 'Александрович', '11');
INSERT INTO `student` VALUES ('318', 'Сергей', 'Антоненко', 'Андреевич', '12');
INSERT INTO `student` VALUES ('319', 'Виталий', 'Артюшков ', 'Васильевич', '12');
INSERT INTO `student` VALUES ('320', 'Денис', 'Бычков', 'Игоревич', '12');
INSERT INTO `student` VALUES ('321', 'Иван', 'Доронин', 'Владимирович', '12');
INSERT INTO `student` VALUES ('322', 'Роман', 'Дюжий', 'Юрьевич', '12');
INSERT INTO `student` VALUES ('323', 'Александр', 'Емельянов', 'Сергеевич', '12');
INSERT INTO `student` VALUES ('324', 'Иван', 'Ерохин ', 'Евгеньевич', '12');
INSERT INTO `student` VALUES ('325', 'Даниил', 'Жарков', 'Игоревич', '12');
INSERT INTO `student` VALUES ('326', 'Александр', 'Зятев', 'Валерьевич', '12');
INSERT INTO `student` VALUES ('327', 'Андрей', 'Комасин', 'Олегович', '12');
INSERT INTO `student` VALUES ('328', 'Максим', 'Кормилкин', 'Юрьевич', '12');
INSERT INTO `student` VALUES ('329', 'Александр', 'Кургузов', 'Николаевич', '12');
INSERT INTO `student` VALUES ('330', 'Владислав', 'Лазаренков', 'Владимирович', '12');
INSERT INTO `student` VALUES ('331', 'Алексей', 'Меркулов', 'Александрович', '12');
INSERT INTO `student` VALUES ('332', 'Владислав', 'Метелкин', 'Олегович', '12');
INSERT INTO `student` VALUES ('333', 'Виктор', 'Никитин', 'Юрьевич', '12');
INSERT INTO `student` VALUES ('334', 'Константин', 'Окорков', 'Павлович', '12');
INSERT INTO `student` VALUES ('335', 'Сергей', 'Павликов', 'Васильевич', '12');
INSERT INTO `student` VALUES ('336', 'Никита', 'Пайко', 'Владимирович', '12');
INSERT INTO `student` VALUES ('337', 'Родион', 'Пугачев', 'Викторович', '12');
INSERT INTO `student` VALUES ('338', 'Владимир ', 'Романенков', 'Сергеевич', '12');
INSERT INTO `student` VALUES ('339', 'Максим', 'Рыженков', 'Анатольевич', '12');
INSERT INTO `student` VALUES ('340', 'Артём', 'Смоляков', 'Андреевич', '12');
INSERT INTO `student` VALUES ('341', 'Виталий', 'Телицын', 'Олегович', '12');
INSERT INTO `student` VALUES ('342', 'Александр', 'Ториков ', 'Иванович', '12');
INSERT INTO `student` VALUES ('343', 'Максим', 'Трошкин', 'Михайлович', '12');
INSERT INTO `student` VALUES ('344', 'Дмитрий', 'Филатов', 'Александрович', '12');
INSERT INTO `student` VALUES ('345', 'Дмитрий', 'Фролов', 'Игоревич', '12');
INSERT INTO `student` VALUES ('346', 'Андрей', 'Чепурнов', 'Викторович', '12');
INSERT INTO `student` VALUES ('347', 'Иван', 'Анисин', 'Сергеевич', '13');
INSERT INTO `student` VALUES ('348', 'Игорь', 'Булычев', 'Александрович', '13');
INSERT INTO `student` VALUES ('349', 'Владислав', 'Бурносов', 'Олегович', '13');
INSERT INTO `student` VALUES ('350', 'Денис', 'Бычков', 'Владимирович', '13');
INSERT INTO `student` VALUES ('351', 'Евгений', 'Волков ', 'Викторович', '13');
INSERT INTO `student` VALUES ('352', 'Евгений', 'Волков ', 'Юрьевич', '13');
INSERT INTO `student` VALUES ('353', 'Юрий', 'Гавричков', 'Игоревич', '13');
INSERT INTO `student` VALUES ('354', 'Алексей', 'Гущин', 'Анатольевич', '13');
INSERT INTO `student` VALUES ('355', 'Александр', 'Егоров', 'Александрович', '13');
INSERT INTO `student` VALUES ('356', 'Дмитрий', 'Ермоленко', 'Александрович', '13');
INSERT INTO `student` VALUES ('357', 'Юрий', 'Карпенко', 'Владимирович', '13');
INSERT INTO `student` VALUES ('358', 'Юрий', 'Китасов', 'Борисович', '13');
INSERT INTO `student` VALUES ('359', 'Иван', 'Коваленко', 'Павлович', '13');
INSERT INTO `student` VALUES ('360', 'Денис', 'Кошарный', 'Вадимович', '13');
INSERT INTO `student` VALUES ('361', 'Евгений', 'Левшонков', 'Владимирович', '13');
INSERT INTO `student` VALUES ('362', 'Илья', 'Макаров', 'Дмитриевич', '13');
INSERT INTO `student` VALUES ('363', 'Денис', 'Мартынов', 'Сергеевич', '13');
INSERT INTO `student` VALUES ('364', 'Александр', 'Матвеев', 'Олегович', '13');
INSERT INTO `student` VALUES ('365', 'Дмитрий', 'Октябрьский', 'Алексеевич', '13');
INSERT INTO `student` VALUES ('366', 'Дмитрий', 'Полешко', 'Олегович', '13');
INSERT INTO `student` VALUES ('367', 'Артем', 'Праздников', 'Сергеевич', '13');
INSERT INTO `student` VALUES ('368', 'Сергей', 'Прокопенков', 'Олегович', '13');
INSERT INTO `student` VALUES ('369', 'Алексей', 'Разиков', 'Сергеевич', '13');
INSERT INTO `student` VALUES ('370', 'Дмитрий', 'Рыжиков', 'Игоревич', '13');
INSERT INTO `student` VALUES ('371', 'Владимир', 'Сиников', 'Владимирович', '13');
INSERT INTO `student` VALUES ('372', 'Евгений', 'Солдатенков', 'Александрович', '13');
INSERT INTO `student` VALUES ('373', 'Александр', 'Тимохов', 'Александрович', '13');
INSERT INTO `student` VALUES ('374', 'Владислав', 'Фромешкин', 'Максимович', '13');
INSERT INTO `student` VALUES ('375', 'Евгений', 'Хенкин', 'Игоревич', '13');
INSERT INTO `student` VALUES ('376', 'Дмитрий', 'Аркашов', 'Олегович', '14');
INSERT INTO `student` VALUES ('377', 'Сергей', 'Башмаков', 'Александрович', '14');
INSERT INTO `student` VALUES ('378', 'Дмитрий', 'Бородулин', 'Сергеевич', '14');
INSERT INTO `student` VALUES ('379', 'Сабина', 'Гараева', 'Азер гызы', '14');
INSERT INTO `student` VALUES ('380', 'Елизавета', 'Гришаева', 'Викторовна', '14');
INSERT INTO `student` VALUES ('381', 'Иван', 'Дмитриев', 'Сергеевич', '14');
INSERT INTO `student` VALUES ('382', 'Кирилл', 'Ковзиков', 'Владимирович', '14');
INSERT INTO `student` VALUES ('383', 'Екатерина', 'Кулаченко', 'Алексеевна', '14');
INSERT INTO `student` VALUES ('384', 'Ольга', 'Левкина', 'Константиновна', '14');
INSERT INTO `student` VALUES ('385', 'Екатерина', 'Лопатина', 'Владимировна', '14');
INSERT INTO `student` VALUES ('386', 'Александр', 'Мельников', 'Леонидович', '14');
INSERT INTO `student` VALUES ('387', 'Николай', 'Московец', 'Сергеевич', '14');
INSERT INTO `student` VALUES ('388', 'Константин', 'Пискарев', 'Андреевич', '14');
INSERT INTO `student` VALUES ('389', 'Никита', 'Рудской', 'Сергеевич', '14');
INSERT INTO `student` VALUES ('390', 'Михаил', 'Сехин', 'Михайлович', '14');
INSERT INTO `student` VALUES ('391', 'Дмитрий', 'Сорокин', 'Константинович', '14');
INSERT INTO `student` VALUES ('392', 'Илья', 'Стешков', 'Игоревич', '14');
INSERT INTO `student` VALUES ('393', 'Дмитрий', 'Титов', 'Сергеевич', '14');
INSERT INTO `student` VALUES ('394', 'Валерий', 'Тремасов', 'Андреевич', '14');
INSERT INTO `student` VALUES ('395', 'Денис', 'Хоровец', 'Сергеевич', '14');
INSERT INTO `student` VALUES ('396', 'Дарья', 'Хромых', 'Сергеевна', '14');
INSERT INTO `student` VALUES ('397', 'Виктория', 'Чепик', 'Владимировна', '14');
INSERT INTO `student` VALUES ('398', 'Мария', 'Чудакова', 'Владимировна', '14');
INSERT INTO `student` VALUES ('399', 'Алексей', 'Шпиньков', 'Витальевич', '14');
INSERT INTO `student` VALUES ('400', 'Виталий', 'Юрьев', 'Владимирович', '14');
INSERT INTO `student` VALUES ('401', 'Юлия', 'Савченко ', 'Алексеевна', '14');
INSERT INTO `student` VALUES ('402', 'Анастасия', 'Амельченкова', 'Геннадьевна', '15');
INSERT INTO `student` VALUES ('403', 'Константин', 'Алгиничев', 'Владимирович', '15');
INSERT INTO `student` VALUES ('404', 'Игорь', 'Аносов ', 'Владимирович', '15');
INSERT INTO `student` VALUES ('405', 'Сергей', 'Пташников', 'Александрович', '15');
INSERT INTO `student` VALUES ('406', 'Егор', 'Болгов', 'Сергеевич', '15');
INSERT INTO `student` VALUES ('407', 'Юлия', 'Васина', 'Вячеславовна', '15');
INSERT INTO `student` VALUES ('408', 'Олег', 'Вьюков', 'Викторович', '15');
INSERT INTO `student` VALUES ('409', 'Маргарита', 'Гришина', 'Андреевна', '15');
INSERT INTO `student` VALUES ('410', 'Георгий', 'Гулевский', 'Дмитриевич', '15');
INSERT INTO `student` VALUES ('411', 'Юлия', 'Давыдова', 'Александровна', '15');
INSERT INTO `student` VALUES ('412', 'Александр', 'Кобяков ', 'Александрович', '15');
INSERT INTO `student` VALUES ('413', 'Дмитрий', 'Колесов', 'Дмитриевич', '15');
INSERT INTO `student` VALUES ('414', 'Наталья', 'Коробченко', 'Владимировна', '15');
INSERT INTO `student` VALUES ('415', 'Александр', 'Кузнецов', 'Юрьевич', '15');
INSERT INTO `student` VALUES ('416', 'Алексей', 'Кулешов', 'Игоревич', '15');
INSERT INTO `student` VALUES ('417', 'Денис', 'Кучин', 'Михайлович', '15');
INSERT INTO `student` VALUES ('418', 'Максим', 'Малышев', 'Валерьевич', '15');
INSERT INTO `student` VALUES ('419', 'Карина', 'Миронова', 'Николаевна', '15');
INSERT INTO `student` VALUES ('420', 'Сергей', 'Моисеев', 'Александрович', '15');
INSERT INTO `student` VALUES ('421', 'Юрий', 'Орлов', 'Сергеевич', '15');
INSERT INTO `student` VALUES ('422', 'Олег', 'Придня', 'Геннадьевич', '15');
INSERT INTO `student` VALUES ('423', 'Владислав', 'Романченко', 'Александрович', '15');
INSERT INTO `student` VALUES ('424', 'Екатерина', 'Тарасенко', 'Борисовна', '15');
INSERT INTO `student` VALUES ('425', 'Юрий', 'Тишкин', 'Михайлович', '15');
INSERT INTO `student` VALUES ('426', 'Валерия', 'Фатеева', 'Андреевна', '15');
INSERT INTO `student` VALUES ('427', 'Артем', 'Харитоненко', 'Сергеевич', '15');
INSERT INTO `student` VALUES ('428', 'Максим', 'Хорт', 'Сергеевич', '15');
INSERT INTO `student` VALUES ('429', 'Елена', 'Хулапова', 'Сергеевна', '15');
INSERT INTO `student` VALUES ('430', 'Максим', 'Шапаков', 'Константинович', '15');
INSERT INTO `student` VALUES ('431', 'Юлия', 'Южанинова', 'Николаевна', '15');
INSERT INTO `student` VALUES ('432', 'Алексей', 'Якименко', 'Владимирович', '15');

-- ----------------------------
-- Table structure for `subject`
-- ----------------------------
DROP TABLE IF EXISTS `subject`;
CREATE TABLE `subject` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `subject` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of subject
-- ----------------------------
INSERT INTO `subject` VALUES ('1', 'WEB дизайн');
INSERT INTO `subject` VALUES ('2', 'Технология разработки програмного продукта');
INSERT INTO `subject` VALUES ('3', 'Информационная безопасность');
INSERT INTO `subject` VALUES ('4', 'Информационные технологии');
INSERT INTO `subject` VALUES ('5', 'Основы алгоритмизации и программирования');
INSERT INTO `subject` VALUES ('6', 'Менеджмент');

-- ----------------------------
-- Table structure for `subject_specialty`
-- ----------------------------
DROP TABLE IF EXISTS `subject_specialty`;
CREATE TABLE `subject_specialty` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `subject` int(11) NOT NULL,
  `specialty` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `subspec_ibfk_1` (`subject`),
  KEY `subspec_ibfk_2` (`specialty`),
  CONSTRAINT `subspec_ibfk_1` FOREIGN KEY (`subject`) REFERENCES `subject` (`id`) ON DELETE CASCADE,
  CONSTRAINT `subspec_ibfk_2` FOREIGN KEY (`specialty`) REFERENCES `specialty` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of subject_specialty
-- ----------------------------
INSERT INTO `subject_specialty` VALUES ('1', '1', '1');
INSERT INTO `subject_specialty` VALUES ('2', '2', '1');
INSERT INTO `subject_specialty` VALUES ('3', '3', '1');
INSERT INTO `subject_specialty` VALUES ('4', '4', '1');
INSERT INTO `subject_specialty` VALUES ('5', '5', '1');
INSERT INTO `subject_specialty` VALUES ('6', '6', '1');
INSERT INTO `subject_specialty` VALUES ('7', '6', '2');

-- ----------------------------
-- Table structure for `testing_state`
-- ----------------------------
DROP TABLE IF EXISTS `testing_state`;
CREATE TABLE `testing_state` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `student` int(11) NOT NULL COMMENT 'Студент проходящий тестирование',
  `test` varchar(50) NOT NULL COMMENT 'ключ теста',
  `date` date NOT NULL COMMENT 'дата прохождения тестирования',
  `end_date` date NOT NULL,
  `tests_count` int(2) NOT NULL COMMENT 'количество использованых попыток',
  PRIMARY KEY (`id`),
  KEY `student` (`student`),
  CONSTRAINT `testing_state_ibfk_1` FOREIGN KEY (`student`) REFERENCES `student` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=cp1251 COMMENT='Информация о прохождении студентом тестирования';

-- ----------------------------
-- Records of testing_state
-- ----------------------------

-- ----------------------------
-- Table structure for `test_results`
-- ----------------------------
DROP TABLE IF EXISTS `test_results`;
CREATE TABLE `test_results` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `subject` int(11) NOT NULL COMMENT 'Предмет',
  `test` varchar(50) NOT NULL COMMENT 'идентификатор теста',
  `student` int(11) NOT NULL COMMENT 'ID студента',
  `trying` int(5) NOT NULL,
  `score_type` smallint(1) NOT NULL COMMENT 'Тип тестирования: 0-пятибальная система, 1-зачет/незачет',
  `right_count` int(11) NOT NULL COMMENT 'Количество правильных ответов',
  `percent` smallint(3) NOT NULL COMMENT 'процент правильных',
  `score` smallint(1) NOT NULL COMMENT 'полученная оценка',
  `alter_score` varchar(1024) NOT NULL COMMENT 'альтернативный текст к оценке',
  `report` blob NOT NULL COMMENT 'отчет о тесте',
  `data` date NOT NULL,
  `test_name` varchar(1024) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `testresults_ibfk_1` (`subject`),
  KEY `student` (`student`),
  CONSTRAINT `testresults_ibfk_1` FOREIGN KEY (`subject`) REFERENCES `subject` (`id`) ON DELETE CASCADE,
  CONSTRAINT `test_results_ibfk_1` FOREIGN KEY (`student`) REFERENCES `student` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of test_results
-- ----------------------------
INSERT INTO `test_results` VALUES ('6', '4', '4fff137e9e737b54e8661009da623ac5', '11', '16', '0', '0', '0', '2', 'Плохо', 0x0000002E785EB3B1AFC8CD51284B2D2ACECCCFB35532D433505248CD4BCE4FC9CC4BB7550ACFCC4BC92F2FD63534323554B2B7E3020065F80E53, '2011-03-09', 'Егэ информатика\r\nВсе темы');

-- ----------------------------
-- View structure for `allstudents`
-- ----------------------------
DROP VIEW IF EXISTS `allstudents`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `allstudents` AS select `student`.`id` AS `id`,concat_ws(' ',`student`.`surname`,`student`.`name`,`student`.`patronymic`) AS `stud`,`groups`.`caption` AS `caption`,`groups`.`cource` AS `cource` from (`student` join `groups` on((`groups`.`id` = `student`.`group`)));

-- ----------------------------
-- Procedure structure for `12`
-- ----------------------------
DROP PROCEDURE IF EXISTS `12`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `12`(in start int(11))
begin
	declare buf varchar(50);
	declare i int(11) default 1;
	declare col int(11) default 0;

	select count(student.id) into col from student;

	set i = start;
	while  i <= col do
		select student.name into buf from student where student.id = i;
		update student set student.name = student.surname, student.surname = buf where
			student.id = i;
		set i = i + 1;
	end while;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `AddStudent`
-- ----------------------------
DROP PROCEDURE IF EXISTS `AddStudent`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AddStudent`(in sF varchar(50), in sI varchar(50), in sO varchar(50), in grName varchar(50))
begin
	declare groupID int default 0;/*код группы*/
	/*Найти код группы по имени*/
	select groups.id into groupID from groups where groups.caption like grName;
	select groupID;

	/*
		Проверка учится ли данный студент
	*/
	if not exists (select *
		FROM
		student
		Inner Join groups ON groups.id = student.`group`
		Inner Join basegroups ON basegroups.id = groups.base
		Inner Join specialty ON specialty.id = basegroups.specialty
		WHERE
		lcase(student.name) =  sI AND
		lcase(student.surname) =  sF AND
		lcase(student.patronymic) =  sO AND
		(year(groups.intake)-year(now())) <=  specialty.during) then

			select groupID;
	end if;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `makeGroupScores`
-- ----------------------------
DROP PROCEDURE IF EXISTS `makeGroupScores`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `makeGroupScores`(cGroup varchar(50), cSubject varchar(100), tstIdent varchar(50))
BEGIN
	declare grID int default -1;
	declare subjectID, studID int default -1;
	declare done int default 0;
	declare stud_surname, stud_name, stud_patronymic, stud_group varchar(100);

	declare strying, grcount, spercent, sscore, ccount int;
	declare salter varchar(100);
	declare sdata date;

	declare cur cursor for SELECT student.id, student.surname, student.name, student.patronymic, groups.caption
					FROM student Inner Join groups ON groups.id = student.`group`
					WHERE groups.caption = cGroup;
	declare continue handler for sqlstate '02000' set done = 1;

	select groups.id into grID from groups where groups.caption = cGroup;
	select subject.id into subjectID from subject where subject.subject = cSubject;

	select groups.id into grID from groups where groups.caption = cGroup;
	select subject.id into subjectID from subject where subject.subject = cSubject;

	create TEMPORARY table if not exists jornay(
		student_name varchar(256),
		student_trying int(2),
		student_right_count int(2),
		student_percent int(2),
		student_score int(1),
		student_alter_score varchar(100),
		test_data date
	);

	open cur;
	while done <> 1 do
		fetch cur into studID, stud_surname, stud_name, stud_patronymic, stud_group;
		select MAX(test_results.id),trying, right_count, percent, score, alter_score, data
				into ccount, strying, grcount, spercent, sscore, salter, `sdata`
				from test_results
				where student = studID AND test_results.test = tstIdent;
		if ccount <> 0 THEN
			insert into jornay values(concat_ws(' ', stud_surname, stud_name, stud_patronymic),
				strying, grcount, spercent, sscore, salter, sdata);
		ELSE
			insert into jornay values(concat_ws(' ', stud_surname, stud_name, stud_patronymic),
				0, 0, 0, 0, 'Студент не проходил тестирование', NOW());
		END IF;
	end while;
	close cur;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `translateStudents`
-- ----------------------------
DROP PROCEDURE IF EXISTS `translateStudents`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `translateStudents`()
begin
	declare done int default 0;
	declare dDate date;
	declare groupID int(11) default -1;
	declare specID int(11) default -1;
	declare iDuring int(11) default -1;
	declare iCource int(11) default -1;
	declare group_name varchar(100) default '';
	declare cPrefics varchar(10) default '';
	declare iNum int(11) default -1;

	declare cur cursor for SELECT groups.intake, groups.id, groups.caption, groups.cource, groups.special, specialty.during
						FROM
						specialty
						Inner Join groups ON specialty.id = groups.special
						where
						(year(now()) - year(groups.intake)) <= specialty.during;

	declare continue handler for sqlstate '02000' set done = 1;

	open cur;
	REPEAT
		FETCH cur INTO dDate, groupID, group_name, iCource, specID, iDuring;
		if iCource < iDuring then
			select num, prefix into iNum, cPrefics from specialty where specialty.id = specID;
			set group_name = concat_ws('', iCource + 1, mid(group_name, 2, 1), ' ', cPrefics, ' ', date_format(dDate, '%y'));
			
			update groups set caption = group_name, cource = iCource + 1 where id = groupID;
		end if;
	UNTIL done END REPEAT;
	close cur;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `updateSpeciality`
-- ----------------------------
DROP PROCEDURE IF EXISTS `updateSpeciality`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateSpeciality`(op int(1), vKod int(11), vCaption varchar(100), vDuring int(11), vPref varchar(10), vNum int(1), oldKod int(11), oldCaption varchar(100))
if op = 0 then
	if not exists(select * from specialty where specialty.kod = vKod and specialty.caption = vCaption) then
	begin
		declare iLoop int(4) default 1;
		declare ident int(11) default -1;

		insert into specialty(kod, caption, during, prefix, num) values(vKod, vCaption, vDuring, vPref, vNum);
		select id into ident from specialty s where s.kod = vKod and s.caption like vCaption;
	end;
	end if;
else
	 if op = 1 and not exists(select * from specialty s where s.kod = vKod and s.caption like vCaption) then
	begin
		declare ident int(11)  default -1;
		select id into ident from specialty s where s.kod = oldKod and s.caption like oldCaption;
		if ident <> -1 then
			update specialty set kod = vKod, caption = vCaption, during = vDuring,  prefix =  vPref, num = vNum  where id = ident;
		end if;
	end;
	else
	begin
		declare ident int(11)  default -1;
		select id into ident from specialty s where s.kod = vKod and s.caption like vCaption;
		if ident <> -1 then
			delete from specialty where id = ident;
		end if;
	end;
	end if;
end if
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `UpdateTestResults`
-- ----------------------------
DROP PROCEDURE IF EXISTS `UpdateTestResults`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UpdateTestResults`(subj int(11), testIdenty varchar(50), studID int(11), scType smallint(1), rCount smallint(3), rPercent smallint(3), uScore smallint(3), altScore varchar(1024), uReport blob, uTestName varchar(1024))
if not exists(select * from test_results as t where ((t.subject = subj) and (t.test = md5(testIdenty)) and
		(t.student =  studID))) then
begin
	insert into test_results(subject, test, student, score_type, right_count, percent, score, alter_score, report, trying, `data`, test_name)
	values(subj, md5(testIdenty), studID, scType, rCount, rPercent, uScore, altScore, uReport, '1', Now(), uTestName );
end;
else
begin
	update test_results set subject = subj,  test = md5(testIdenty), student = studID, score_type = scType,
		 right_count = rCount, percent = rPercent, score = uScore, alter_score = altScore, report = uReport,
		trying =  trying + 1, `data` = Now(), test_name = uTestName
		where ((subject = subj) and (test = md5(testIdenty)) and
		(student =  studID));
end;
end if
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `UpdateTestState`
-- ----------------------------
DROP PROCEDURE IF EXISTS `UpdateTestState`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UpdateTestState`(st int(11), testIdent varchar(50), days int(3))
begin
	/*
		Проверить существование записи в таблице
		Если запись существует увеличить количество попыток,
		иначе создать запись
	*/
	if not exists(select * from testing_state as t where (Now() between  t.date and t.end_date) and
	(t.student = st) and (t.test =  md5(testIdent))) then
	begin
		declare newDate date default Now();
		set newDate = DATE_ADD(Now(), interval days DAY);
		insert into testing_state(student, test, date, end_date, tests_count)
			values(st, md5(testIdent), Now(), newDate, 1);
	end;
	else
	begin
		update testing_state set tests_count = tests_count + 1 where
			 (Now() between  testing_state.date and testing_state.end_date) and
			(testing_state.student = st) and (testing_state.test =  md5(testIdent));
	end;
	end if;
end
;;
DELIMITER ;

-- ----------------------------
-- Function structure for `BEGIN_OF_MONTH`
-- ----------------------------
DROP FUNCTION IF EXISTS `BEGIN_OF_MONTH`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `BEGIN_OF_MONTH`(p_date DATE) RETURNS date
    DETERMINISTIC
    SQL SECURITY INVOKER
return last_day(p_date) + interval 1 day - interval 1 month
;;
DELIMITER ;

-- ----------------------------
-- Function structure for `END_OF_MONTH`
-- ----------------------------
DROP FUNCTION IF EXISTS `END_OF_MONTH`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `END_OF_MONTH`(p_date DATE) RETURNS date
    DETERMINISTIC
    SQL SECURITY INVOKER
return last_day(p_date)
;;
DELIMITER ;

-- ----------------------------
-- Function structure for `IsStudentExists`
-- ----------------------------
DROP FUNCTION IF EXISTS `IsStudentExists`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `IsStudentExists`(param text) RETURNS tinyint(1)
begin
/*if exists (select `student`.`id` AS `id`,
	concat_ws(' ',`student`.`surname`,`student`.`name`,`student`.`patronymic`)  AS v1
	from `student` where v1 = param) then
		return '1';
else
	return '0';
end if;*/

	return exists (select * from allstudents s where s.stud = param );
end
;;
DELIMITER ;
