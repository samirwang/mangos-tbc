ALTER TABLE `realmlist`
ADD COLUMN `localaddress`  varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '127.0.0.1' AFTER `address`,
ADD COLUMN `localsubmask`  varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '255.255.255.0' AFTER `localaddress`
