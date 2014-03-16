DROP TABLE IF EXISTS `playerscript_template`;
CREATE TABLE `playerscript_template` (
  `entry` int(11) unsigned NOT NULL,
  `ScriptName` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `playerscript_template` VALUES ('1', 'playerscript_templates');
