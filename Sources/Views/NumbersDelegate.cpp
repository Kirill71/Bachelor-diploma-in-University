
#include "Views/NumbersDelegate.hpp"

/*---------------------------------------------------------------------------*/

QWidget*
NumbersDelegate::createEditor(
        QWidget* _parent
    ,   const QStyleOptionViewItem & /*_option*/
    ,   const QModelIndex & /*_index*/
) const
{
    auto lineEdit = new QLineEdit( _parent );
    // Set validator
    auto validator = new QDoubleValidator( 0.0, 9.0, 6, lineEdit );
    lineEdit->setValidator( validator );

    return lineEdit;

} // NumbersDelegate::createEditor

/*---------------------------------------------------------------------------*/
