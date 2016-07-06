CREATE TABLE IF NOT EXISTS `tbl_notes_dbg`(
	`_id` int(11) NOT NULL AUTO_INCREMENT,
	`fk_author` int(11) NOT NULL,
	`the_text` varchar(255),
	`ts_create` bigint(16) NOT NULL,
	`ts_edit` bigint(16) NOT NULL,
	`dt_create` varchar(80) NOT NULL,
	`dt_edit` varchar(80) NOT NULL,
	`deleted` int(3) NOT NULL DEFAULT 0,
	`uuid` varchar(255) NOT NULL DEFAULT '',
  	PRIMARY KEY (`_id`),
    UNIQUE KEY `id_UNIQUE` (`uuid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT='note table' AUTO_INCREMENT=1 ;

BEGIN;

INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 1,0,'from server - First',1465373116950,1465389248015,0,'kpb1F9bFRziPR1TBGFxlpQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 2,0,'from server - Seconfd',1465376395416,1465388262089,0,'QOojlgttToeg5oqmVWWKkg==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 3,0,'from server - 3',1465376507830,1465388260513,0,'JtTYXjI/TfGTLOg1vZra/g==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 4,0,'from server - 4',1465376526294,1465389248551,0,'ZOa5NUfTRD28iZxDEfIaFQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 5,0,'from server - 5',1465388267043,1465389249071,0,'jVz9l+KhRhadDUV72o7UUA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 6,0,'from server - 6',1465388677040,1465389251055,0,'EiuzAs3RQRKIXtLqeH+Few==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 7,0,'from server - 7',1465388704264,1465389247479,0,'Y3A54CmtTKSoe1i66EOPWQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 8,0,'1',1465389258542,1465394726263,0,'C9tL5jnQQ/OorZPXlzxrVQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 9,0,'2',1465389266295,1465390327339,0,'h600aeUzTB61UpvIzQh44g==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 10,0,'3',1465389445872,1465390328619,0,'mfXtafbLTkGwulehlBIBXw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 11,0,'4',1465389540421,1465390327995,0,'wqMs4DV/RoKuRgYAHchpBQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 12,0,'5',1465389658597,1465390329963,0,'AtGQIsFNRt6XNVdoMydHaw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 13,0,'7',1465389989972,1465390322603,0,'MFlQ4iPKR3yIt12zrKGUMQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 14,0,'8',1465390011566,1465390296556,0,'WikAkLP3SVO/tghE4cE0Yg==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 15,0,'2',1465390371979,1465394723047,0,'ReS7Qmm7TGybFYdGHUS1Bg==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 16,0,'3',1465390634714,1465394725791,0,'sCWg7ihnTcm3KWxknEio+A==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 17,0,'0',1465390640786,1465391245185,0,'y5uExY2eTHqdKLRxDY3jIw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 18,0,'6',1465390660268,1465394725358,0,'C6cEOpnoSkCS8C2gMzd7CA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 19,0,'e',1465391221603,1465394724135,0,'KRa+RUrCRsqY8qDU6O+uuQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 20,0,'fwefr',1465394721062,1465394724911,0,'QNZiRk1zT5SYFKCzg3bBcQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 21,0,'ewrfe',1465394728727,1465467781905,0,'ELfFD8bWR7yE+0LMDhtGIg==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 22,0,'2',1465394732407,1465467400050,0,'Z4Kvy4KwRWapSGxit4oZHw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 23,0,'5',1465450373756,1465450377700,0,'jWjDczRJT5GdY2iREM+9xQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 24,0,'e',1465455244496,1465456084079,0,'Nv4zuN6fSVyQq03pzRyjJw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 25,0,'wxsw',1465455252865,1465531186320,0,'FqEQYXEiT0S1GWHzY1Brjw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 26,0,'4',1465467787785,1465468166976,0,'eCa+NMD7Qj6Kod5oJe8KMQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 27,0,'wsws',1465531507463,1465531510343,0,'28Y3ykCfTdi3cIjzZ3Ajog==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 28,0,'44',1465532466965,1465536484717,0,'EvjNZ3KFRXKpsvZXo76VgA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 29,0,'55',1465532502077,1465532557517,0,'DvZg4MJYQAyJm24Q++rs7g==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 30,0,'11',1465532509701,1465554922095,0,'Uo85KQlgQ/2gZUiHGsCdpQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 31,0,'33',1465532724612,1465556706050,1,'YwImK822T06khnqYJO5/zw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 32,0,'22',1465532729197,1465532966436,0,'/IKvKXcYTHKhZufRD6prrQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 33,0,'w',1465536451324,1465536471563,0,'hwWpBVvYS2mIsx85e3tYew==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 34,0,'33',1465536455349,1465554917752,0,'vXY/7Vt5QaiFFM73VtGByQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 35,0,'aa11',1465559312895,1466329155506,0,'zJRINXD6QeeJ+Z7yRdLNcw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 36,0,'AAA',1465560290043,1466329033826,0,'4U1olJDgRnWVerHCFufEGw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 37,0,'13 июня',1465804275782,1465804275782,0,'3laDfcRWQhid1ITpfvTfdQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 38,0,'19_2',1466329857483,1466329857483,0,'w6wBgjYmRaKBSmo/KR7rHA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 39,0,'19_3',1466329953239,1466329953239,0,'7HqAtr2oQG2GwXlIzpg7MA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 40,0,'20_1',1466404943373,1466408842305,0,'wd0Lhn3dTRalrSqUbLKlJA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 41,0,'20_2',1466405087750,1466408846081,0,'QArHDVtAToSNwx11S3pI0Q==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 42,0,'20_3',1466406615886,1466415480386,0,'wqWBgAkMSjmOyaPBFrDH/g==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 43,0,'20_4',1466406669708,1466414044209,0,'8tZBw+hxRd225gAOpBlegg==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 44,0,'MANUALLY',1466406669718,1466406669718,0,'8tZBw+hxRd225gAOpBleggG=' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 45,0,'20_5',1466417876598,1466417876598,0,'MTHkYP7ARXiASWKm97WXFQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 46,0,'20_6',1466418180606,1466418214087,0,'Wx9C/KQPQoqnu/gAmqWS7g==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 47,0,'20_7',1466418238478,1466418238478,0,'7J1MaDq1RruP6v9HPl9heA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 48,0,'20_8',1466418709716,1466418709716,0,'+EaYcgFbTaCx1Mz+Adq0ZQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 49,0,'20_9',1466418950071,1466418950071,0,'x6PYHiSOR4WmVU4BBBFyFA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 50,0,'20_10',1466419970359,1466480539092,0,'30quBwGvQD6CTfwKXUvCcA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 51,0,'20_12',1466420451903,1466421579129,1,'MIwP+DQeQla+xee5l6TZ0A==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 52,0,'20_13',1466420675735,1466421578489,1,'OZO6dIF9RBG6IQUpWWOCpg==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 53,0,'20_14',1466421373596,1466480538853,0,'Q6HJUQoWTG6bi1n4+gZi9A==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 54,0,'20_15',1466421572367,1466423598641,0,'Zk4GRCwGQQGyNaDh1umOEA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 55,0,'from server - 20_16 edited 27th',1466423604116,1467007530214,0,'Yf6Y0eybTeeiXyp3bhHq/g==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 56,0,'21_0',1466480500640,1466480500640,0,'8wEwvRNsRjiJ/yxnH0fuqQ==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 57,0,'21_1',1466480508041,1466480508041,0,'OdsMQ5gTRQuIhiAQja+sxw==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 58,0,'from server - 21_2edit14',1466484450749,1466493838315,0,'ZDXVnEuqRe+5plJXysLYFA==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 59,0,'from server - q_edited_6',1466491934254,1466495082221,0,'hk+NWdsRTke7Hod1fgpjpg==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 60,0,'from server - 21_6_2',1466493848268,1466507038033,0,'zYApME51TdGvBn9qb+VJ9w==' );
INSERT INTO `tbl_notes`(`_id`,`fk_author`,`the_text`,`ts_create`,`ts_edit`,`deleted`,`uuid`) VALUES ( 61,0,'from server - 21-7_2',1466495097308,1466506892233,0,'VGZt3ARpQ3mupkGB5ffMlg==' );


COMMIT;

