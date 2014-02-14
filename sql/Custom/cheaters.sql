DROP TABLE IF EXISTS `cheaters`;
CREATE TABLE `cheaters` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `account` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `type` varchar(255) CHARACTER SET latin1 NOT NULL DEFAULT 'Unknown',
  `time` bigint(40) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
