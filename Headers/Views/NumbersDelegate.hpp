#ifndef NUMBERS_DELEGATE_HPP_
#define NUMBERS_DELEGATE_HPP_


/*---------------------------------------------------------------------------*/

#include <QItemDelegate>
#include<QLineEdit>


/*---------------------------------------------------------------------------*/

class NumbersDelegate
        :   public QItemDelegate
{

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    QWidget* createEditor(
                    QWidget* _parent
                  , const QStyleOptionViewItem & /*_option*/
                  , const QModelIndex & /*_index*/
    ) const override;

/*---------------------------------------------------------------------------*/

}; // class NumbersDelegate

/*---------------------------------------------------------------------------*/

#endif //NUMBERS_DELEGATE_HPP_
