.. index::
   single: Forms; Fields; datetime

.. note::

   * 対象バージョン：2.3+
   * 翻訳更新日：2014/04/23

datetime フィールドタイプ
=========================

このフィールドタイプは、特定の日付と時間（例 ``1984-06-05 12:15:30`` ）を表すデータを変更することができます。

テキスト入力またはセレクトタグとして表示されます。
元となるデータのフォーマットは ``DateTime`` オブジェクト、 ``string`` 、 ``timestamp`` または ``array`` です。

+----------------------+------------------------------------------------------------------------------------------+
| 対応するデータ型     | ``DateTime``, ``string``, ``timestamp``, ``array``  (参照 ``input`` オプション )         |
+----------------------+------------------------------------------------------------------------------------------+
| 対応するタグ         | 単一のテキストボックス、または、3つのセレクトフィールド                                  |
+----------------------+------------------------------------------------------------------------------------------+
| オプション           | - `date_widget`_                                                                         |
|                      | - `time_widget`_                                                                         |
|                      | - `widget`_                                                                              |
|                      | - `input`_                                                                               |
|                      | - `date_format`_                                                                         |
|                      | - `format`_                                                                              |
|                      | - `hours`_                                                                               |
|                      | - `minutes`_                                                                             |
|                      | - `seconds`_                                                                             |
|                      | - `years`_                                                                               |
|                      | - `months`_                                                                              |
|                      | - `days`_                                                                                |
|                      | - `with_minutes`_                                                                        |
|                      | - `with_seconds`_                                                                        |
|                      | - `model_timezone`_                                                                      |
|                      | - `view_timezone`_                                                                       |
|                      | - `empty_value`_                                                                         |
+----------------------+------------------------------------------------------------------------------------------+
| 継承された           | - `data`_                                                                                |
| オプション           | - `invalid_message`_                                                                     |
|                      | - `invalid_message_parameters`_                                                          |
|                      | - `read_only`_                                                                           |
|                      | - `disabled`_                                                                            |
|                      | - `mapped`_                                                                              |
|                      | - `inherit_data`_                                                                        |
+----------------------+------------------------------------------------------------------------------------------+
| 親タイプ             | :doc:`form </reference/forms/types/form>`                                                |
+----------------------+------------------------------------------------------------------------------------------+
| クラス               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateTimeType`                   |
+----------------------+------------------------------------------------------------------------------------------+

フィールドオプション
--------------------

date_widget
~~~~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``choice``

:doc:`date </reference/forms/types/date>` タイプの ``widget`` オプションを定義します。

time_widget
~~~~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``choice``

:doc:`time </reference/forms/types/time>` タイプの ``widget`` オプションを定義します。

widget
~~~~~~

**データ型**: ``string`` **デフォルト**: ``null``

:doc:`date </reference/forms/types/date>` タイプと、:doc:`time </reference/forms/types/time>` タイプの ``widget`` オプションを両方定義します。
これは  `date_widget`_ と `time_widget`_ オプションを上書きすることが出来ます。

input
~~~~~

**データ型**: ``string`` **デフォルト**: ``datetime``

*input* データのフォーマット、すなわち、対応するオブジェクトに保存される日付のフォーマットの有効な値は次のとおりです。:

* ``string`` (例 ``2011-06-05 12:15:00``)
* ``datetime`` (単一の ``DateTime`` オブジェクト)
* ``array`` (例 ``array(2011, 06, 05, 12, 15, 0)``)
* ``timestamp`` (例 ``1307276100``)

フォームから戻って来る値も、この形式に戻して正規化されます。

.. include:: /reference/forms/types/options/_date_limitation.rst.inc

date_format
~~~~~~~~~~~

**データ型**: ``integer`` or ``string`` **デフォルト**: ``IntlDateFormatter::MEDIUM``

``format`` オプションの定義は ``date`` フィールドに渡されます。
より詳細は ::ref:`date type's format option <reference-forms-type-date-format>` を参照してください。

format
~~~~~~

**データ型**: ``string`` **デフォルト**: ``Symfony\Component\Form\Extension\Core\Type\DateTimeType::HTML5_FORMAT``

``widget`` オプションを ``single_text`` にセットした場合、このオプションは入力フォーマットを特定します。
すなわち、 Symfony が入力を日時文字列としてどのように解釈するかを指定します。
デフォルトでは `RFC3339`_ フォーマットが HTML5の ``datetime`` フィールドに利用されます。
デフォルト値を維持することでフィールドが ``type="datetime"`` 付の ``input`` フィールドとしてレンダリングされます。

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/with_minutes.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

フィールド変数
--------------

+----------+------------+---------------------------------------------------------------------------------------+
| 変数     | データ型   | 使用法                                                                                |
+==========+============+=======================================================================================+
| widget   | ``mixed``  | `widget`_ オプションの値                                                              |
+----------+------------+---------------------------------------------------------------------------------------+
| type     | ``string`` | ``widget`` が ``single_text`` で、且つ、 HTML5 が有効な場合のみ、                     |
|          |            | インプットタイプ( ``datetime``, ``date``  または ``time`` )を使用できます。           |
+----------+------------+---------------------------------------------------------------------------------------+


.. _`RFC 3339`: http://tools.ietf.org/html/rfc3339

.. 2014/04/23 yositani2002 d49d12eaf265a5d6d32ac660c62f385d57261475
