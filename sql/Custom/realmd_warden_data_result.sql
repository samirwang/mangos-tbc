/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50612
Source Host           : localhost:3306
Source Database       : ptr_realmd

Target Server Type    : MYSQL
Target Server Version : 50612
File Encoding         : 65001

Date: 2014-04-01 15:20:43
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for warden_data_result
-- ----------------------------
DROP TABLE IF EXISTS `warden_data_result`;
CREATE TABLE `warden_data_result` (
  `check` int(3) DEFAULT NULL,
  `data` tinytext,
  `str` tinytext,
  `address` int(8) DEFAULT NULL,
  `length` int(2) DEFAULT NULL,
  `result` tinytext,
  `comment` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of warden_data_result
-- ----------------------------
INSERT INTO `warden_data_result` VALUES ('243', '', '', '4840352', '2', '558B', 'lua protection');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '8979979', '5', 'E04D62503F', 'Hyper speed');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '9208923', '5', 'C0854A3340', 'gravity');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '8100062', '2', '7541', 'air jump');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '8095301', '2', '7513', 'Air Jump 2');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '9208728', '4', 'BB8D243F', 'Wallclimb');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '6518444', '2', 'DECA', 'Wallclimb 1');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '8095954', '3', '8B4908 ', 'Teleport To Plane 2');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '8095957', '3', '894808 ', 'Teleport To Plane');
INSERT INTO `warden_data_result` VALUES ('243', '', '', '8103107', '3', '894E3C', 'No Fall DMG');
INSERT INTO `warden_data_result` VALUES ('217', '', 'WPESPY.DLL', '0', '0', '', 'WPE PRO - injected dll');
INSERT INTO `warden_data_result` VALUES ('217', '', 'RPE.DLL', '0', '0', '', 'rEdoX Packet Editor - injected dll');
