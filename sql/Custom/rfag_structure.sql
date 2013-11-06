/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50612
Source Host           : localhost:3306
Source Database       : realmd

Target Server Type    : MYSQL
Target Server Version : 50612
File Encoding         : 65001

Date: 2013-11-05 23:54:23
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `rfag_account_groups`
-- ----------------------------
DROP TABLE IF EXISTS `rfag_account_groups`;
CREATE TABLE `rfag_account_groups` (
  `accid` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `groupid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`accid`,`groupid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of rfag_account_groups
-- ----------------------------

-- ----------------------------
-- Table structure for `rfag_account_permissions`
-- ----------------------------
DROP TABLE IF EXISTS `rfag_account_permissions`;
CREATE TABLE `rfag_account_permissions` (
  `accid` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `permid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`accid`,`permid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of rfag_account_permissions
-- ----------------------------

-- ----------------------------
-- Table structure for `rfag_group_permissions`
-- ----------------------------
DROP TABLE IF EXISTS `rfag_group_permissions`;
CREATE TABLE `rfag_group_permissions` (
  `groupid` int(10) unsigned NOT NULL,
  `permid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`groupid`,`permid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of rfag_group_permissions
-- ----------------------------
