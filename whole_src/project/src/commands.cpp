/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "commands.h"
#include "sqlitestorage.h"
#include "notemodel.h"

using namespace model;
using namespace storage;

/******************************************************************************
** MarkAsDeletedCommand
*/

MarkAsDeletedCommand::MarkAsDeletedCommand(const UuidVector_t &ids, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_ids = ids;
}

void MarkAsDeletedCommand::undo()
{
    StorageObjectOperations *storage = &SqliteStorage::instance();
    if(storage->isInit()){
        storage->markNotesAsDeleted(20, m_ids, false);
    }
}

void MarkAsDeletedCommand::redo()
{
    StorageObjectOperations *storage = &SqliteStorage::instance();
    if(storage->isInit()){
        storage->markNotesAsDeleted(20, m_ids, true);
    }
}

AddNoteCommand::AddNoteCommand(Note *note, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_note = note;
}

AddNoteCommand::~AddNoteCommand()
{
    delete m_note;
}

void AddNoteCommand::undo()
{
    StorageObjectOperations *storage = &SqliteStorage::instance();
    if(storage->isInit()){
        UuidVector_t ids;
        ids.append(m_note->uuid());
        storage->removeNotes(TASK_ID_DEL_NOTES, ids);
    }
}

void AddNoteCommand::redo()
{
    StorageObjectOperations *storage = &SqliteStorage::instance();
    if(storage->isInit()){
        QSharedPointer<Notes> sp(new Notes);
        sp->items.append(m_note->copy());
        storage->addNotes(TASK_ID_ADD_NOTES, sp);
    }
}


EditNoteCommand::EditNoteCommand(Note *oldNote, Note *editedNote, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_oldNote = oldNote;
    m_editedNote = editedNote;
}

EditNoteCommand::~EditNoteCommand()
{
    delete m_oldNote;
    delete m_editedNote;
}

void EditNoteCommand::undo()
{
    StorageObjectOperations *storage = &SqliteStorage::instance();
    if(storage->isInit()){
        QSharedPointer<Notes> sp(new Notes);
        sp->items.append(m_oldNote->copy());
        storage->editNotes(TASK_ID_EDIT_NOTES, sp);
    }
}

void EditNoteCommand::redo()
{
    StorageObjectOperations *storage = &SqliteStorage::instance();
    if(storage->isInit()){
        QSharedPointer<Notes> sp(new Notes);
        sp->items.append(m_editedNote->copy());
        storage->editNotes(TASK_ID_EDIT_NOTES, sp);
    }
}
