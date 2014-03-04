DROP TABLE IF EXISTS `fakeplayerbytes`;
CREATE TABLE `fakeplayerbytes` (
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `maleBytes` int(10) unsigned NOT NULL DEFAULT '0',
  `maleBytes2` int(10) unsigned NOT NULL DEFAULT '0',
  `femaleBytes` int(10) unsigned NOT NULL DEFAULT '0',
  `femaleBytes2` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`race`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `fakeplayerbytes` VALUES ('1', '151061248', '33554435', '67242757', '33554437');
INSERT INTO `fakeplayerbytes` VALUES ('2', '67110400', '33554433', '34014978', '33554434');
INSERT INTO `fakeplayerbytes` VALUES ('3', '67306498', '33554440', '16909320', '33554432');
INSERT INTO `fakeplayerbytes` VALUES ('4', '50464008', '33554435', '50724871', '33554439');
INSERT INTO `fakeplayerbytes` VALUES ('5', '67306497', '33554438', '100860930', '33554438');
INSERT INTO `fakeplayerbytes` VALUES ('6', '132111', '33554438', '33816584', '33554433');
INSERT INTO `fakeplayerbytes` VALUES ('7', '134546689', '33554436', '100663811', '33554434');
INSERT INTO `fakeplayerbytes` VALUES ('8', '67175170', '33554438', '67371778', '33554436');
INSERT INTO `fakeplayerbytes` VALUES ('10', '84215302', '33554439', '460547', '33554438');
INSERT INTO `fakeplayerbytes` VALUES ('11', '395523', '33554436', '50660358', '33554438');
