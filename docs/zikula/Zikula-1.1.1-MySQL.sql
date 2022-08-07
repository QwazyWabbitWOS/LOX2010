-- phpMyAdmin SQL Dump
-- version 3.0.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 20, 2008 at 02:42 PM
-- Server version: 5.0.67
-- PHP Version: 5.2.6-2ubuntu4
--
-- Zikula 1.1.1
--

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `zikula_1_1_0`
--
-- Password for the admin account is zikula - please change this
-- immediately after the installation!

-- --------------------------------------------------------

--
-- Table structure for table `zk_admin_category`
--

CREATE TABLE IF NOT EXISTS `zk_admin_category` (
  `pn_cid` int(11) NOT NULL auto_increment,
  `pn_name` varchar(32) collate latin1_german2_ci NOT NULL default '',
  `pn_description` varchar(254) collate latin1_german2_ci NOT NULL default '',
  PRIMARY KEY  (`pn_cid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=8 ;

--
-- Dumping data for table `zk_admin_category`
--

INSERT INTO `zk_admin_category` (`pn_cid`, `pn_name`, `pn_description`) VALUES
(1, 'System', 'Core modules at the heart of operation of this site'),
(2, 'Layout', 'Layout modules for controlling this site''s look and feel'),
(3, 'Users', 'Modules for controlling user membership, access rights and profiles'),
(4, 'Content', 'Modules for providing content to your users'),
(5, '3rd-Party', '3rd-party add-on modules and newly-installed modules'),
(6, 'Security', 'Modules for controlling the security of this site'),
(7, 'Hooked', 'Modules to be hooked to other modules, to provide them with enhanced functionality');

-- --------------------------------------------------------

--
-- Table structure for table `zk_admin_module`
--

CREATE TABLE IF NOT EXISTS `zk_admin_module` (
  `pn_amid` int(11) NOT NULL auto_increment,
  `pn_mid` int(11) NOT NULL default '0',
  `pn_cid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`pn_amid`),
  KEY `mid_cid` (`pn_mid`,`pn_cid`),
  KEY `mid` (`pn_mid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=23 ;

--
-- Dumping data for table `zk_admin_module`
--

INSERT INTO `zk_admin_module` (`pn_amid`, `pn_mid`, `pn_cid`) VALUES
(1, 1, 1),
(2, 7, 3),
(3, 12, 3),
(4, 19, 2),
(5, 10, 1),
(6, 2, 3),
(7, 11, 2),
(8, 16, 1),
(9, 17, 1),
(10, 13, 4),
(11, 8, 6),
(12, 14, 4),
(13, 5, 2),
(14, 4, 4),
(15, 15, 1),
(16, 21, 1),
(17, 9, 2),
(18, 18, 1),
(19, 20, 4),
(20, 3, 1),
(21, 23, 1),
(22, 6, 6);

-- --------------------------------------------------------

--
-- Table structure for table `zk_blocks`
--

CREATE TABLE IF NOT EXISTS `zk_blocks` (
  `pn_bid` int(11) NOT NULL auto_increment,
  `pn_bkey` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_title` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_content` longtext collate latin1_german2_ci NOT NULL,
  `pn_url` longtext collate latin1_german2_ci NOT NULL,
  `pn_mid` int(11) NOT NULL default '0',
  `pn_filter` longtext collate latin1_german2_ci NOT NULL,
  `pn_active` tinyint(4) NOT NULL default '1',
  `pn_collapsable` int(11) NOT NULL default '1',
  `pn_defaultstate` int(11) NOT NULL default '1',
  `pn_refresh` int(11) NOT NULL default '0',
  `pn_last_update` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `pn_language` varchar(30) collate latin1_german2_ci NOT NULL default '',
  PRIMARY KEY  (`pn_bid`),
  KEY `active_idx` (`pn_active`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=6 ;

--
-- Dumping data for table `zk_blocks`
--

INSERT INTO `zk_blocks` (`pn_bid`, `pn_bkey`, `pn_title`, `pn_content`, `pn_url`, `pn_mid`, `pn_filter`, `pn_active`, `pn_collapsable`, `pn_defaultstate`, `pn_refresh`, `pn_last_update`, `pn_language`) VALUES
(1, 'extmenu', 'Main menu', 'a:5:{s:14:"displaymodules";s:1:"1";s:10:"stylesheet";s:11:"extmenu.css";s:8:"template";s:24:"blocks_block_extmenu.htm";s:11:"blocktitles";a:1:{s:3:"eng";s:9:"Main menu";}s:5:"links";a:1:{s:3:"eng";a:4:{i:0;a:7:{s:4:"name";s:4:"Home";s:3:"url";s:10:"{homepage}";s:5:"title";s:21:"Back to the home page";s:5:"level";i:0;s:6:"parent";i:0;s:5:"image";s:0:"";s:6:"active";s:1:"1";}i:1;a:7:{s:4:"name";s:12:"Your account";s:3:"url";s:9:"{Profile}";s:5:"title";s:41:"Go to your personal account control panel";s:5:"level";i:0;s:6:"parent";i:0;s:5:"image";s:0:"";s:6:"active";s:1:"1";}i:2;a:7:{s:4:"name";s:14:"Administration";s:3:"url";s:24:"{Admin:adminpanel:admin}";s:5:"title";s:30:"Go to the administration panel";s:5:"level";i:0;s:6:"parent";i:0;s:5:"image";s:0:"";s:6:"active";s:1:"1";}i:3;a:7:{s:4:"name";s:7:"Log out";s:3:"url";s:14:"{Users:logout}";s:5:"title";s:23:"Log out of your account";s:5:"level";i:0;s:6:"parent";i:0;s:5:"image";s:0:"";s:6:"active";s:1:"1";}}}}', '', 19, '', 1, 1, 1, 3600, '2008-12-20 14:37:13', ''),
(2, 'thelang', 'Languages', '', '', 19, '', 1, 1, 1, 3600, '2008-12-20 14:37:13', ''),
(3, 'login', 'User log-in', '', '', 2, '', 1, 1, 1, 3600, '2008-12-20 14:37:13', ''),
(4, 'online', 'Who''s online', '', '', 2, '', 1, 1, 1, 3600, '2008-12-20 14:37:13', ''),
(5, 'messages', 'Administration Messages', '', '', 13, '', 1, 1, 1, 3600, '2008-12-20 14:37:13', '');

-- --------------------------------------------------------

--
-- Table structure for table `zk_block_placements`
--

CREATE TABLE IF NOT EXISTS `zk_block_placements` (
  `pn_pid` int(11) NOT NULL default '0',
  `pn_bid` int(11) NOT NULL default '0',
  `pn_order` int(11) NOT NULL default '0',
  KEY `bid_pid_idx` (`pn_bid`,`pn_pid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;

--
-- Dumping data for table `zk_block_placements`
--

INSERT INTO `zk_block_placements` (`pn_pid`, `pn_bid`, `pn_order`) VALUES
(1, 1, 0),
(1, 2, 0),
(2, 3, 0),
(2, 4, 0),
(3, 5, 0);

-- --------------------------------------------------------

--
-- Table structure for table `zk_block_positions`
--

CREATE TABLE IF NOT EXISTS `zk_block_positions` (
  `pn_pid` int(11) NOT NULL auto_increment,
  `pn_name` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_description` varchar(255) collate latin1_german2_ci NOT NULL default '',
  PRIMARY KEY  (`pn_pid`),
  KEY `name_idx` (`pn_name`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=4 ;

--
-- Dumping data for table `zk_block_positions`
--

INSERT INTO `zk_block_positions` (`pn_pid`, `pn_name`, `pn_description`) VALUES
(1, 'left', 'Left blocks'),
(2, 'right', 'Right blocks'),
(3, 'center', 'Center blocks');

-- --------------------------------------------------------

--
-- Table structure for table `zk_categories_category`
--

CREATE TABLE IF NOT EXISTS `zk_categories_category` (
  `cat_id` int(11) NOT NULL auto_increment,
  `cat_parent_id` int(11) NOT NULL default '1',
  `cat_is_locked` tinyint(4) NOT NULL default '0',
  `cat_is_leaf` tinyint(4) NOT NULL default '0',
  `cat_name` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `cat_value` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `cat_sort_value` int(11) NOT NULL default '0',
  `cat_display_name` text collate latin1_german2_ci,
  `cat_display_desc` text collate latin1_german2_ci,
  `cat_path` text collate latin1_german2_ci,
  `cat_ipath` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `cat_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `cat_obj_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `cat_cr_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `cat_cr_uid` int(11) NOT NULL default '0',
  `cat_lu_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `cat_lu_uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`cat_id`),
  KEY `idx_categories_parent` (`cat_parent_id`),
  KEY `idx_categories_is_leaf` (`cat_is_leaf`),
  KEY `idx_categories_name` (`cat_name`),
  KEY `idx_categories_ipath` (`cat_ipath`,`cat_is_leaf`,`cat_status`),
  KEY `idx_categories_status` (`cat_status`),
  KEY `idx_categories_ipath_status` (`cat_ipath`,`cat_status`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=10000 ;

--
-- Dumping data for table `zk_categories_category`
--

INSERT INTO `zk_categories_category` (`cat_id`, `cat_parent_id`, `cat_is_locked`, `cat_is_leaf`, `cat_name`, `cat_value`, `cat_sort_value`, `cat_display_name`, `cat_display_desc`, `cat_path`, `cat_ipath`, `cat_status`, `cat_obj_status`, `cat_cr_date`, `cat_cr_uid`, `cat_lu_date`, `cat_lu_uid`) VALUES
(1, 0, 1, 0, '__SYSTEM__', '', 0, 'b:0;', 'b:0;', '/__SYSTEM__', '/1', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(2, 1, 0, 0, 'Modules', '', 0, 'a:1:{s:3:"eng";s:7:"Modules";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules', '/1/2', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(3, 1, 0, 0, 'General', '', 0, 'a:1:{s:3:"eng";s:7:"General";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General', '/1/3', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(4, 3, 0, 0, 'YesNo', '', 0, 'a:1:{s:3:"eng";s:6:"Yes/No";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/YesNo', '/1/3/4', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(5, 4, 0, 1, '1 - Yes', '', 0, 'b:0;', 'b:0;', '/__SYSTEM__/General/YesNo/1 - Yes', '/1/3/4/5', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(6, 4, 0, 1, '2 - No', '', 0, 'b:0;', 'b:0;', '/__SYSTEM__/General/YesNo/2 - No', '/1/3/4/6', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(10, 3, 0, 0, 'Pending Status Extended', '', 0, 'a:1:{s:3:"eng";s:14:"Pending status";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Extended', '/1/3/10', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(11, 10, 0, 1, 'Pending', '', 0, 'a:1:{s:3:"eng";s:7:"Pending";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Extended/Pending', '/1/3/10/11', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(12, 10, 0, 1, 'Checked', '', 0, 'a:1:{s:3:"eng";s:7:"Checked";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Extended/Checked', '/1/3/10/12', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(13, 10, 0, 1, 'Approved', '', 0, 'a:1:{s:3:"eng";s:8:"Approved";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Extended/Approved', '/1/3/10/13', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(14, 10, 0, 1, 'Online', '', 0, 'a:1:{s:3:"eng";s:6:"Online";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Extended/Online', '/1/3/10/14', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(15, 10, 0, 1, 'Rejected', '', 0, 'a:1:{s:3:"eng";s:8:"Rejected";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Extended/Rejected', '/1/3/10/15', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(16, 3, 0, 0, 'Gender', '', 0, 'a:1:{s:3:"eng";s:6:"Gender";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Gender', '/1/3/16', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(17, 16, 0, 1, 'Male', '', 0, 'a:1:{s:3:"eng";s:4:"Male";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Gender/Male', '/1/3/16/17', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(18, 16, 0, 1, 'Female', '', 0, 'a:1:{s:3:"eng";s:6:"Female";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Gender/Female', '/1/3/16/18', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(19, 3, 0, 0, 'Title', '', 0, 'a:1:{s:3:"eng";s:5:"Title";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Title', '/1/3/19', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(20, 19, 0, 1, 'Dr', '', 0, 'a:1:{s:3:"eng";s:3:"Dr.";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Title/Dr', '/1/3/19/20', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(21, 19, 0, 1, 'Prof', '', 0, 'a:1:{s:3:"eng";s:5:"Prof.";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Title/Prof', '/1/3/19/21', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(22, 19, 0, 1, 'Mag', '', 0, 'a:1:{s:3:"eng";s:4:"Mag.";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Title/Mag', '/1/3/19/13', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(23, 3, 0, 0, 'ActiveStatus', '', 0, 'a:1:{s:3:"eng";s:15:"Activity status";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/ActiveStatus', '/1/3/22', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(24, 23, 0, 1, 'Active', '', 0, 'a:1:{s:3:"eng";s:6:"Active";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/ActiveStatus/Active', '/1/3/22/23', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(25, 23, 0, 1, 'Inactive', '', 0, 'a:1:{s:3:"eng";s:8:"Inactive";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/ActiveStatus/Inactive', '/1/3/22/24', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(26, 3, 0, 0, 'Pending Status Basic', '', 0, 'a:1:{s:3:"eng";s:20:"Pending status basic";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Basic', '/1/3/25', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(27, 26, 0, 1, 'Pending', '', 0, 'a:1:{s:3:"eng";s:7:"Pending";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Basic/Pending', '/1/3/25/26', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(28, 26, 0, 1, 'Approved', '', 0, 'a:1:{s:3:"eng";s:8:"Approved";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/General/Pending Status Basic/Approved', '/1/3/25/27', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(29, 1, 0, 0, 'Users', '', 0, 'a:1:{s:3:"eng";s:5:"Users";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Users', '/1/29', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(30, 2, 0, 0, 'Global', '', 0, 'a:1:{s:3:"eng";s:6:"Global";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global', '/1/2/30', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(31, 30, 0, 1, 'Arts & Entertainment', '', 0, 'a:1:{s:3:"eng";s:20:"Arts & Entertainment";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global/Arts & Entertainment', '/1/2/30/31', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(32, 30, 0, 1, 'Default', '', 0, 'a:1:{s:3:"eng";s:7:"Default";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global/Default', '/1/2/30/32', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(33, 30, 0, 1, 'Information Technology', '', 0, 'a:1:{s:3:"eng";s:22:"Information Technology";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global/Information Technology', '/1/2/30/33', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(34, 30, 0, 1, 'Politics', '', 0, 'a:1:{s:3:"eng";s:8:"Politics";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global/Politics', '/1/2/30/34', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(35, 30, 0, 1, 'Science', '', 0, 'a:1:{s:3:"eng";s:7:"Science";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global/Science', '/1/2/30/35', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(36, 30, 0, 1, 'Society', '', 0, 'a:1:{s:3:"eng";s:7:"Society";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global/Society', '/1/2/30/36', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(37, 30, 0, 1, 'Sports', '', 0, 'a:1:{s:3:"eng";s:6:"Sports";}', 'a:1:{s:3:"eng";s:0:"";}', '/__SYSTEM__/Modules/Global/Sports', '/1/2/30/37', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0);

-- --------------------------------------------------------

--
-- Table structure for table `zk_categories_mapmeta`
--

CREATE TABLE IF NOT EXISTS `zk_categories_mapmeta` (
  `cmm_id` int(11) NOT NULL auto_increment,
  `cmm_meta_id` int(11) NOT NULL default '0',
  `cmm_category_id` int(11) NOT NULL default '0',
  `cmm_obj_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `cmm_cr_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `cmm_cr_uid` int(11) NOT NULL default '0',
  `cmm_lu_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `cmm_lu_uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`cmm_id`),
  KEY `idx_categories_mapmeta` (`cmm_meta_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_categories_mapmeta`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_categories_mapobj`
--

CREATE TABLE IF NOT EXISTS `zk_categories_mapobj` (
  `cmo_id` int(11) NOT NULL auto_increment,
  `cmo_modname` varchar(60) collate latin1_german2_ci NOT NULL default '',
  `cmo_table` varchar(60) collate latin1_german2_ci NOT NULL,
  `cmo_obj_id` int(11) NOT NULL default '0',
  `cmo_obj_idcolumn` varchar(60) collate latin1_german2_ci NOT NULL default 'id',
  `cmo_reg_id` int(11) NOT NULL,
  `cmo_category_id` int(11) NOT NULL,
  `cmo_obj_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `cmo_cr_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `cmo_cr_uid` int(11) NOT NULL default '0',
  `cmo_lu_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `cmo_lu_uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`cmo_id`),
  KEY `idx_categories_mapobj` (`cmo_modname`,`cmo_table`,`cmo_obj_id`,`cmo_obj_idcolumn`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_categories_mapobj`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_categories_registry`
--

CREATE TABLE IF NOT EXISTS `zk_categories_registry` (
  `crg_id` int(11) NOT NULL auto_increment,
  `crg_modname` varchar(60) collate latin1_german2_ci NOT NULL default '',
  `crg_table` varchar(60) collate latin1_german2_ci NOT NULL default '',
  `crg_property` varchar(60) collate latin1_german2_ci NOT NULL default '',
  `crg_category_id` int(11) NOT NULL default '0',
  `crg_obj_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `crg_cr_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `crg_cr_uid` int(11) NOT NULL default '0',
  `crg_lu_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `crg_lu_uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`crg_id`),
  KEY `idx_categories_registry` (`crg_modname`,`crg_table`,`crg_property`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_categories_registry`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_groups`
--

CREATE TABLE IF NOT EXISTS `zk_groups` (
  `pn_gid` int(11) NOT NULL auto_increment,
  `pn_name` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_gtype` tinyint(4) NOT NULL default '0',
  `pn_description` varchar(200) collate latin1_german2_ci NOT NULL default '',
  `pn_prefix` varchar(25) collate latin1_german2_ci NOT NULL default '',
  `pn_state` tinyint(4) NOT NULL default '0',
  `pn_nbuser` int(11) NOT NULL default '0',
  `pn_nbumax` int(11) NOT NULL default '0',
  `pn_link` int(11) NOT NULL default '0',
  `pn_uidmaster` int(11) NOT NULL default '0',
  PRIMARY KEY  (`pn_gid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=3 ;

--
-- Dumping data for table `zk_groups`
--

INSERT INTO `zk_groups` (`pn_gid`, `pn_name`, `pn_gtype`, `pn_description`, `pn_prefix`, `pn_state`, `pn_nbuser`, `pn_nbumax`, `pn_link`, `pn_uidmaster`) VALUES
(1, 'Users', 0, 'Users default Group', 'usr', 0, 0, 0, 0, 0),
(2, 'Administrators', 0, 'Admin default Group', 'adm', 0, 0, 0, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `zk_group_applications`
--

CREATE TABLE IF NOT EXISTS `zk_group_applications` (
  `pn_app_id` int(11) NOT NULL auto_increment,
  `pn_uid` int(11) NOT NULL default '0',
  `pn_gid` int(11) NOT NULL default '0',
  `pn_application` longblob,
  `pn_status` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`pn_app_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_group_applications`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_group_membership`
--

CREATE TABLE IF NOT EXISTS `zk_group_membership` (
  `pn_gid` int(11) NOT NULL default '0',
  `pn_uid` int(11) NOT NULL default '0',
  KEY `gid_uid` (`pn_uid`,`pn_gid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;

--
-- Dumping data for table `zk_group_membership`
--

INSERT INTO `zk_group_membership` (`pn_gid`, `pn_uid`) VALUES
(1, 1),
(2, 2);

-- --------------------------------------------------------

--
-- Table structure for table `zk_group_perms`
--

CREATE TABLE IF NOT EXISTS `zk_group_perms` (
  `pn_pid` int(11) NOT NULL auto_increment,
  `pn_gid` int(11) NOT NULL default '0',
  `pn_sequence` int(11) NOT NULL default '0',
  `pn_realm` int(11) NOT NULL default '0',
  `pn_component` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_instance` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_level` int(11) NOT NULL default '0',
  `pn_bond` int(11) NOT NULL default '0',
  PRIMARY KEY  (`pn_pid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=6 ;

--
-- Dumping data for table `zk_group_perms`
--

INSERT INTO `zk_group_perms` (`pn_pid`, `pn_gid`, `pn_sequence`, `pn_realm`, `pn_component`, `pn_instance`, `pn_level`, `pn_bond`) VALUES
(1, 2, 1, 0, '.*', '.*', 800, 0),
(2, -1, 2, 0, 'ExtendedMenublock::', '1:2:', 0, 0),
(3, 1, 3, 0, '.*', '.*', 300, 0),
(4, 0, 4, 0, 'ExtendedMenublock::', '1:(1|3):', 0, 0),
(5, 0, 5, 0, '.*', '.*', 200, 0);

-- --------------------------------------------------------

--
-- Table structure for table `zk_hooks`
--

CREATE TABLE IF NOT EXISTS `zk_hooks` (
  `pn_id` int(11) NOT NULL auto_increment,
  `pn_object` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_action` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_smodule` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_stype` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_tarea` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_tmodule` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_ttype` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_tfunc` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_sequence` int(11) NOT NULL default '0',
  PRIMARY KEY  (`pn_id`),
  KEY `smodule` (`pn_smodule`),
  KEY `smodule_tmodule` (`pn_smodule`,`pn_tmodule`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_hooks`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_message`
--

CREATE TABLE IF NOT EXISTS `zk_message` (
  `pn_mid` int(11) NOT NULL auto_increment,
  `pn_title` varchar(100) collate latin1_german2_ci NOT NULL default '',
  `pn_content` longtext collate latin1_german2_ci NOT NULL,
  `pn_date` int(11) NOT NULL default '0',
  `pn_expire` int(11) NOT NULL default '0',
  `pn_active` int(11) NOT NULL default '1',
  `pn_view` int(11) NOT NULL default '1',
  `pn_language` varchar(30) collate latin1_german2_ci NOT NULL default '',
  PRIMARY KEY  (`pn_mid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=2 ;

--
-- Dumping data for table `zk_message`
--

INSERT INTO `zk_message` (`pn_mid`, `pn_title`, `pn_content`, `pn_date`, `pn_expire`, `pn_active`, `pn_view`, `pn_language`) VALUES
(1, 'Welcome to Zikula', '<p><a href="http://www.zikula.org">Zikula</a> is a weblog/content management system (CMS). It is far more secure and stable than competing products, and is able to work in high-volume environments with ease.<br /><br />\r\nSome of the key features of Zikula are:</p>\r\n<ul>\r\n<li> customization of all aspects of the web site''s appearance through themes, with support for CSS stylesheets</li>\r\n<li> the ability to specify items as being suitable for either a single language or for all languages</li>\r\n<li> the best guarantee of properly displaying your web pages in all browsers, thanks to full compliance with W3C HTML standards</li>\r\n<li> a standard API (application programming interface) and extensive documentation to allow for easy extension of your web site''s functionality through modules and blocks.</li>\r\n</ul>\r\n<p>Zikula has a very active developer and support community at <a href="http://www.zikula.org">zikula.org</a>.</p>\r\n<p>We hope you will enjoy using Zikula.<br /><br /><strong>The Zikula development team </strong></p>\r\n<p><em>Note: you can edit or remove this message by going to the Administration page and clicking on the ''Administration messages'' entry </em></p>', 1229780233, 0, 1, 1, '');

-- --------------------------------------------------------

--
-- Table structure for table `zk_modules`
--

CREATE TABLE IF NOT EXISTS `zk_modules` (
  `pn_id` int(11) NOT NULL auto_increment,
  `pn_name` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_type` tinyint(4) NOT NULL default '0',
  `pn_displayname` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_description` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_regid` int(11) NOT NULL default '0',
  `pn_directory` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_version` varchar(10) collate latin1_german2_ci NOT NULL default '0',
  `pn_official` tinyint(4) NOT NULL default '0',
  `pn_author` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_contact` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_admin_capable` tinyint(4) NOT NULL default '0',
  `pn_user_capable` tinyint(4) NOT NULL default '0',
  `pn_profile_capable` tinyint(4) NOT NULL default '0',
  `pn_message_capable` tinyint(4) NOT NULL default '0',
  `pn_state` smallint(6) NOT NULL default '0',
  `pn_credits` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_changelog` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_help` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_license` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_securityschema` text collate latin1_german2_ci,
  PRIMARY KEY  (`pn_id`),
  KEY `state` (`pn_state`),
  KEY `mod_state` (`pn_name`,`pn_state`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=46 ;

--
-- Dumping data for table `zk_modules`
--

INSERT INTO `zk_modules` (`pn_id`, `pn_name`, `pn_type`, `pn_displayname`, `pn_description`, `pn_regid`, `pn_directory`, `pn_version`, `pn_official`, `pn_author`, `pn_contact`, `pn_admin_capable`, `pn_user_capable`, `pn_profile_capable`, `pn_message_capable`, `pn_state`, `pn_credits`, `pn_changelog`, `pn_help`, `pn_license`, `pn_securityschema`) VALUES
(1, 'Modules', 3, 'Modules', 'Modules configuration', 0, 'Modules', '3.3', 1, 'Jim McDonald, Mark West', 'http://www.zikula.org', 1, 0, 0, 0, 3, '', '', '', '', 'a:1:{s:9:"Modules::";s:2:"::";}'),
(2, 'Users', 3, 'Users', 'Provides an interface for configuring and administering registered user accounts. Works in close unison with the Profile module.', 0, 'Users', '1.9', 1, 'Xiaoyu Huang, Drak', 'class007@sina.com, drak@zikula.org', 1, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:6:{s:11:"Users::user";s:14:"Uname::User ID";s:14:"Users::newuser";s:2:"::";s:19:"Users::lostpassword";s:2:"::";s:18:"Users::youraccount";s:2:"::";s:7:"Users::";s:2:"::";s:16:"Users::MailUsers";s:2:"::";}'),
(3, 'Workflow', 3, 'Workflow', 'Provides a workflow engine, and an interface for designing and administering workflows comprised of actions and events.', 0, 'Workflow', '1.0', 1, 'Drak', 'drak@hostnuke.com', 0, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/manual.html', 'pndocs/copying.txt', 'a:1:{s:10:"Workflow::";s:2:"::";}'),
(4, 'legal', 3, 'Legal', 'Provides an interface for managing the web''s Terms of Use, Privacy Statement and Accessibility Statement.', 0, 'legal', '1.2', 1, 'Michael M. Wechsler', 'michael@thelaw.com', 1, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:4:{s:7:"legal::";s:2:"::";s:17:"legal::termsofuse";s:2:"::";s:14:"legal::privacy";s:2:"::";s:29:"legal::accessibilitystatement";s:2:"::";}'),
(5, 'Header_Footer', 3, 'Header_Footer', 'Provides the header and footer portions of legacy themes and other non-Xanthia themes.', 0, 'Header_Footer', '1.0', 1, 'Mark West', 'http://www.markwest.me.uk/', 0, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:0:{}'),
(6, 'SysInfo', 3, 'SysInfo', 'Provides detailed information reports about the system configuration and environment, for tracking and troubleshooting.', 0, 'SysInfo', '1.0', 1, 'Simon Birtwistle', 'hammerhead@zikula.org', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:9:"SysInfo::";s:2:"::";}'),
(7, 'Permissions', 3, 'Permissions', 'Provides an interface for fine-grained management of accessibility of the web''s functionality and content through permission rules.', 0, 'Permissions', '1.0', 1, 'Jim McDonald, M.Maes', 'http://www.mcdee.net/, http://www.mmaes.com', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:13:"Permissions::";s:2:"::";}'),
(8, 'SecurityCenter', 3, 'SecurityCenter', 'Provides a system for tracking security via an attempted hacks and event logs database, together with an interface for controlling alerting and security settings.', 0, 'SecurityCenter', '1.1', 1, 'Mark West', 'http://www.markwest.me.uk', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:16:"SecurityCenter::";s:16:"hackid::hacktime";}'),
(9, 'pnRender', 3, 'pnRender', 'Provides the core system with a Smarty-based engine to control content rendering and presentation.', 0, 'pnRender', '1.0', 1, 'The Zikula development team', 'http://www.zikula.org/', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:10:"pnRender::";s:2:"::";}'),
(10, 'ObjectData', 3, 'ObjectData', 'Provides a framework for Object Model Data as an underlying technology in module and application design.', 0, 'ObjectData', '1.01', 0, 'Robert Gasch', 'rgasch@gmail.com', 0, 0, 0, 0, 3, 'docs/credits.txt', 'docs/changelog.txt', 'docs/help.txt', 'docs/license.txt', 'a:1:{s:12:"ObjectData::";s:2:"::";}'),
(11, 'Theme', 3, 'Theme', 'Provides the site''s theming system, and an interface for managing themes, to control the site''s presentation and appearance.', 0, 'Theme', '3.1', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:7:"Theme::";s:12:"Theme name::";}'),
(12, 'Groups', 3, 'Groups', 'Provides an interface for adding, removing and administering user groups.', 0, 'Groups', '2.1', 1, 'Mark West,Franky Chestnut,Michael Halbook', 'http://www.markwest.me.uk/,http://dev.pnconcept.com,http://www.halbrooktech.com', 1, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:8:"Groups::";s:20:"Group Name::Group ID";}'),
(13, 'Admin_Messages', 3, 'Admin_Messages', 'Provides a system for displaying automated messages from the administrator in the site''s pages, and an interface for managing and scheduling them.', 0, 'Admin_Messages', '2.0', 1, 'Mark West', 'http://www.markwest.me.uk', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:16:"Admin_Messages::";s:25:"message title::message id";}'),
(14, 'Categories', 3, 'Categories', 'Provides support for categories, as a concept used by other modules, and an interface for adding, removing and administering categories.', 0, 'Categories', '1.04', 1, 'Robert Gasch', 'rgasch@gmail.com', 1, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:20:"Categories::Category";s:40:"Category ID:Category Path:Category IPath";}'),
(15, 'Mailer', 3, 'Mailer', 'Provides the site''s mailer, and an interface for managing the e-mail service settings used by the mailer.', 0, 'Mailer', '1.2', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:8:"Mailer::";s:2:"::";}'),
(16, 'Admin', 3, 'Admin', 'Provides the site''s Admin Panel, and the ability to configure and manage it.', 0, 'Admin', '1.5', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:7:"Admin::";s:38:"Admin Category name::Admin Category ID";}'),
(17, 'Settings', 3, 'Settings', 'Provides an interface for managing the site''s global settings, i.e. ownership info, HTML settings, multilingual settings, general security options, and various other features.', 0, 'Settings', '2.5', 1, 'Simon Wunderlin', '', 1, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:10:"Settings::";s:2:"::";}'),
(18, 'pnForm', 3, 'pnForm', 'Provides a framework for standardized presentation of the site''s forms.', 0, 'pnForm', '1.0', 1, 'The Zikula development team', 'http://www.zikula.org/', 0, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:10:"pnRender::";s:2:"::";}'),
(19, 'Blocks', 3, 'Blocks', 'Provides an interface for adding, removing and administering the site''s side and center blocks.', 0, 'Blocks', '3.2', 1, 'Jim McDonald, Mark West', 'http://www.mcdee.net/, http://www.markwest.me.uk/', 1, 1, 0, 0, 3, '', '', '', '', 'a:2:{s:8:"Blocks::";s:30:"Block key:Block title:Block ID";s:16:"Blocks::position";s:26:"Position name::Position ID";}'),
(20, 'Search', 3, 'Search', 'Provides an engine for searching within the site, and an interface for managing search page settings.', 0, 'Search', '1.4', 1, 'Patrick Kellum', 'http://www.ctarl-ctarl.com', 1, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:8:"Search::";s:13:"Module name::";}'),
(21, 'Errors', 3, 'Errors', 'Provides the core system of the site with error-logging capability.', 0, 'Errors', '1.0', 1, 'Brian Lindner <Furbo>', 'furbo@sigtauonline.com', 0, 1, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:8:"Errors::";s:2:"::";}'),
(22, 'AuthPN', 3, 'AuthPN', 'Provides the ability to use custom user authentication systems within a site, notably LDAP or OpenID.', 0, 'AuthPN', '1.0', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 0, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:0:{}'),
(23, 'PageLock', 3, 'PageLock', 'Provides the ability to lock pages when they are in use, for content and access control.', 0, 'PageLock', '1.0', 1, 'Jorn Wildt', 'http://www.elfisk.dk', 0, 0, 0, 0, 3, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:10:"PageLock::";s:2:"::";}'),
(24, 'Topics', 1, 'Topics', 'Display site topics.', 0, 'Topics', '1.0', 1, 'Francisco Burzi', 'http://www.phpnuke.org', 1, 0, 0, 0, 1, 'docs/credits.txt', 'docs/changelog.txt', 'docs/install.txt', 'docs/license.txt', 'a:2:{s:13:"Topics::Topic";s:20:"Topic name::Topic ID";s:15:"Topics::Related";s:32:"Related name:Topic name:Topic ID";}'),
(25, 'Quotes', 2, 'Random Quote', 'Random quotes', 0, 'Quotes', '2.2', 1, 'The Zikula Development Team', 'http://www.zikula.org', 1, 0, 0, 0, 1, 'pndocs/credits.txt', '', '', '', 'a:1:{s:8:"Quotes::";s:21:"Author name::Quote ID";}'),
(26, 'PendingContent', 2, 'Pending Content', 'Display any unapproved content items.', 0, 'PendingContent', '1.0', 1, 'Hartmut Koenig', 'http://www.snakelab.de/', 1, 0, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:16:"pendingcontent::";s:2:"::";}'),
(27, 'Pages', 2, 'Pages', 'Displays static pages', 0, 'Pages', '2.3', 1, 'The Zikula Development Team', 'http://www.zikula.org/', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:11:"Pages::Page";s:18:"Page name::Page ID";}'),
(28, 'Banners', 2, 'Banners', 'Provides support for banner advertisements and advertising clients, and provides an interface for adding, removing and administering banner ads and clients.', 0, 'Banners', '2.1', 1, 'Devin Hayes', 'http://www.zikula.org/', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:2:{s:9:"Banners::";s:11:"Banner ID::";s:15:"Banners::Client";s:11:"Client ID::";}'),
(29, 'Stats', 2, 'Stats', 'Display site statistics.', 0, 'Stats', '2.3', 1, 'Mark West', 'http://www.markwest.me.uk', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:7:"Stats::";s:2:"::";}'),
(30, 'Multisites', 2, 'MultiSites', 'Run multiple sites from single installation', 0, 'Multisites', '1.0', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 0, 0, 0, 1, '', '', '', '', 'a:1:{s:12:"MultiSites::";s:40:"MultiSites item name::MultiSites item ID";}'),
(31, 'Thumbnail', 2, 'Thumbnail', 'Provides thumbnail generation facilities via userapi functions.', 0, 'Thumbnail', '1.1', 0, 'Robert Gasch', 'rgasch@gmail.com', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:11:"Thumbnail::";s:2:"::";}'),
(32, 'FAQ', 2, 'FAQ', 'Frequently Asked Questions', 0, 'FAQ', '2.2', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:5:"FAQ::";s:8:"FAQ ID::";}'),
(33, 'Ratings', 2, 'Ratings', 'Rate Zikula items.', 0, 'Ratings', '2.0', 1, 'Jim McDonald', 'http://www.mcdee.net/', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:9:"Ratings::";s:31:"Module name:Rating type:Item ID";}'),
(34, 'bbcode', 2, 'bbcode', 'BBCode Hook', 164, 'bbcode', '2.0', 0, 'pnForum team', 'http://www.pnforum.de', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:2:{s:23:"bbcode:Modulename:Links";s:2:"::";s:24:"bbcode:Modulename:Emails";s:2:"::";}'),
(35, 'AuthLDAP', 2, 'AuthLDAP', 'LDAP authentication', 0, 'AuthLDAP', '0.4.0.0', 1, 'Mike Goldfinger', 'MikeGoldfinger@linuxmail.org', 1, 0, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', ''),
(36, 'Ephemerids', 2, 'Ephemerids', 'A ''This day in history'' module.', 0, 'Ephemerids', '1.6', 1, 'Mark West', 'http://www.markwest.me.uk', 1, 0, 0, 0, 1, '', '', '', '', 'a:1:{s:12:"Ephemerids::";s:23:"Ephemerid::Ephemerid ID";}'),
(37, 'Reviews', 2, 'Reviews', 'Reviews System Module', 0, 'Reviews', '2.3', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:9:"Reviews::";s:22:"Review name::Review ID";}'),
(38, 'Profile', 2, 'Profile', 'Provides a personal account control panel for each registered user, an interface to administer the information items (controls) displayed in it, and a members list functionality. Works in close unison with the Users module.', 0, 'Profile', '1.2', 1, 'Mark West, Franky Chestnut', 'http://www.markwest.me.uk/, http://dev.pnconcept.com/', 1, 1, 1, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:4:{s:13:"Profile::item";s:62:"Dynamic User Data Property Name::Dynamic User Data Property ID";s:17:"Profile:Members::";s:2:"::";s:22:"Profile:Members:recent";s:2:"::";s:22:"Profile:Members:online";s:2:"::";}'),
(39, 'Credits', 2, 'Credits', 'Display Module credits, license, help and contact information', 0, 'Credits', '1.2', 1, 'Rob Brandt', 'http://bronto.csd-bes.net', 0, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:9:"Credits::";s:2:"::";}'),
(40, 'blank', 2, 'blank', 'Provides a blank, generic module to use as a template for designing and developing new modules.', 0, 'blank', '1.0', 1, 'Zikula Development Team', 'http://www.zikula.org', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:7:"blank::";s:7:"blank::";}'),
(41, 'Recommend_Us', 2, 'Recommend Us', 'Recommend site/Send articles Module.', 0, 'Recommend_Us', '1.1', 1, 'Mark West', 'http://www.markwest.me.uk/', 0, 1, 0, 0, 1, 'pndocs/credits.txt', '', '', '', 'a:1:{s:14:"Recommend us::";s:2:"::";}'),
(42, 'InterCom', 2, 'InterCom', 'Enhanced Private Message System', 0, 'InterCom', '2.1', 0, 'InterCom Development Team', 'http://code.zikula.org/intercom/', 1, 1, 0, 1, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:10:"InterCom::";s:2:"::";}'),
(43, 'News', 2, 'News', 'Provides the ability to add, remove and administer news articles published on the News Front Page.', 0, 'News', '2.2', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/install.txt', 'pndocs/license.txt', 'a:1:{s:14:"Stories::Story";s:19:"Author ID::Story ID";}'),
(44, 'Feeds', 2, 'Feeds', 'News Feed Reader.', 0, 'Feeds', '2.1', 1, 'Mark West', 'http://www.markwest.me.uk/', 1, 1, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/changelog.txt', 'pndocs/help.txt', 'pndocs/license.txt', 'a:1:{s:11:"Feeds::Item";s:28:"Feed item name::Feed item ID";}'),
(45, 'Sniffer', 2, 'Sniffer', 'Browser detection and information.', 0, 'Sniffer', '1.1', 1, 'Mark West', 'http://www.zikula.org/', 1, 0, 0, 0, 1, 'pndocs/credits.txt', 'pndocs/CHANGES', '', 'pndocs/LICENSE', 'a:1:{s:9:"Sniffer::";s:2:"::";}');

-- --------------------------------------------------------

--
-- Table structure for table `zk_module_deps`
--

CREATE TABLE IF NOT EXISTS `zk_module_deps` (
  `pn_id` int(11) NOT NULL auto_increment,
  `pn_modid` int(11) NOT NULL default '0',
  `pn_modname` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_minversion` varchar(10) collate latin1_german2_ci NOT NULL default '',
  `pn_maxversion` varchar(10) collate latin1_german2_ci NOT NULL default '',
  `pn_status` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`pn_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=7 ;

--
-- Dumping data for table `zk_module_deps`
--

INSERT INTO `zk_module_deps` (`pn_id`, `pn_modid`, `pn_modname`, `pn_minversion`, `pn_maxversion`, `pn_status`) VALUES
(1, 14, 'ObjectData', '1.0', '', 1),
(2, 29, 'Sniffer', '1.1', '', 1),
(3, 35, 'AuthPN', '1.0', '', 1),
(4, 42, 'ContactList', '1.0', '', 2),
(5, 42, 'bbcode', '2.0', '', 2),
(6, 42, 'bbsmile', '2.1', '', 2);

-- --------------------------------------------------------

--
-- Table structure for table `zk_module_vars`
--

CREATE TABLE IF NOT EXISTS `zk_module_vars` (
  `pn_id` int(11) NOT NULL auto_increment,
  `pn_modname` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_name` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_value` longtext collate latin1_german2_ci,
  PRIMARY KEY  (`pn_id`),
  KEY `mod_var` (`pn_modname`,`pn_name`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=184 ;

--
-- Dumping data for table `zk_module_vars`
--

INSERT INTO `zk_module_vars` (`pn_id`, `pn_modname`, `pn_name`, `pn_value`) VALUES
(1, 'Modules', 'itemsperpage', 'i:25;'),
(2, '/PNConfig', 'loadlegacy', 'i:0;'),
(3, 'Permissions', 'filter', 'i:1;'),
(4, 'Permissions', 'warnbar', 'i:1;'),
(5, 'Permissions', 'rowview', 'i:20;'),
(6, 'Permissions', 'rowedit', 'i:20;'),
(7, 'Permissions', 'lockadmin', 'i:1;'),
(8, 'Permissions', 'adminid', 'i:1;'),
(9, 'Groups', 'itemsperpage', 'i:25;'),
(10, 'Groups', 'defaultgroup', 's:5:"Users";'),
(11, 'Groups', 'mailwarning', 'i:0;'),
(12, 'Groups', 'hideclosed', 'i:0;'),
(13, 'Blocks', 'collapseable', 'i:0;'),
(14, 'Users', 'itemsperpage', 'i:25;'),
(15, 'Users', 'reg_allowreg', 'i:1;'),
(16, 'Users', 'reg_verifyemail', 'i:1;'),
(17, 'Users', 'reg_Illegalusername', 's:87:"root adm linux webmaster admin god administrator administrador nobody anonymous anonimo";'),
(18, 'Users', 'reg_Illegaldomains', 's:0:"";'),
(19, 'Users', 'reg_Illegaluseragents', 's:0:"";'),
(20, 'Users', 'reg_noregreasons', 's:59:"Sorry! New user account registration is currently disabled.";'),
(21, 'Users', 'reg_uniemail', 'i:1;'),
(22, 'Users', 'reg_notifyemail', 's:0:"";'),
(23, 'Users', 'reg_optitems', 'i:0;'),
(24, 'Users', 'userimg', 's:11:"images/menu";'),
(25, 'Users', 'minage', 'i:13;'),
(26, 'Users', 'minpass', 'i:5;'),
(27, 'Users', 'anonymous', 's:5:"Guest";'),
(28, 'Users', 'savelastlogindate', 'i:0;'),
(29, 'Users', 'loginviaoption', 'i:0;'),
(30, 'Users', 'moderation', 'i:0;'),
(31, 'Users', 'hash_method', 's:6:"sha256";'),
(32, 'Users', 'login_redirect', 'i:1;'),
(33, 'Users', 'reg_question', 's:0:"";'),
(34, 'Users', 'reg_answer', 's:0:"";'),
(35, 'Users', 'idnnames', 'i:1;'),
(36, 'Theme', 'modulesnocache', 's:0:"";'),
(37, 'Theme', 'enablecache', 'b:0;'),
(38, 'Theme', 'compile_check', 'b:1;'),
(39, 'Theme', 'cache_lifetime', 'i:3600;'),
(40, 'Theme', 'force_compile', 'b:0;'),
(41, 'Theme', 'trimwhitespace', 'b:0;'),
(42, 'Theme', 'makelinks', 'b:0;'),
(43, 'Theme', 'maxsizeforlinks', 'i:30;'),
(44, 'Theme', 'itemsperpage', 'i:25;'),
(45, 'Admin', 'modulesperrow', 'i:5;'),
(46, 'Admin', 'itemsperpage', 'i:25;'),
(47, 'Admin', 'defaultcategory', 'i:5;'),
(48, 'Admin', 'modulestylesheet', 's:11:"navtabs.css";'),
(49, 'Admin', 'admingraphic', 'i:1;'),
(50, 'Admin', 'startcategory', 'i:1;'),
(51, 'Admin', 'ignoreinstallercheck', 'i:0;'),
(52, 'Admin', 'admintheme', 's:0:"";'),
(53, 'Admin', 'displaynametype', 'i:1;'),
(54, '/PNConfig', 'debug', 's:1:"0";'),
(55, '/PNConfig', 'sitename', 's:9:"Site Name";'),
(56, '/PNConfig', 'site_logo', 's:8:"logo.gif";'),
(57, '/PNConfig', 'slogan', 's:16:"Site description";'),
(58, '/PNConfig', 'metakeywords', 's:253:"zikula, community, portal, portal web, open source, gpl, web site, website, weblog, blog, content management, content management system, web content management, web content management system, enterprise web content management, cms, application framework";'),
(59, '/PNConfig', 'dyn_keywords', 's:1:"0";'),
(60, '/PNConfig', 'startdate', 's:7:"12/2008";'),
(61, '/PNConfig', 'adminmail', 's:17:"admin@example.com";'),
(62, '/PNConfig', 'Default_Theme', 's:9:"andreas08";'),
(63, '/PNConfig', 'anonymous', 's:9:"Anonymous";'),
(64, '/PNConfig', 'timezone_offset', 's:1:"0";'),
(65, '/PNConfig', 'timezone_server', 's:1:"0";'),
(66, '/PNConfig', 'funtext', 's:1:"1";'),
(67, '/PNConfig', 'reportlevel', 's:1:"0";'),
(68, '/PNConfig', 'startpage', 's:6:"Search";'),
(69, '/PNConfig', 'backend_title', 's:19:"Zikula-powered site";'),
(70, '/PNConfig', 'backend_language', 's:5:"en-us";'),
(71, '/PNConfig', 'seclevel', 's:6:"Medium";'),
(72, '/PNConfig', 'secmeddays', 's:1:"7";'),
(73, '/PNConfig', 'secinactivemins', 's:2:"20";'),
(74, '/PNConfig', 'Version_Num', 's:5:"1.1.1";'),
(75, '/PNConfig', 'Version_ID', 's:6:"Zikula";'),
(76, '/PNConfig', 'Version_Sub', 's:9:"adam_baum";'),
(77, '/PNConfig', 'debug_sql', 's:1:"0";'),
(78, '/PNConfig', 'language', 's:3:"eng";'),
(79, '/PNConfig', 'locale', 's:5:"en_US";'),
(80, '/PNConfig', 'multilingual', 's:1:"0";'),
(81, '/PNConfig', 'useflags', 's:1:"0";'),
(82, '/PNConfig', 'AllowableHTML', 'a:83:{s:3:"!--";s:1:"2";s:1:"a";s:1:"2";s:4:"abbr";s:1:"0";s:7:"acronym";s:1:"0";s:7:"address";s:1:"0";s:6:"applet";s:1:"0";s:4:"area";s:1:"0";s:1:"b";s:1:"2";s:4:"base";s:1:"0";s:8:"basefont";s:1:"0";s:3:"bdo";s:1:"0";s:3:"big";s:1:"0";s:10:"blockquote";s:1:"2";s:2:"br";s:1:"2";s:6:"button";s:1:"0";s:7:"caption";s:1:"0";s:6:"center";s:1:"2";s:4:"cite";s:1:"0";s:4:"code";s:1:"0";s:3:"col";s:1:"0";s:8:"colgroup";s:1:"0";s:3:"del";s:1:"0";s:3:"dfn";s:1:"0";s:3:"dir";s:1:"0";s:3:"div";s:1:"2";s:2:"dl";s:1:"0";s:2:"dd";s:1:"0";s:2:"dt";s:1:"0";s:2:"em";s:1:"2";s:5:"embed";s:1:"0";s:8:"fieldset";s:1:"0";s:4:"font";i:0;s:4:"form";s:1:"0";s:2:"h1";s:1:"0";s:2:"h2";s:1:"0";s:2:"h3";s:1:"0";s:2:"h4";s:1:"0";s:2:"h5";s:1:"0";s:2:"h6";s:1:"0";s:2:"hr";s:1:"2";s:1:"i";s:1:"2";s:6:"iframe";s:1:"0";s:3:"img";i:0;s:5:"input";s:1:"0";s:3:"ins";s:1:"0";s:3:"kbd";s:1:"0";s:5:"label";s:1:"0";s:6:"legend";s:1:"0";s:2:"li";s:1:"2";s:3:"map";s:1:"0";s:7:"marquee";i:0;s:4:"menu";s:1:"0";s:4:"nobr";s:1:"0";s:6:"object";s:1:"0";s:2:"ol";s:1:"2";s:8:"optgroup";s:1:"0";s:6:"option";s:1:"0";s:1:"p";s:1:"2";s:5:"param";s:1:"0";s:3:"pre";s:1:"2";s:1:"q";s:1:"0";s:1:"s";s:1:"0";s:4:"samp";s:1:"0";s:6:"script";s:1:"0";s:6:"select";s:1:"0";s:5:"small";s:1:"0";s:4:"span";i:0;s:6:"strike";s:1:"0";s:6:"strong";s:1:"2";s:3:"sub";s:1:"0";s:3:"sup";s:1:"0";s:5:"table";s:1:"2";s:5:"tbody";s:1:"0";s:2:"td";s:1:"2";s:8:"textarea";s:1:"0";s:5:"tfoot";s:1:"0";s:2:"th";s:1:"2";s:5:"thead";s:1:"0";s:2:"tr";s:1:"2";s:2:"tt";s:1:"2";s:1:"u";s:1:"0";s:2:"ul";s:1:"2";s:3:"var";s:1:"0";}'),
(83, '/PNConfig', 'theme_change', 's:1:"0";'),
(84, '/PNConfig', 'htmlentities', 's:1:"1";'),
(85, '/PNConfig', 'UseCompression', 's:1:"0";'),
(86, '/PNConfig', 'refereronprint', 's:1:"0";'),
(87, '/PNConfig', 'storyorder', 's:1:"0";'),
(88, '/PNConfig', 'timezone_info', 'a:38:{i:-12;s:31:"(GMT -12:00 hours) Baker Island";i:-11;s:39:"(GMT -11:00 hours) Midway Island, Samoa";i:-10;s:25:"(GMT -10:00 hours) Hawaii";s:4:"-9.5";s:34:"(GMT -9:30 hours) French Polynesia";i:-9;s:24:"(GMT -9:00 hours) Alaska";i:-8;s:44:"(GMT -8:00 hours) Pacific Time (US & Canada)";i:-7;s:45:"(GMT -7:00 hours) Mountain Time (US & Canada)";i:-6;s:57:"(GMT -6:00 hours) Central Time (US & Canada), Mexico City";i:-5;s:65:"(GMT -5:00 hours) Eastern Time (US & Canada), Bogota, Lima, Quito";i:-4;s:57:"(GMT -4:00 hours) Atlantic Time (Canada), Caracas, La Paz";s:4:"-3.5";s:30:"(GMT -3:30 hours) Newfoundland";i:-3;s:50:"(GMT -3:00 hours) Brazil, Buenos Aires, Georgetown";i:-2;s:30:"(GMT -2:00 hours) Mid-Atlantic";i:-1;s:44:"(GMT -1:00 hours) Azores, Cape Verde Islands";i:0;s:63:"(GMT) Western Europe Time, London, Lisbon, Casablanca, Monrovia";i:1;s:79:"(GMT +1:00 hours) CET(Central Europe Time), Brussels, Copenhagen, Madrid, Paris";i:2;s:69:"(GMT +2:00 hours) EET(Eastern Europe Time), Kaliningrad, South Africa";i:3;s:65:"(GMT +3:00 hours) Baghdad, Kuwait, Riyadh, Moscow, St. Petersburg";s:3:"3.5";s:24:"(GMT +3:30 hours) Tehran";i:4;s:50:"(GMT +4:00 hours) Abu Dhabi, Muscat, Baku, Tbilisi";s:3:"4.5";s:23:"(GMT +4:30 hours) Kabul";i:5;s:60:"(GMT +5:00 hours) Ekaterinburg, Islamabad, Karachi, Tashkent";s:3:"5.5";s:53:"(GMT +5:30 hours) Bombay, Calcutta, Madras, New Delhi";s:4:"5.75";s:27:"(GMT +5:45 hours) Kathmandu";i:6;s:40:"(GMT +6:00 hours) Almaty, Dhaka, Colombo";s:3:"6.5";s:40:"(GMT +6:30 hours) Cocos Islands, Myanmar";i:7;s:41:"(GMT +7:00 hours) Bangkok, Hanoi, Jakarta";i:8;s:81:"(GMT +8:00 hours) Beijing, Perth, Singapore, Hong Kong, Chongqing, Urumqi, Taipei";i:9;s:55:"(GMT +9:00 hours) Tokyo, Seoul, Osaka, Sapporo, Yakutsk";s:3:"9.5";s:34:"(GMT +9:30 hours) Adelaide, Darwin";i:10;s:49:"(GMT +10:00 hours) EAST(East Australian Standard)";s:4:"10.5";s:52:"(GMT +10:30 hours) Lord Howe Island (NSW, Australia)";i:11;s:58:"(GMT +11:00 hours) Magadan, Solomon Islands, New Caledonia";s:4:"11.5";s:33:"(GMT +11:30 hours) Norfolk Island";i:12;s:73:"(GMT +12:00 hours) Auckland, Wellington, Fiji, Kamchatka, Marshall Island";s:5:"12.75";s:34:"(GMT +12:45 hours) Chatham Islands";i:13;s:51:"(GMT +13:00 hours Tonga, Kiribati (Phoenix Islands)";i:14;s:42:"(GMT +14:00 hours) Kiribati (Line Islands)";}'),
(89, '/PNConfig', 'errordisplay', 'i:1;'),
(90, '/PNConfig', 'errorlog', 'i:0;'),
(91, '/PNConfig', 'errorlogtype', 'i:0;'),
(92, '/PNConfig', 'errormailto', 's:14:"me@example.com";'),
(93, '/PNConfig', 'siteoff', 'i:0;'),
(94, '/PNConfig', 'siteoffreason', 's:0:"";'),
(95, '/PNConfig', 'starttype', 's:0:"";'),
(96, '/PNConfig', 'startfunc', 's:0:"";'),
(97, '/PNConfig', 'startargs', 's:0:"";'),
(98, '/PNConfig', 'entrypoint', 's:9:"index.php";'),
(99, '/PNConfig', 'secure_domain', 's:0:"";'),
(100, '/PNConfig', 'language_detect', 'i:1;'),
(101, '/PNConfig', 'shorturls', 'b:0;'),
(102, '/PNConfig', 'shorturlstype', 's:1:"0";'),
(103, '/PNConfig', 'shorturlsext', 's:4:"html";'),
(104, '/PNConfig', 'shorturlsseparator', 's:1:"-";'),
(105, '/PNConfig', 'shorturlsstripentrypoint', 'b:0;'),
(106, '/PNConfig', 'shorturlsdefaultmodule', 's:0:"";'),
(107, '/PNConfig', 'signcookies', 'i:1;'),
(108, '/PNConfig', 'signingkey', 's:40:"ace4b441a4782b6e3a4b467942a23ff329d1d477";'),
(109, '/PNConfig', 'sessionipcheck', 'i:0;'),
(110, '/PNConfig', 'keyexpiry', 'i:0;'),
(111, '/PNConfig', 'gc_probability', 'i:100;'),
(112, '/PNConfig', 'anonymoussessions', 'i:1;'),
(113, '/PNConfig', 'sessionstoretofile', 'i:0;'),
(114, '/PNConfig', 'sessionsavepath', 's:0:"";'),
(115, '/PNConfig', 'sessionauthkeyua', 'b:0;'),
(116, '/PNConfig', 'sessionname', 's:5:"ZKSID";'),
(117, '/PNConfig', 'sessionregenerate', 'b:1;'),
(118, '/PNConfig', 'sessionrandregenerate', 'b:1;'),
(119, '/PNConfig', 'sessionregeneratefreq', 'i:10;'),
(120, '/PNConfig', 'jsquicktags', 'b:1;'),
(121, '/PNConfig', 'profilemodule', 's:0:"";'),
(122, '/PNConfig', 'messagemodule', 's:0:"";'),
(123, 'Admin_Messages', 'itemsperpage', 'i:25;'),
(124, 'Admin_Messages', 'allowsearchinactive', 'b:0;'),
(125, 'SecurityCenter', 'itemsperpage', 'i:10;'),
(126, '/PNConfig', 'enableanticracker', 'i:1;'),
(127, '/PNConfig', 'emailhackattempt', 'i:1;'),
(128, '/PNConfig', 'loghackattempttodb', 'i:1;'),
(129, '/PNConfig', 'onlysendsummarybyemail', 'i:1;'),
(130, '/PNConfig', 'usehtaccessbans', 'i:0;'),
(131, '/PNConfig', 'filtergetvars', 'i:1;'),
(132, '/PNConfig', 'filtercookievars', 'i:1;'),
(133, '/PNConfig', 'filterpostvars', 'i:1;'),
(134, '/PNConfig', 'filterarrays', 'i:1;'),
(135, '/PNConfig', 'extrapostprotection', 'i:0;'),
(136, '/PNConfig', 'extragetprotection', 'i:0;'),
(137, '/PNConfig', 'checkmultipost', 'i:0;'),
(138, '/PNConfig', 'maxmultipost', 'i:4;'),
(139, '/PNConfig', 'zipcompress', 'i:0;'),
(140, '/PNConfig', 'compresslevel', 'i:9;'),
(141, '/PNConfig', 'cpuloadmonitor', 'i:0;'),
(142, '/PNConfig', 'cpumaxload', 'd:10;'),
(143, '/PNConfig', 'ccisessionpath', 's:0:"";'),
(144, '/PNConfig', 'htaccessfilelocation', 's:9:".htaccess";'),
(145, '/PNConfig', 'nocookiebanthreshold', 'i:10;'),
(146, '/PNConfig', 'nocookiewarningthreshold', 'i:2;'),
(147, '/PNConfig', 'fastaccessbanthreshold', 'i:40;'),
(148, '/PNConfig', 'fastaccesswarnthreshold', 'i:10;'),
(149, '/PNConfig', 'javababble', 'i:0;'),
(150, '/PNConfig', 'javaencrypt', 'i:0;'),
(151, '/PNConfig', 'preservehead', 'i:0;'),
(152, '/PNConfig', 'outputfilter', 'i:1;'),
(153, '/PNConfig', 'summarycontent', 's:1181:"Attention site admin of %sitename%.\r\n\r\nOn %date% at %time% the Zikula code has detected that somebody tried to send information to your site that may have been intended as a hack. Do not panic, it may be harmless: maybe this detection was triggered by something you did! Anyway, it was detected and blocked. \r\n\r\nThe suspicious activity was recognized in %filename% on line %linenumber%, and is of the type %type%. \r\n\r\nAdditional information given by the code which detected this: %additionalinfo%.\r\n\r\nBelow you will find a lot of information obtained about this attempt, that may help you to find  what happened and maybe who did it.\r\n\r\n\r\n=====================================\r\nInformation about this user:\r\n=====================================\r\nZikula username:  %username%\r\nRegistered email of this Zikula user: %useremail%\r\nRegistered real name of this Zikula user: %userrealname%\r\nIP numbers: (note: when you are dealing with a real cracker these IP numbers might not be from the actual computer he is working on)\r\n\r\nIP according to HTTP_CLIENT_IP: %httpclientip%\r\nIP according to REMOTE_ADDR: %remoteaddr%\r\nIP according to GetHostByName($REMOTE_ADDR): %gethostbyremoteaddr%\r\n";'),
(154, '/PNConfig', 'fullcontent', 's:1363:"=====================================\r\nInformation in the $_REQUEST array\r\n=====================================\r\n%requestarray%\r\n\r\n=====================================\r\nInformation in the $_GET array\r\nThis is about variables that may have been in the URL string or in a ''GET'' type form.\r\n=====================================\r\n%getarray%\r\n\r\n=====================================\r\nInformation in the $_POST array\r\nThis is about visible and invisible form elements.\r\n=====================================\r\n%postarray%\r\n\r\n=====================================\r\nBrowser information\r\n=====================================\r\n%browserinfo%\r\n\r\n=====================================\r\nInformation in the $_SERVER array\r\n=====================================\r\n%serverarray%\r\n\r\n=====================================\r\nInformation in the $_ENV array\r\n=====================================\r\n%envarray%\r\n\r\n=====================================\r\nInformation in the $_COOKIE array\r\n=====================================\r\n%cookiearray%\r\n\r\n=====================================\r\nInformation in the $_FILES array\r\n=====================================\r\n%filearray%\r\n\r\n=====================================\r\nInformation in the $_SESSION array\r\nThis is session info. The variables\r\nstarting with PNSV are Zikula Session Variables.\r\n=====================================\r\n%sessionarray%";'),
(155, 'Categories', 'userrootcat', 's:17:"/__SYSTEM__/Users";'),
(156, 'Categories', 'allowusercatedit', 'i:0;'),
(157, 'Categories', 'autocreateusercat', 'i:0;'),
(158, 'Categories', 'autocreateuserdefaultcat', 'i:0;'),
(159, 'Categories', 'userdefaultcatname', 's:7:"Default";'),
(160, 'legal', 'termsofuse', 'b:1;'),
(161, 'legal', 'privacypolicy', 'b:1;'),
(162, 'legal', 'accessibilitystatement', 'b:1;'),
(163, 'Mailer', 'mailertype', 'i:1;'),
(164, 'Mailer', 'charset', 's:10:"ISO-8859-1";'),
(165, 'Mailer', 'encoding', 's:4:"8bit";'),
(166, 'Mailer', 'html', 'b:0;'),
(167, 'Mailer', 'wordwrap', 'i:50;'),
(168, 'Mailer', 'msmailheaders', 'b:0;'),
(169, 'Mailer', 'sendmailpath', 's:18:"/usr/sbin/sendmail";'),
(170, 'Mailer', 'smtpauth', 'b:0;'),
(171, 'Mailer', 'smtpserver', 's:9:"localhost";'),
(172, 'Mailer', 'smtpport', 'i:25;'),
(173, 'Mailer', 'smtptimeout', 'i:10;'),
(174, 'Mailer', 'smtpusername', 's:0:"";'),
(175, 'Mailer', 'smtppassword', 's:0:"";'),
(176, 'pnRender', 'compile_check', 'b:1;'),
(177, 'pnRender', 'force_compile', 'b:0;'),
(178, 'pnRender', 'cache', 'b:0;'),
(179, 'pnRender', 'expose_template', 'b:0;'),
(180, 'pnRender', 'lifetime', 'i:3600;'),
(181, 'Search', 'itemsperpage', 'i:10;'),
(182, 'Search', 'limitsummary', 'i:255;'),
(183, '/PNConfig', 'log_last_rotate', 'i:1229780294;');

-- --------------------------------------------------------

--
-- Table structure for table `zk_objectdata_attributes`
--

CREATE TABLE IF NOT EXISTS `zk_objectdata_attributes` (
  `oba_id` int(11) NOT NULL auto_increment,
  `oba_attribute_name` varchar(80) collate latin1_german2_ci NOT NULL default '',
  `oba_object_id` int(11) NOT NULL default '0',
  `oba_object_type` varchar(80) collate latin1_german2_ci NOT NULL default '',
  `oba_value` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `oba_obj_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `oba_cr_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `oba_cr_uid` int(11) NOT NULL default '0',
  `oba_lu_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `oba_lu_uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`oba_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=14 ;

--
-- Dumping data for table `zk_objectdata_attributes`
--

INSERT INTO `zk_objectdata_attributes` (`oba_id`, `oba_attribute_name`, `oba_object_id`, `oba_object_type`, `oba_value`, `oba_obj_status`, `oba_cr_date`, `oba_cr_uid`, `oba_lu_date`, `oba_lu_uid`) VALUES
(1, 'code', 5, 'categories_category', 'Y', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(2, 'code', 6, 'categories_category', 'N', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(3, 'code', 11, 'categories_category', 'P', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(4, 'code', 12, 'categories_category', 'C', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(5, 'code', 13, 'categories_category', 'A', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(6, 'code', 14, 'categories_category', 'O', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(7, 'code', 15, 'categories_category', 'R', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(8, 'code', 17, 'categories_category', 'M', 'A', '2008-12-20 14:37:14', 0, '2008-12-20 14:37:14', 0),
(9, 'code', 18, 'categories_category', 'F', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(10, 'code', 24, 'categories_category', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(11, 'code', 25, 'categories_category', 'I', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(12, 'code', 27, 'categories_category', 'P', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0),
(13, 'code', 28, 'categories_category', 'A', 'A', '2008-12-20 14:37:15', 0, '2008-12-20 14:37:15', 0);

-- --------------------------------------------------------

--
-- Table structure for table `zk_objectdata_log`
--

CREATE TABLE IF NOT EXISTS `zk_objectdata_log` (
  `obl_id` int(11) NOT NULL auto_increment,
  `obl_object_type` varchar(80) collate latin1_german2_ci NOT NULL default '',
  `obl_object_id` int(11) NOT NULL default '0',
  `obl_op` varchar(16) collate latin1_german2_ci NOT NULL default '',
  `obl_diff` text collate latin1_german2_ci,
  `obl_obj_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `obl_cr_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `obl_cr_uid` int(11) NOT NULL default '0',
  `obl_lu_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `obl_lu_uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`obl_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_objectdata_log`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_objectdata_meta`
--

CREATE TABLE IF NOT EXISTS `zk_objectdata_meta` (
  `obm_id` int(11) NOT NULL auto_increment,
  `obm_module` varchar(40) collate latin1_german2_ci NOT NULL default '',
  `obm_table` varchar(40) collate latin1_german2_ci NOT NULL default '',
  `obm_idcolumn` varchar(40) collate latin1_german2_ci NOT NULL default '',
  `obm_obj_id` int(11) NOT NULL default '0',
  `obm_permissions` varchar(255) collate latin1_german2_ci default '',
  `obm_dc_title` varchar(80) collate latin1_german2_ci default '',
  `obm_dc_author` varchar(80) collate latin1_german2_ci default '',
  `obm_dc_subject` varchar(255) collate latin1_german2_ci default '',
  `obm_dc_keywords` varchar(128) collate latin1_german2_ci default '',
  `obm_dc_description` varchar(255) collate latin1_german2_ci default '',
  `obm_dc_publisher` varchar(128) collate latin1_german2_ci default '',
  `obm_dc_contributor` varchar(128) collate latin1_german2_ci default '',
  `obm_dc_startdate` datetime default '1970-01-01 00:00:00',
  `obm_dc_enddate` datetime default '1970-01-01 00:00:00',
  `obm_dc_type` varchar(128) collate latin1_german2_ci default '',
  `obm_dc_format` varchar(128) collate latin1_german2_ci default '',
  `obm_dc_uri` varchar(255) collate latin1_german2_ci default '',
  `obm_dc_source` varchar(128) collate latin1_german2_ci default '',
  `obm_dc_language` varchar(32) collate latin1_german2_ci default '',
  `obm_dc_relation` varchar(255) collate latin1_german2_ci default '',
  `obm_dc_coverage` varchar(64) collate latin1_german2_ci default '',
  `obm_dc_entity` varchar(64) collate latin1_german2_ci default '',
  `obm_dc_comment` varchar(255) collate latin1_german2_ci default '',
  `obm_dc_extra` varchar(255) collate latin1_german2_ci default '',
  `obm_obj_status` varchar(1) collate latin1_german2_ci NOT NULL default 'A',
  `obm_cr_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `obm_cr_uid` int(11) NOT NULL default '0',
  `obm_lu_date` datetime NOT NULL default '1970-01-01 00:00:00',
  `obm_lu_uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`obm_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_objectdata_meta`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_pagelock`
--

CREATE TABLE IF NOT EXISTS `zk_pagelock` (
  `plock_id` int(11) NOT NULL auto_increment,
  `plock_name` varchar(100) collate latin1_german2_ci NOT NULL default '',
  `plock_cdate` datetime NOT NULL,
  `plock_edate` datetime NOT NULL,
  `plock_session` varchar(50) collate latin1_german2_ci NOT NULL,
  `plock_title` varchar(100) collate latin1_german2_ci NOT NULL,
  `plock_ipno` varchar(30) collate latin1_german2_ci NOT NULL,
  PRIMARY KEY  (`plock_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_pagelock`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_sc_anticracker`
--

CREATE TABLE IF NOT EXISTS `zk_sc_anticracker` (
  `pn_hid` int(11) NOT NULL auto_increment,
  `pn_hacktime` varchar(20) collate latin1_german2_ci default NULL,
  `pn_hackfile` varchar(255) collate latin1_german2_ci default '',
  `pn_hackline` int(11) default NULL,
  `pn_hacktype` varchar(255) collate latin1_german2_ci default '',
  `pn_hackinfo` longtext collate latin1_german2_ci,
  `pn_userid` int(11) default NULL,
  `pn_browserinfo` longtext collate latin1_german2_ci,
  `pn_requestarray` longtext collate latin1_german2_ci,
  `pn_gettarray` longtext collate latin1_german2_ci,
  `pn_postarray` longtext collate latin1_german2_ci,
  `pn_serverarray` longtext collate latin1_german2_ci,
  `pn_envarray` longtext collate latin1_german2_ci,
  `pn_cookiearray` longtext collate latin1_german2_ci,
  `pn_filesarray` longtext collate latin1_german2_ci,
  `pn_sessionarray` longtext collate latin1_german2_ci,
  PRIMARY KEY  (`pn_hid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_sc_anticracker`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_sc_log_event`
--

CREATE TABLE IF NOT EXISTS `zk_sc_log_event` (
  `lge_id` int(11) NOT NULL auto_increment,
  `lge_date` datetime default NULL,
  `lge_uid` int(11) default NULL,
  `lge_component` varchar(64) collate latin1_german2_ci default NULL,
  `lge_module` varchar(64) collate latin1_german2_ci default NULL,
  `lge_type` varchar(64) collate latin1_german2_ci default NULL,
  `lge_function` varchar(64) collate latin1_german2_ci default NULL,
  `lge_sec_component` varchar(64) collate latin1_german2_ci default NULL,
  `lge_sec_instance` varchar(64) collate latin1_german2_ci default NULL,
  `lge_sec_permission` varchar(64) collate latin1_german2_ci default NULL,
  `lge_message` varchar(255) collate latin1_german2_ci default '',
  PRIMARY KEY  (`lge_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_sc_log_event`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_search_result`
--

CREATE TABLE IF NOT EXISTS `zk_search_result` (
  `sres_id` int(11) NOT NULL auto_increment,
  `sres_title` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `sres_text` longtext collate latin1_german2_ci,
  `sres_module` varchar(100) collate latin1_german2_ci default NULL,
  `sres_extra` varchar(100) collate latin1_german2_ci default NULL,
  `sres_created` datetime default NULL,
  `sres_found` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `sres_sesid` varchar(50) collate latin1_german2_ci default NULL,
  PRIMARY KEY  (`sres_id`),
  KEY `title` (`sres_title`),
  KEY `module` (`sres_module`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_search_result`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_search_stat`
--

CREATE TABLE IF NOT EXISTS `zk_search_stat` (
  `pn_id` int(11) NOT NULL auto_increment,
  `pn_search` varchar(50) collate latin1_german2_ci NOT NULL default '',
  `pn_count` int(11) NOT NULL default '0',
  `pn_date` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`pn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_search_stat`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_session_info`
--

CREATE TABLE IF NOT EXISTS `zk_session_info` (
  `pn_sessid` varchar(40) collate latin1_german2_ci NOT NULL default '',
  `pn_ipaddr` varchar(32) collate latin1_german2_ci NOT NULL default '',
  `pn_lastused` datetime default '1970-01-01 00:00:00',
  `pn_uid` int(11) default '0',
  `pn_remember` tinyint(4) NOT NULL default '0',
  `pn_vars` longtext collate latin1_german2_ci NOT NULL,
  PRIMARY KEY  (`pn_sessid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;

--
-- Dumping data for table `zk_session_info`
--

INSERT INTO `zk_session_info` (`pn_sessid`, `pn_ipaddr`, `pn_lastused`, `pn_uid`, `pn_remember`, `pn_vars`) VALUES
('cf7230bbf42fe574dd85ec888ffa0495', 'a7ea6ab31cc4a2f7221b7691418b2ba9', '2008-12-20 14:39:40', 2, 0, 'PNSVrand|s:38:"XU0c5cW$Zp]2tm|qYZawu1j=c+Lh_R*PjPR^Rm";PNSVuseragent|s:40:"f731549a8eed9a0873ac20f67cbef21ef7d2d527";PNSVlang|s:3:"eng";PNSVencoding|s:10:"ISO-8859-1";PNSVuid|i:2;');

-- --------------------------------------------------------

--
-- Table structure for table `zk_themes`
--

CREATE TABLE IF NOT EXISTS `zk_themes` (
  `pn_id` int(11) NOT NULL auto_increment,
  `pn_name` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_type` tinyint(4) NOT NULL default '0',
  `pn_displayname` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_description` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_regid` int(11) NOT NULL default '0',
  `pn_directory` varchar(64) collate latin1_german2_ci NOT NULL default '',
  `pn_version` varchar(10) collate latin1_german2_ci NOT NULL default '0',
  `pn_official` tinyint(4) NOT NULL default '0',
  `pn_author` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_contact` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_admin` tinyint(4) NOT NULL default '0',
  `pn_user` tinyint(4) NOT NULL default '0',
  `pn_system` tinyint(4) NOT NULL default '0',
  `pn_state` tinyint(4) NOT NULL default '0',
  `pn_credits` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_changelog` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_help` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_license` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_xhtml` tinyint(4) NOT NULL default '1',
  PRIMARY KEY  (`pn_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=8 ;

--
-- Dumping data for table `zk_themes`
--

INSERT INTO `zk_themes` (`pn_id`, `pn_name`, `pn_type`, `pn_displayname`, `pn_description`, `pn_regid`, `pn_directory`, `pn_version`, `pn_official`, `pn_author`, `pn_contact`, `pn_admin`, `pn_user`, `pn_system`, `pn_state`, `pn_credits`, `pn_changelog`, `pn_help`, `pn_license`, `pn_xhtml`) VALUES
(1, 'Printer', 3, 'Printer', 'Display pages in a printer friendly format', 0, 'Printer', '2.0', 0, 'Mark West', 'http://www.markwest.me.uk', 0, 0, 1, 1, '', '', '', '', 1),
(2, 'SeaBreeze', 3, 'SeaBreeze', 'The theme has been updated with the release of Zikula 1.0. It is valid CSS and XHTML. \r\nI updated the colors of the theme and added some great new images.', 0, 'SeaBreeze', '3', 0, 'Vanessa Haakenson, Mark West, Martin Andersen', 'http://www.Designs4Zikula.com', 0, 1, 0, 1, '', '', '', '', 1),
(3, 'Andreas08', 3, 'Andreas08', 'The ''Andreas08'' theme - a very good template for light, CSS-compatible themes.', 0, 'andreas08', '1.1', 0, 'David Brucas, Mark West, Andreas Viklund', 'http://dbrucas.povpromotions.com, http://www.markwest.me.uk, http://www.andreasviklund.com', 1, 1, 0, 1, '', '', '', '', 1),
(4, 'VoodooDolly', 3, 'VoodooDolly', 'The ''Voodoo Dolly'' theme - a conservative but Web 2.0 look and feel, ready to go.', 0, 'voodoodolly', '1.0', 0, 'Mark West, pogy366', 'http://www.markwest.me.uk, http://www.dbfnetwork.info/rayk/index.html', 0, 1, 0, 1, '', '', '', '', 1),
(5, 'Atom', 3, 'Atom', 'The ''Atom'' theme, for rendering pages in Atom mark-up', 0, 'Atom', '1.0', 0, 'Franz Skaaning', 'http://www.lexebus.net/', 0, 0, 1, 1, '', '', '', '', 0),
(6, 'RSS', 3, 'RSS', 'An auxiliary theme to display pages as an RSS feed.', 0, 'rss', '1.0', 0, 'Mark West', 'http://www.markwest.me.uk', 0, 0, 1, 1, 'docs/credits.txt', 'docs/changelog.txt', 'docs/help.txt', 'docs/license.txt', 0),
(7, 'ExtraLite', 1, 'ExtraLite', 'Legacy theme used during installation and upgrades, or for troubleshooting.', 0, 'ExtraLite', '1.0', 0, 'Zikula Development Team', 'http://www.zikula.org', 0, 0, 1, 1, 'docs/credits.txt', 'docs/changelog.txt', 'docs/help.txt', 'docs/license.txt', 1);

-- --------------------------------------------------------

--
-- Table structure for table `zk_userblocks`
--

CREATE TABLE IF NOT EXISTS `zk_userblocks` (
  `pn_uid` int(11) NOT NULL default '0',
  `pn_bid` int(11) NOT NULL default '0',
  `pn_active` tinyint(4) NOT NULL default '1',
  `pn_last_update` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  KEY `bid_uid_idx` (`pn_uid`,`pn_bid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;

--
-- Dumping data for table `zk_userblocks`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_users`
--

CREATE TABLE IF NOT EXISTS `zk_users` (
  `pn_uid` int(11) NOT NULL auto_increment,
  `pn_uname` varchar(25) collate latin1_german2_ci NOT NULL default '',
  `pn_email` varchar(60) collate latin1_german2_ci NOT NULL default '',
  `pn_user_regdate` datetime NOT NULL default '1970-01-01 00:00:00',
  `pn_user_viewemail` smallint(6) default '0',
  `pn_user_theme` varchar(64) collate latin1_german2_ci default '',
  `pn_pass` varchar(128) collate latin1_german2_ci NOT NULL default '',
  `pn_storynum` int(4) NOT NULL default '10',
  `pn_ublockon` tinyint(4) NOT NULL default '0',
  `pn_ublock` text collate latin1_german2_ci,
  `pn_theme` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `pn_counter` int(11) NOT NULL default '0',
  `pn_activated` tinyint(4) NOT NULL default '0',
  `pn_lastlogin` datetime NOT NULL default '1970-01-01 00:00:00',
  `pn_validfrom` int(11) NOT NULL default '0',
  `pn_validuntil` int(11) NOT NULL default '0',
  `pn_hash_method` tinyint(4) NOT NULL default '8',
  PRIMARY KEY  (`pn_uid`),
  KEY `uname` (`pn_uname`),
  KEY `email` (`pn_email`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=3 ;

--
-- Dumping data for table `zk_users`
--

INSERT INTO `zk_users` (`pn_uid`, `pn_uname`, `pn_email`, `pn_user_regdate`, `pn_user_viewemail`, `pn_user_theme`, `pn_pass`, `pn_storynum`, `pn_ublockon`, `pn_ublock`, `pn_theme`, `pn_counter`, `pn_activated`, `pn_lastlogin`, `pn_validfrom`, `pn_validuntil`, `pn_hash_method`) VALUES
(1, 'guest', '', '1970-01-01 00:00:00', 0, '', '', 10, 0, '', '', 0, 1, '1970-01-01 00:00:00', 0, 0, 1),
(2, 'admin', 'admin@example.com', '2008-12-20 14:37:46', 0, '', '9d70f482fc4f15ba2d9bd53402f17896432f5486a23118385c3489f226ea6afa', 10, 0, '', '', 0, 1, '2008-12-20 14:37:46', 0, 0, 8);

-- --------------------------------------------------------

--
-- Table structure for table `zk_users_temp`
--

CREATE TABLE IF NOT EXISTS `zk_users_temp` (
  `pn_tid` int(11) NOT NULL auto_increment,
  `pn_uname` varchar(25) collate latin1_german2_ci NOT NULL default '',
  `pn_email` varchar(60) collate latin1_german2_ci NOT NULL default '',
  `pn_femail` tinyint(4) NOT NULL default '0',
  `pn_pass` varchar(128) collate latin1_german2_ci NOT NULL default '',
  `pn_dynamics` longtext collate latin1_german2_ci NOT NULL,
  `pn_comment` varchar(254) collate latin1_german2_ci NOT NULL default '',
  `pn_type` tinyint(4) NOT NULL default '0',
  `pn_tag` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`pn_tid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_users_temp`
--


-- --------------------------------------------------------

--
-- Table structure for table `zk_workflows`
--

CREATE TABLE IF NOT EXISTS `zk_workflows` (
  `id` int(11) NOT NULL auto_increment,
  `metaid` int(11) NOT NULL default '0',
  `module` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `schemaname` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `state` varchar(255) collate latin1_german2_ci NOT NULL default '',
  `type` smallint(6) NOT NULL default '1',
  `obj_table` varchar(40) collate latin1_german2_ci NOT NULL default '',
  `obj_idcolumn` varchar(40) collate latin1_german2_ci NOT NULL default '',
  `obj_id` int(11) NOT NULL default '0',
  `busy` int(11) NOT NULL default '0',
  `debug` longblob,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci AUTO_INCREMENT=1 ;

--
-- Dumping data for table `zk_workflows`
--

