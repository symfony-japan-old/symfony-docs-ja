.. index::
   single: Forms; Fields; percent

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2013/12/22

percent フィールドタイプ
========================

"パーセント"タイプはテキストフィールドを表示し、パーセンテージのデータを取り扱うことに特化しています。パーセンテージのデータが少数として保存される場合(例 ``0.95`` )、このフィールドをすぐに使うことが出来ます。データを数値(例 ``95`` )として保持する場合、 ``type`` オプションに ``integer`` を指定すべきです。

このフィールドはパーセンテージ記号 "``%``" をインプットボックスの後に追加します。

+-------------+-----------------------------------------------------------------------+
| 対応するタグ| ``input`` ``text`` フィールド                                         |
+-------------+-----------------------------------------------------------------------+
| オプション  | - `type`_                                                             |
|             | - `precision`_                                                        |
+-------------+-----------------------------------------------------------------------+
| 継承された  | - `required`_                                                         |
| オプション  | - `label`_                                                            |
|             | - `read_only`_                                                        |
|             | - `disabled`_                                                         |
|             | - `error_bubbling`_                                                   |
|             | - `error_mapping`_                                                    |
|             | - `invalid_message`_                                                  |
|             | - `invalid_message_parameters`_                                       |
|             | - `mapped`_                                                           |
+-------------+-----------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                             |
+-------------+-----------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PercentType` |
+-------------+-----------------------------------------------------------------------+

オプション
----------

type
~~~~

**データ型**: ``string`` **デフォルト**: ``fractional``

このオプションはデータがオブジェクトにどのように保存されているかを指定します。たとえば、パーセンテージが "55%" に対してデータは ``0.55`` または ``55`` のようにオブジェクトに保存されているでしょう。2種類の "type" でそれらの2つのケースを取り扱います。:
    
*   ``fractional``
    ( ``0.55`` のような)少数としてデータが保存されている場合、このタイプを利用します。データがユーザーに( ``55`` のように)表示される前に ``100`` を掛けられます。フォームから送信されたデータは ``100`` で割られ少数( ``0.55`` )として保存されます。

*   ``integer``
    データが整数(例 ``55`` )として保存される場合、このオプションを使用してください。そのままの値 ( ``55`` )が表示され、オブジェクトに保存されます。整数でのみ機能しますので注意してください。

precision
~~~~~~~~~

**データ型**: ``integer`` **デフォルト**: ``0``

デフォルトでは、入力された数値は端数処理されます。少数点以下の桁数を増やす場合このオプションを使用します。

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/12/22 yositani2002 4848f40b7de1463e40911bc2871d8990757d0097
