DROP TABLE IF EXISTS `item_transmog`;
CREATE TABLE `item_transmog` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `transmog` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
