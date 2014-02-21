DROP TABLE IF EXISTS `playertemplate_enchants`;
CREATE TABLE `playertemplate_enchants` (
  `class` tinyint(2) NOT NULL,
  `spec` tinyint(2) NOT NULL,
  `spellid` bigint(6) NOT NULL,
  `slotid` tinyint(2) DEFAULT NULL,
  PRIMARY KEY (`class`,`spec`,`spellid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
