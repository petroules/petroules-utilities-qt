#include "explorersearchlineedit.h"
#include <QtGui>

class ExplorerSearchLineEdit::Private
{
public:
    QToolButton *searchButton;
};

ExplorerSearchLineEdit::ExplorerSearchLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    d = new Private();

    // Create the search button and set its icon, cursor, and stylesheet
    d->searchButton = new QToolButton(this);
    d->searchButton->setFixedSize(18, 18);
    d->searchButton->setCursor(Qt::ArrowCursor);
    d->searchButton->setStyleSheet(this->buttonStyleSheetForCurrentState());

    // Update the search button when the text changes
    QObject::connect(this, SIGNAL(textChanged(QString)), SLOT(updateSearchButton(QString)));

    // Some stylesheet and size corrections for the text box
    setPlaceholderText(tr("Search"));
    setStyleSheet(this->styleSheetForCurrentState());

    int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    QSize minSizeHint = this->minimumSizeHint();
    setMinimumSize(qMax(minSizeHint.width(), d->searchButton->sizeHint().width() + frameWidth * 2 + 2),
                         qMax(minSizeHint.height(), d->searchButton->sizeHint().height() + frameWidth * 2 + 2));
    setSizePolicy(QSizePolicy::Maximum, sizePolicy().verticalPolicy());

    setFixedSize(200, 24);
}

ExplorerSearchLineEdit::~ExplorerSearchLineEdit()
{
    delete d;
}

void ExplorerSearchLineEdit::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QSize size = d->searchButton->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    d->searchButton->move(rect().right() - frameWidth - size.width() - 2, (rect().bottom() + 2 - size.height()) / 2);
}

void ExplorerSearchLineEdit::updateSearchButton(const QString &text)
{
    if (!text.isEmpty())
    {
        // We have some text in the box - set the button to clear the text
        QObject::connect(d->searchButton, SIGNAL(clicked()), SLOT(clear()));
    }
    else
    {
        // The text box is empty - make the icon do nothing when clicked
        QObject::disconnect(d->searchButton, SIGNAL(clicked()), this, SLOT(clear()));
    }

    setStyleSheet(styleSheetForCurrentState());
    d->searchButton->setStyleSheet(buttonStyleSheetForCurrentState());
}

QString ExplorerSearchLineEdit::styleSheetForCurrentState() const
{
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    QString style;
    style += "QLineEdit {";
    if (text().isEmpty())
    {
        style += "font-family: 'MS Sans Serif';";
        style += "font-style: italic;";
    }

    style += "padding-left: 3px;";
    style += QString("padding-right: %1px;").arg(d->searchButton->sizeHint().width() + frameWidth + 1);
    style += "border-width: 3px;";
    style += "border-image: url(:/res/images/esf-border.png) 3 3 3 3 stretch;";
    style += "background-color: rgba(255, 255, 255, 204);";
    style += "}";
    style += "QLineEdit:hover, QLineEdit:focus {";
    style += "background-color: rgba(255, 255, 255, 255);";
    style += "}";
    return style;
}

QString ExplorerSearchLineEdit::buttonStyleSheetForCurrentState() const
{
    QString style;
    style += "QToolButton {";
    style += "border: none; margin: 0; padding: 0;";
    style += QString("background-image: url(:/res/images/esf-%1.png);").arg(text().isEmpty() ? "search" : "clear");
    style += "}";

    if (!text().isEmpty())
    {
        style += "QToolButton:hover { background-image: url(:/res/images/esf-clear-hover.png); }";
        style += "QToolButton:pressed { background-image: url(:/res/images/esf-clear-active.png); }";
    }

    return style;
}
