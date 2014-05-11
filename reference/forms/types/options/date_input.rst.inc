input
~~~~~

**データ型**: ``string`` **デフォルト**: ``datetime``

*input* データのフォーマット、すなわち、対応するオブジェクトに保存される日付のフォーマットの有効な値は次のとおりです。:

* ``string`` (例 ``2011-06-05``)
* ``datetime`` (単一の ``DateTime`` オブジェクト)
* ``array`` (例 ``array('year' => 2011, 'month' => 06, 'day' => 05)```)
* ``timestamp`` (例 ``1307276100``)

フォームから戻って来る値も、この形式に戻して正規化されます。

.. include:: /reference/forms/types/options/_date_limitation.rst.inc

.. 2014/05/05 yositani2002 7dacc671f01ab8bf22f35933b06de58f563766bb
