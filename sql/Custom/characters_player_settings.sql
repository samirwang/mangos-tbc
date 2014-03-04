DROP TABLE IF EXISTS `player_settings`;
CREATE TABLE `player_settings` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `SettingNumber` int(11) NOT NULL DEFAULT '0',
  `DataTypeID` int(11) NOT NULL DEFAULT '0',
  `FloatSetting` float NOT NULL DEFAULT '0',
  `IntSetting` int(11) NOT NULL DEFAULT '0',
  `UintSetting` int(11) unsigned NOT NULL DEFAULT '0',
  `StringSetting` varchar(255) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`SettingNumber`,`DataTypeID`,`StringSetting`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
