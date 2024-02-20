/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 80025
 Source Host           : localhost:3306
 Source Schema         : exhi_management

 Target Server Type    : MySQL
 Target Server Version : 80025
 File Encoding         : 65001

 Date: 15/12/2021 16:55:11
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for admin
-- ----------------------------
DROP TABLE IF EXISTS `admin`;
CREATE TABLE `admin`  (
  `adminid` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`adminid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of admin
-- ----------------------------
INSERT INTO `admin` VALUES (1, 'admin');
INSERT INTO `admin` VALUES (2, '123456');

-- ----------------------------
-- Table structure for exhibition
-- ----------------------------
DROP TABLE IF EXISTS `exhibition`;
CREATE TABLE `exhibition`  (
  `eid` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `ename` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `start_date` date NOT NULL,
  `end_date` date NOT NULL,
  `eintro` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `epicture` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `mid` int NOT NULL,
  PRIMARY KEY (`eid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 9 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of exhibition
-- ----------------------------
INSERT INTO `exhibition` VALUES (1, '国家宝藏-上海站', '2021-12-06', '2021-12-16', '国家宝藏上海站', '/null', 1);
INSERT INTO `exhibition` VALUES (2, '国家宝藏-北京站', '2021-12-17', '2021-12-27', '国家宝藏北京站', '/null', 3);
INSERT INTO `exhibition` VALUES (3, '国家宝藏-南京站', '2022-01-01', '2021-12-11', '国家宝藏南京站', '/null', 2);
INSERT INTO `exhibition` VALUES (4, '戏曲金陵', '2021-12-01', '2022-01-01', '南京的戏曲文化', '/null', 2);

-- ----------------------------
-- Table structure for likelist
-- ----------------------------
DROP TABLE IF EXISTS `likelist`;
CREATE TABLE `likelist`  (
  `uid` int UNSIGNED NOT NULL,
  `eid` int UNSIGNED NOT NULL,
  PRIMARY KEY (`uid`, `eid`) USING BTREE,
  INDEX `likeeid`(`eid`) USING BTREE,
  CONSTRAINT `likeeid` FOREIGN KEY (`eid`) REFERENCES `exhibition` (`eid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `likeuid` FOREIGN KEY (`uid`) REFERENCES `user` (`uid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of likelist
-- ----------------------------

-- ----------------------------
-- Table structure for museum
-- ----------------------------
DROP TABLE IF EXISTS `museum`;
CREATE TABLE `museum`  (
  `mid` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `mname` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `mintro` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `mpicture` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `city` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`mid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 5 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of museum
-- ----------------------------
INSERT INTO `museum` VALUES (1, '上海博物馆', '上海博物馆', '/null', '上海');
INSERT INTO `museum` VALUES (2, '南京博物院', '南京博物院', '/null', '南京');
INSERT INTO `museum` VALUES (3, '北京博物馆', '北京博物馆', '/null', '北京');

-- ----------------------------
-- Table structure for showlist
-- ----------------------------
DROP TABLE IF EXISTS `showlist`;
CREATE TABLE `showlist`  (
  `eid` int UNSIGNED NOT NULL,
  `tid` int UNSIGNED NOT NULL,
  PRIMARY KEY (`eid`, `tid`) USING BTREE,
  INDEX `showtid`(`tid`) USING BTREE,
  CONSTRAINT `showeid` FOREIGN KEY (`eid`) REFERENCES `exhibition` (`eid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `showtid` FOREIGN KEY (`tid`) REFERENCES `treasure` (`tid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of showlist
-- ----------------------------
INSERT INTO `showlist` VALUES (4, 1);
INSERT INTO `showlist` VALUES (1, 2);
INSERT INTO `showlist` VALUES (2, 2);
INSERT INTO `showlist` VALUES (3, 2);
INSERT INTO `showlist` VALUES (4, 2);
INSERT INTO `showlist` VALUES (1, 3);
INSERT INTO `showlist` VALUES (2, 3);
INSERT INTO `showlist` VALUES (3, 3);
INSERT INTO `showlist` VALUES (4, 3);
INSERT INTO `showlist` VALUES (1, 4);
INSERT INTO `showlist` VALUES (2, 4);
INSERT INTO `showlist` VALUES (3, 4);

-- ----------------------------
-- Table structure for treasure
-- ----------------------------
DROP TABLE IF EXISTS `treasure`;
CREATE TABLE `treasure`  (
  `tid` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `tname` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `mid` int NOT NULL,
  PRIMARY KEY (`tid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 15 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of treasure
-- ----------------------------
INSERT INTO `treasure` VALUES (1, '金陵戏曲面具', 2);
INSERT INTO `treasure` VALUES (2, '清明上河图', 3);
INSERT INTO `treasure` VALUES (3, '京剧戏曲面具', 3);
INSERT INTO `treasure` VALUES (4, '上海的宝藏', 1);

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`  (
  `uid` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `uname` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`uid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 10 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES (1, '畅若', '123456');
INSERT INTO `user` VALUES (2, 'Uranium', '123456');
INSERT INTO `user` VALUES (3, 'Apple', '123456');
INSERT INTO `user` VALUES (4, '安若', '123456');
INSERT INTO `user` VALUES (9, 'flower', '123456');

SET FOREIGN_KEY_CHECKS = 1;
