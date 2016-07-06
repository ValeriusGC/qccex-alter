CREATE TABLE IF NOT EXISTS `tbl_notes_dbg`(
	`_id` int(11) NOT NULL AUTO_INCREMENT,
	`fk_author` int(11) NOT NULL,
	`the_text` varchar(255),
	`ts_create` bigint(16) NOT NULL,
	`ts_edit` bigint(16) NOT NULL,
	`dt_create` varchar(80),
	`dt_edit` varchar(80),
	`deleted` int(3) NOT NULL DEFAULT 0,
	`uuid` varchar(255) NOT NULL DEFAULT '',
  	PRIMARY KEY (`_id`),
    UNIQUE KEY `id_UNIQUE` (`uuid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT='note table' AUTO_INCREMENT=1 ;


