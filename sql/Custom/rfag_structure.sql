/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50612
Source Host           : localhost:3306
Source Database       : realmd

Target Server Type    : MYSQL
Target Server Version : 50612
File Encoding         : 65001

Date: 2013-11-09 02:42:16
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `rfag_account_groups`
-- ----------------------------
DROP TABLE IF EXISTS `rfag_account_groups`;
CREATE TABLE `rfag_account_groups` (
  `accid` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `groupid` int(11) unsigned NOT NULL,
  PRIMARY KEY (`accid`,`groupid`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of rfag_account_groups
-- ----------------------------
INSERT INTO `rfag_account_groups` VALUES ('5', '1');

-- ----------------------------
-- Table structure for `rfag_account_permissions`
-- ----------------------------
DROP TABLE IF EXISTS `rfag_account_permissions`;
CREATE TABLE `rfag_account_permissions` (
  `accid` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `permname` varchar(32) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `remove` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`accid`,`permname`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of rfag_account_permissions
-- ----------------------------

-- ----------------------------
-- Table structure for `rfag_group_inheritance`
-- ----------------------------
DROP TABLE IF EXISTS `rfag_group_inheritance`;
CREATE TABLE `rfag_group_inheritance` (
  `inheritor` int(11) unsigned NOT NULL,
  `inherited` int(11) unsigned NOT NULL,
  PRIMARY KEY (`inheritor`,`inherited`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of rfag_group_inheritance
-- ----------------------------

-- ----------------------------
-- Table structure for `rfag_group_permissions`
-- ----------------------------
DROP TABLE IF EXISTS `rfag_group_permissions`;
CREATE TABLE `rfag_group_permissions` (
  `groupid` int(11) unsigned NOT NULL,
  `permname` varchar(32) CHARACTER SET utf8 NOT NULL DEFAULT '',
  PRIMARY KEY (`groupid`,`permname`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of rfag_group_permissions
-- ----------------------------
