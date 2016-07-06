#include "storage_globals.h"

namespace storage {

//const qint32 V1 = 1;
//const qint32 V2 = 2;
//const qint32 V3 = 3;

const qint32 STORAGE_CURR_VER = STORAGE_V3;

const qint32 Const::SUCCESS = 1; ///< Just result is OK
const qint32 Const::INVALID = -1; ///<

const qint32 Const::E_RES_NO_RESULT = 1001; // No result code in response
const qint32 Const::E_REQ_WRONG_TYPE = 1010; // Request has wrong type. hdr sent back in [body][data]
const qint32 Const::E_BAD_ALGORITHM = 1016; // Bad algorithm in function (maybe in if/else or some…)

const qint32 Const::REQ_TYPE_GET_NOTES = 3110; // Get notes
const qint32 Const::REQ_TYPE_DEL_NOTES = 3111; // Delete notes
const qint32 Const::REQ_TYPE_ADD_NOTES = 3112; // Add notes
const qint32 Const::REQ_TYPE_COUNT_NOTES = 3113; // Get count of notes

const QString Const::FLD_UUID = "uuid"; //Standard UUID-field for Note-object, etc
const QString Const::FLD_DEL = "deleted"; //Standard DELETED-field for Note-object, etc
const QString Const::FLD_AUTHOR_REF = "fk_author"; //AUTHOR-reference for Note-object (ref. to Author-object)
const QString Const::FLD_TS_CREATE = "ts_create"; //Standard creation timestamp for Note-object (int64)
const QString Const::FLD_TS_EDIT = "ts_edit"; //Standard last edit timestamp for Note-object (int64)
const QString Const::FLD_THE_TEXT = "the_text"; //Standard text for Note-object

const QString Const::BODY_SET_NOTES = "notes"; //Set of notes in 'body'
const QString Const::BODY_TS_EDIT_BOTTOM = "ts_edit_bottom"; //Low border for requesting records
const QString Const::BODY_TS_EDIT_TOP = "ts_edit_top"; //High border for requesting records

}

