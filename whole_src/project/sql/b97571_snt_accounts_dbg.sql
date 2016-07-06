-- phpMyAdmin SQL Dump
-- version 4.0.7
-- http://www.phpmyadmin.net
--
-- Host: 185.84.108.3
-- Generation Time: Jun 27, 2016 at 11:41 AM
-- Server version: 5.5.49-MariaDB-1ubuntu0.14.04.1
-- PHP Version: 5.3.27

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `b97571`
--

-- --------------------------------------------------------

--
-- Table structure for table `snt_accounts_dbg`
--

CREATE TABLE IF NOT EXISTS `snt_accounts_dbg` (
  `_id` int(11) NOT NULL AUTO_INCREMENT,
  `cid` varchar(60) NOT NULL,
  `nick` varchar(40) NOT NULL,
  `psw` varchar(20) NOT NULL,
  `ts_create` bigint(16) NOT NULL,
  `dt_create` varchar(80) NOT NULL,
  `ts_lastedit` bigint(16) NOT NULL,
  `dt_lastedit` varchar(80) NOT NULL,
  `ts_mode_lastedit` bigint(16) NOT NULL,
  `dt_mode_lastedit` varchar(80) NOT NULL,
  `activation_code` varchar(40) NOT NULL,
  `fk_acc_type` int(11) NOT NULL DEFAULT '1000',
  `fk_acc_mode` int(255) NOT NULL DEFAULT '1000',
  PRIMARY KEY (`_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT='accounts table' AUTO_INCREMENT=1137 ;

--
-- Dumping data for table `snt_accounts_dbg`
--

INSERT INTO `snt_accounts_dbg` (`_id`, `cid`, `nick`, `psw`, `ts_create`, `dt_create`, `ts_lastedit`, `dt_lastedit`, `ts_mode_lastedit`, `dt_mode_lastedit`, `activation_code`, `fk_acc_type`, `fk_acc_mode`) VALUES
(8, 'mycid', '', '', 0, '', 0, '', 0, '', '', 1000, 0),
(1136, 'test@ya.ru', 'new nick', '111', 1466762293775, '2016-06-24T14:58:13.775+05:00[Asia/Yekaterinburg]', 1466762294206, '2016-06-24T14:58:14.206+05:00[Asia/Yekaterinburg]', 1466762294206, '2016-06-24T14:58:14.206+05:00[Asia/Yekaterinburg]', '', 1000, 2);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
