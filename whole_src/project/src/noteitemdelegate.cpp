#include <QTextDocument>
#include <QApplication>
#include <QAbstractTextDocumentLayout>

#include "noteitemdelegate.h"
#include "notemodel.h"
#include "shared_def.h"

using namespace nq;
using namespace vfx_shared;
using namespace model;

NoteItemDelegate::NoteItemDelegate(const QString &templ, QObject *parent)
    : Inherited_t(parent),
      m_template(templ)
{
    INC_THIS(true);
}

NoteItemDelegate::~NoteItemDelegate()
{
    DEC_THIS(true);
}

void NoteItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QString output = m_template;
    if(output.isEmpty()) {
        output = index.data().toString();
    }else{
        output.replace("%title%",    "TITLE");
        const qint64 ts = index.data(NoteModel::NMR_TsCreated).toLongLong();
        output.replace("%datetime%", QDateTime::fromMSecsSinceEpoch(ts).toString());
        output.replace("%note%",     index.data().toString());
        output.replace("%tags%",     "taga taga");
    }

    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);
    options.text = "";
    options.state & 0;

    QStyleOptionViewItemV4 optionV4 = option;
    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

    /// Painting item without text
    optionV4.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

    QAbstractTextDocumentLayout::PaintContext ctx;

    // Highlighting text if item is selected
    if (optionV4.state & QStyle::State_Selected)
        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::HighlightedText));


//    qDebug() << "options.rect.height()" << options.rect.height();
    QRect clip(0, 0, options.rect.width(), options.rect.height());
    QTextDocument doc;
    doc.setHtml(output);
    doc.setTextWidth(options.rect.width());

    painter->save();
    painter->translate(options.rect.left(), options.rect.top());
    doc.drawContents(painter, clip);
    painter->restore();


// 16/06/04
//    QStyleOptionViewItemV4 optionV4 = option;
//    initStyleOption(&optionV4, index);

//    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

//    /// Painting item without text
//    optionV4.text = QString();
//    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

//    QAbstractTextDocumentLayout::PaintContext ctx;

//    // Highlighting text if item is selected
//    if (optionV4.state & QStyle::State_Selected)
//        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::HighlightedText));

//    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
//    painter->save();
//    painter->translate(textRect.topLeft());
//    painter->setClipRect(textRect.translated(-textRect.topLeft()));
//    doc.documentLayout()->draw(painter, ctx);
//    painter->restore();



//    QStyleOptionViewItemV4 options = option;
//    initStyleOption(&options, index);

//    painter->save();

//    QTextDocument doc;
//    doc.setHtml(options.text);

//    options.text = "";
//    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

//    // shift text right to make icon visible
//    QSize iconSize = options.icon.actualSize(options.rect.size());
//    painter->translate(options.rect.left()+iconSize.width(), options.rect.top());
//    QRect clip(0, 0, options.rect.width()+iconSize.width(), options.rect.height());

//    //doc.drawContents(painter, clip);

//    painter->setClipRect(clip);
//    QAbstractTextDocumentLayout::PaintContext ctx;
//    // set text color to red for selected item
//    if (option.state & QStyle::State_Selected)
//        ctx.palette.setColor(QPalette::Text, QColor("red"));
//    ctx.clip = clip;
//    doc.documentLayout()->draw(painter, ctx);

//    painter->restore();
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString output = m_template;
    if(output.isEmpty()) {
        output = index.data().toString();
    }else{
        output.replace("%title%",    "TITLE");
        output.replace("%datetime%", index.data(NoteModel::NMR_TsCreated).toString());
        output.replace("%note%",     index.data().toString());
        output.replace("%tags%",     "taga taga");
    }
//    QStyleOptionViewItemV4 options = option;
//    initStyleOption(&options, index);

//    QTextDocument doc;
//    doc.setHtml(options.text);
//    doc.setTextWidth(options.rect.width());
//    return QSize(doc.idealWidth(), doc.size().height());
    QStyleOptionViewItemV4 options = option;
    QTextDocument doc;

    initStyleOption(&options, index);
    doc.setHtml(output);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());

}

