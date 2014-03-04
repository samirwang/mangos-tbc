DROP TABLE IF EXISTS `playertemplate_talents`;
CREATE TABLE `playertemplate_talents` (
  `class` tinyint(2) NOT NULL,
  `spec` tinyint(2) NOT NULL,
  `id` bigint(6) NOT NULL,
  `rank` tinyint(2) DEFAULT NULL,
  PRIMARY KEY (`class`,`spec`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
